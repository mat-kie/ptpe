#ifndef PTPE_DEFAULT_MDADAPTER_HPP
#error "do not include directly!"
#include "default_md_adapter.hpp" // just for IDE
#endif

namespace ptpe
{
    template <Transport netif>
    DerivedTypes<HostEndian>::PortIdentity DefaultMDAdapter<netif>::upstreamPort{};
    template <Transport netif>
    DefaultMDAdapter<netif>::DelayState DefaultMDAdapter<netif>:: dState;
    template <Transport netif>
    bool DefaultMDAdapter<netif>::isGeneral = false;

        template <Transport netif>
    bool DefaultMDAdapter<netif>::timingFlag = false;

    template <Transport netif>
    Delay_Req DefaultMDAdapter<netif>::delayReq{};

    template <Transport netif>
    MDSyncReceive DefaultMDAdapter<netif>::timingMsg{};
    template <Transport netif>
    const TimeInternal& DefaultMDAdapter<netif>::t1(DefaultMDAdapter<netif>::timingMsg.originOrPreciseOriginTimestamp);
    template <Transport netif>
    TimeInternal DefaultMDAdapter<netif>::t2{};
    template <Transport netif>
    TimeInternal DefaultMDAdapter<netif>::t3{};
    template <Transport netif>
    TimeInternal DefaultMDAdapter<netif>::t4{};
    template <Transport netif>
    TimeInternal DefaultMDAdapter<netif>::syncCorrectionField{};
    template <Transport netif>
    TimeInternal DefaultMDAdapter<netif>::FollowUoCorrectionField{};
    template <Transport netif>
    TimeInternal DefaultMDAdapter<netif>::mpd{};

    template <Transport netif>
    uint16_t DefaultMDAdapter<netif>::currentSequence = 0;



};
