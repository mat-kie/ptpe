/**
 * @file default_md_adapter.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @version 0.1
 * @date 2022-11-06
 * @copyright Copyright (c) 2022*
 */

#ifndef PTPE_DEFAULT_MDADAPTER_HPP
#define PTPE_DEFAULT_MDADAPTER_HPP
#include <ptpe/interface/concepts.hpp>

namespace ptpe
{
    /// @brief MDAdapter (see IEEE1588 standard) implementation for the standard delay measurement.
    /// @tparam netif the Transport implementation to use for communication.
    template <Transport netif>
    class DefaultMDAdapter
    {
        /// @brief states for the delay measurement state machine.
        enum class DelayState
        {
            WaitForSync,
            DoDelayReq,
            WaitForDelayResp,
            WaitForFollowUp,
        };

        static DerivedTypes<HostEndian>::PortIdentity upstreamPort;
        static DelayState dState;
        static bool isGeneral, timingFlag;
        static Delay_Req delayReq;
        static uint16_t currentSequence;
        static MDSyncReceive timingMsg;
        static TimeInternal t2, t3, t4, syncCorrectionField, FollowUoCorrectionField, mpd;
        static const TimeInternal& t1;

        static bool isFromUpstreamPort(const PTPHeader &header)
        {
            return (upstreamPort.portNumber == header.sourcePortIdentity.portNumber)
            && (upstreamPort.clockIdentity == header.sourcePortIdentity.clockIdentity);
        };

        static void handleSync(const RxResult &rxHandle)
        {
            const Sync &msg = reinterpret_cast<const Sync &>(rxHandle.message);
            if (isFromUpstreamPort(msg.header))
            {
                // store t2 time & sync COrrection Field
                currentSequence = msg.header.sequenceId;
                t2 = rxHandle.timestamp;
                timingMsg.upstreamTxTime = t2 - mpd;
                timingMsg.logMessageInterval = msg.header.logMessageInterval;
                // stroe correction field
                syncCorrectionField = TimeInternal(0,0,msg.header.correctionField);
                if (msg.header.flagField[0] & 0b10)
                {
                    // 2 step flag is set
                    dState = DelayState::WaitForFollowUp;
                }
                else
                {
                    // store t1
                    timingMsg.originOrPreciseOriginTimestamp = msg.originTimestamp;
                    // store cf
                    timingMsg.correctionField = syncCorrectionField;
                    FollowUoCorrectionField = TimeInternal();
                    timingFlag = true;
                    // update MDSyncReceive and raise TimingMsg Flag;
                }
            }
        };

        static void handleFollowUp(const RxResult &rxHandle)
        {
            if (dState == DelayState::WaitForFollowUp)
            {
                const Follow_Up &msg = reinterpret_cast<const Follow_Up &>(rxHandle.message);
                if ((msg.header.sequenceId == currentSequence) && isFromUpstreamPort(msg.header))
                {
                    // store t1 time & sync COrrection Field
                    timingMsg.originOrPreciseOriginTimestamp = msg.preciseOriginTimestamp;
                    FollowUoCorrectionField = TimeInternal(0,0,msg.header.correctionField);
                    timingMsg.correctionField = syncCorrectionField + FollowUoCorrectionField;
                    dState = DelayState::DoDelayReq;
                    timingFlag = true;
                }
            }
        };

        static void handleDelayResp(const RxResult &rxHandle)
        {
            if (dState == DelayState::WaitForDelayResp)
            {
                const Delay_Resp &msg = reinterpret_cast<const Delay_Resp &>(rxHandle.message);
                if ((msg.header.sequenceId == currentSequence) && isFromUpstreamPort(msg.header) &&
                (msg.requestingPortIdentity.portNumber == delayReq.header.sourcePortIdentity.portNumber)
                && (msg.requestingPortIdentity.clockIdentity == delayReq.header.sourcePortIdentity.clockIdentity)
                )
                {
                    // do mpd calculus
                    t4 = msg.receiveTimestamp;
                    TimeInternal delayRespCorrectionField(0,0,msg.header.correctionField);
                    mpd = ((t2 - t3) + (t4 - t1) - syncCorrectionField - FollowUoCorrectionField - delayRespCorrectionField) / 2;
                    dState = DelayState::WaitForSync;
                }
            }
        };
        static void issueDelayReq(){
            delayReq.header.sequenceId = currentSequence;
            TxResult res = netif::transmitEventMessage(&delayReq, sizeof(Delay_Req));
            t3 = res.timestamp;
            dState = DelayState::WaitForDelayResp;
        };

    public:
        static void start(const DefaultDS &parent, const PortDS &port)
        {
            // w store our PortIdentity in the delayReq message buffer.
            delayReq.header.domainNumber = parent.domainNumber;
            delayReq.header.logMessageInterval = port.logMinDelayReqInterval;
            delayReq.header.sourcePortIdentity.clockIdentity = port.portIdentity.clockIdentity;
            delayReq.header.sourcePortIdentity.portNumber = port.portIdentity.portNumber;
            dState = DelayState::WaitForSync;
        };
        static void run()
        {
            PTPMessageType rxType = netif::hasPTPMessage();

            switch (rxType)
            {
            case PTPMessageType::Sync:
            {
                RxResult rxhandle = netif::getPTPMessage();
                handleSync(rxhandle);
            }
            break;
            case PTPMessageType::Follow_Up:
            {
                RxResult rxhandle = netif::getPTPMessage();
                handleFollowUp(rxhandle);
            }
            break;
            case PTPMessageType::Delay_Resp:
            {
                RxResult rxhandle = netif::getPTPMessage();
                handleDelayResp(rxhandle);
            }
            break;
            default:
                isGeneral = true;
                break;
            }
            if(dState == DelayState::DoDelayReq){
                issueDelayReq();
            }
        };
        static void stop();
        static void setUpstreamPortIdentity(DerivedTypes<HostEndian>::PortIdentity id){
            upstreamPort.clockIdentity = id.clockIdentity;
            upstreamPort.portNumber = id.portNumber;
        }
        static bool hasTimingMsg()
        {
            return timingFlag;
        };
        static PTPMessageType hasNontimingMsg()
        {
            if (isGeneral)
            {
                return netif::hasPTPMessage();
            }
            else
            {
                return PTPMessageType::NoMessage;
            }
        };
        static const MDSyncReceive& getTimingMsg(){
                timingFlag = false;
                return timingMsg;

        };

        static RxResult getNontimingMsg()
        {
            return netif::getPTPMessage();
        };
        // static const PTPMessageBuffer getNontiming();
        // static void sendTimingMsg(const MDSyncSend &msg);
    };

} // namespace ptpe
#include "default_md_adapter_impl.hpp"
#endif