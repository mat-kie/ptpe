/**
 * @file platform_interface.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @brief Concepts for the Interface are defined here.
 * @version 0.1
 * @date 2022-11-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PTPE_PLATFORM_INTERFACE_HPP
#define PTPE_PLATFORM_INTERFACE_HPP
#include <concepts>
#include <ptpe/interface/platform.hpp>
#include <ptpe/components/time.hpp>
#include <ptpe/definitions/enum_definitions.hpp>
#include <ptpe/definitions/message_definitions.hpp>
#include <ptpe/definitions/dataset_definitions.hpp>

namespace ptpe
{

    template<typename T>
    concept Runnable = requires{
        T::stop();
        T::run();
    };

    struct TxResult
    {
        bool successful;
        const TimeInternal timestamp;
        TxResult(bool success, const TimeInternal& ts):successful(success), timestamp(ts){};
        TxResult(const TxResult& rhs):successful(rhs.successful), timestamp(rhs.timestamp){};
    };

    struct RxResult
    {
        const TimeInternal timestamp;
        const PTPMessageBuffer &message;
        RxResult(const TimeInternal& ts, const PTPMessageBuffer& buf):timestamp(ts), message(buf){};
        RxResult(const RxResult& rhs):timestamp(rhs.timestamp), message(rhs.message){};
    };
    template <typename T>
    concept Transport = requires()
    {
        std::is_same_v<decltype(T::transportType), PTPTransportType>;
        {T::transmitEventMessage(nullptr, uint32_t{})} -> std::same_as<TxResult>;
        {T::transmitGeneralMessage(nullptr, uint32_t{})} -> std::same_as<TxResult>;
        {T::hasPTPMessage()} -> std::same_as<PTPMessageType>;
        {T::getPTPMessage()} -> std::same_as<RxResult>;
    };

    template<typename T>
    concept MDAdapterConcept = requires{
        Runnable<T>;
        T::setUpstreamPortIdentity(DerivedTypes<HostEndian>::PortIdentity{});
        { T::hasTimingMsg() } -> std::same_as<bool>;
        { T::hasNontimingMsg() } -> std::same_as<PTPMessageType>;
        { T::getNontimingMsg() } -> std::same_as<RxResult>;

    };

    template<typename T>
    concept PortConcept = requires{
        MDAdapterConcept<T>;
		T::recommendState(PortState::FAULTY);
		{T::StateDecisionEvent()}->std::same_as<const Ebest&>; // determine the best master clock connected to the port and return  it
    };

    template <typename T>
    concept ClockServo = requires
    {
        T::initialize(TimeInternal());
        T::updateOffsetFromMaster(
            TimeInternal(),     ///< OffsetFromMaster Time Value
            uint32_t{});        ///< log2MessageInterval for Sync message in seconds
        {T::isStable()}->std::same_as<bool>;    ///< return true if clock is "stable"
    };


};
#endif