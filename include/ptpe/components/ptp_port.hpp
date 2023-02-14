/**
 * @file ptp_port.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @version 0.1
 * @date 2022-11-06
 * @copyright Copyright (c) 2022
 */
#ifndef PTPE_PTP_PORT_HPP
#define PTPE_PTP_PORT_HPP
#include <ptpe/interface/concepts.hpp>
#include <ptpe/definitions/dataset_definitions.hpp>
namespace ptpe
{
    template <MDAdapterConcept MDAdapter>
    class PTPPort
    {
        static PortDS portDS;
        static Ebest bestGM;
        static bool hasAnnounceFlag;

    public:
        static void run()
        {
            MDAdapter::run();

            PTPMessageType rxType = MDAdapter::hasNontimingMsg();
            switch (rxType)
            {
            case PTPMessageType::Announce:
            {
                RxResult rxMsg = MDAdapter::getNontimingMsg();
                const Announce &msg = reinterpret_cast<const Announce &>(rxMsg.message);
                if (!(bestGM == msg))
                {
                    if (bestGM < msg)
                    {
                        bestGM = msg;
                        hasAnnounceFlag = true;
                    };
                }
            }
            break;

            default:
                break;
            };
        };

        static void start(unsigned int portnumber, const DefaultDS& config)
        {
            portDS.portState = PortState::LISTENING;
            portDS.portIdentity.portNumber = portnumber;
            portDS.portIdentity.clockIdentity = config.ClockIdentity;
            MDAdapter::start(config, portDS);
        };

        static void stop(){

        };

        static void recommendState(PortState state)
        {
            // todo: proper state transitions
            if (portDS.portState == PortState::LISTENING || portDS.portState == PortState::UNCALIBRATED)
            {
                if (state == PortState::SLAVE || state == PortState::MASTER || state == PortState::UNCALIBRATED)
                {
                    portDS.portState = state;
                    MDAdapter::setUpstreamPortIdentity(bestGM.sourcePortIdentity);
                }
            }
        };

        static const Ebest &StateDecisionEvent() // determine the best master clock connected to the port and return  it
        {
            return bestGM;
        };

        static bool hasTimingMsg()
        {
            return MDAdapter::hasTimingMsg();
        };

        static bool hasAnnounce()
        {
            if (!hasAnnounceFlag)
            {
                return false;
            }
            else
            {
                hasAnnounceFlag = false;
                return true;
            }
        };

        static bool hasNontimingMsg()
        {
            return MDAdapter::hasNontimingMsg() != PTPMessageType::NoMessage;
        };
        static const MDSyncReceive &getTimingMsg()
        {
            return MDAdapter::getTimingMsg();
        };
        static const PTPMessage &getNontiming()
        {
            return MDAdapter::getNontimingMsg();
        };

        static PortState getState() { return portDS.portState; };
    };
    template <MDAdapterConcept MDAdapter>
    PortDS PTPPort<MDAdapter>::portDS{};
    template <MDAdapterConcept MDAdapter>
    Ebest PTPPort<MDAdapter>::bestGM{};
    template <MDAdapterConcept MDAdapter>
    bool PTPPort<MDAdapter>::hasAnnounceFlag = false;
} // namespace ptpe

#endif