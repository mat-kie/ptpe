#ifndef PTPE_TEST_INTERFACE_HPP
#define PTPE_TEST_INTERFACE_HPP
#include <ptpe/interface/platform.hpp>
#include <ptpe/interface/concepts.hpp>
#include <ptpe/components/time.hpp>
#include <stdexcept>
#include <memory.h>

class TestInterface
{

public:
    static ptpe::PTPMessageBuffer txEvtOutBuf;
    static ptpe::PTPMessageBuffer txGenOutBuf;
    static ptpe::PTPMessageBuffer rxInBuf;
    static size_t txEvtOutCount;
    static size_t txGenOutCount;
    static bool hasRx, TisStable;
    static size_t rxSize;
    static ptpe::TimeInternal ofm, mpd, eventTx, eventRx;
    static int log2SyncInterval, log2DelayRespInterval;
    static constexpr ptpe::PTPTransportType transportType = ptpe::PTPTransportType::IEEE802_3;
    static ptpe::TxResult transmitEventMessage(void *data, size_t size)
    {
        if (size > sizeof(txEvtOutBuf))
        {
            throw 0;
        }
        else
        {
            memcpy(txEvtOutBuf, data, size);
            return ptpe::TxResult(true,eventTx);
            txEvtOutCount++;
        };
    };
    static ptpe::TxResult transmitGeneralMessage(void *data, size_t size)
    {
        if (size > sizeof(txGenOutBuf))
        {
            throw 0;
        }
        else
        {
            memcpy(txGenOutBuf, data, size);
            return ptpe::TxResult(true, ptpe::TimeInternal());
            txGenOutCount++;
        };
    };
    static ptpe::PTPMessageType hasPTPMessage()
    {
        if(hasRx){
            return ptpe::PTPMessageType(rxInBuf[0]);
        }else{
            return ptpe::PTPMessageType::NoMessage;
        }
    };
    static ptpe::RxResult getPTPMessage()
    {
        if (!hasRx)
        {
            throw 1;
        };
        hasRx = false;
        return ptpe::RxResult(eventRx,rxInBuf);
    };

    static void initialize(const ptpe::TimeInternal &initTime)
    {
        memset(txEvtOutBuf, 0, sizeof(ptpe::PTPMessageBuffer));
        memset(txGenOutBuf, 0, sizeof(ptpe::PTPMessageBuffer));

        memset(rxInBuf, 0, sizeof(ptpe::PTPMessageBuffer));

        txEvtOutCount = 0;
        txGenOutCount = 0;
        hasRx = false, TisStable = false;
        rxSize = 0;
        ofm = ptpe::TimeInternal();
        mpd = ptpe::TimeInternal();
        eventTx = ptpe::TimeInternal();

        eventRx = ptpe::TimeInternal();

    };
    static void updateOffsetFromMaster(const ptpe::TimeInternal &ofm, ///< OffsetFromMaster Time Value
                                       int log2MessageInterval)       ///< log2MessageInterval for Sync message in seconds
    {
        TestInterface::ofm = ofm;
        log2SyncInterval = log2MessageInterval;
    };
    static void updateMeanPathDelay(
        const ptpe::TimeInternal &mpd, ///< OffsetFromMaster Time Value
        int log2MessageInterval)       ///< log2MessageInterval for DelayReq / Resp message in seconds
    {
        TestInterface::mpd = mpd;
        log2DelayRespInterval = log2MessageInterval;
    };

    static bool isStable() { return TisStable;};

static void simulateRx(const void* data, size_t size){
    memcpy(TestInterface::rxInBuf, data, size);
    TestInterface::rxSize = size;
    TestInterface::hasRx = true;

}

};
ptpe::PTPMessageBuffer TestInterface::txEvtOutBuf{};
ptpe::PTPMessageBuffer TestInterface::txGenOutBuf{};
ptpe::PTPMessageBuffer TestInterface::rxInBuf{};


size_t TestInterface::txEvtOutCount = 0;
size_t TestInterface::txGenOutCount = 0;
size_t TestInterface::rxSize = 0;

bool TestInterface::hasRx=false;

bool TestInterface::TisStable = false;
int TestInterface::log2SyncInterval = 0;
int TestInterface::log2DelayRespInterval = 0;
ptpe::TimeInternal ofm;
ptpe::TimeInternal TestInterface::mpd{};
ptpe::TimeInternal TestInterface::ofm{};
ptpe::TimeInternal TestInterface::eventTx{};

ptpe::TimeInternal TestInterface::eventRx{};



#endif