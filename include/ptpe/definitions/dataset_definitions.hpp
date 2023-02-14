/**
 * @file dataset_definitions.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @brief The PTP Dataset Definitions will live here (xxxDS)
 * @version 0.1
 * @date 2022-11-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PTPE_DATASET_DEFINITIONS_HPP
#define PTPE_DATASET_DEFINITIONS_HPP
#include <cstring>
#include <ptpe/interface/platform.hpp>
#include <ptpe/definitions/enum_definitions.hpp>
#include <ptpe/definitions/type_definitions.hpp>
namespace ptpe
{
    /**
     */
    struct DefaultDS
    {
        DerivedTypes<HostEndian>::ClockIdentity ClockIdentity;
        HostEndian::UInteger16 numberPorts;
        // ClockQuality clockQuality;
        HostEndian::UInteger8 priority1;
        HostEndian::UInteger8 priority2;
        HostEndian::UInteger8 domainNumber;
        bool slaveOnly;
        uint16_t sdoId; // 12 bit integer in specification
    };

    struct CurrentDS
    {
        HostEndian::UInteger16 stepsRemoved;
        DerivedTypes<HostEndian>::TimeInterval offsetFromMaster;
        DerivedTypes<HostEndian>::TimeInterval meanDelay;
    };

    struct ParentDS
    {
        DerivedTypes<HostEndian>::PortIdentity parentPortIdentity;
        bool parentStats;
        DerivedTypes<HostEndian>::ClockIdentity grandmasterIdentity;
        DerivedTypes<HostEndian>::ClockQuality grandmasterClockQuality;
        HostEndian::UInteger8 grandmasterPriority1;
        HostEndian::UInteger8 grandmasterPriority2;

         template <class T>
        ParentDS &operator=(const T &rhs)
        {
            grandmasterIdentity = rhs.grandmasterIdentity;
            grandmasterPriority1 = rhs.grandmasterPriority1;
            grandmasterClockQuality.clockAccuracy = rhs.grandmasterClockQuality.clockAccuracy;
            grandmasterClockQuality.clockClass = rhs.grandmasterClockQuality.clockClass;
            grandmasterClockQuality.offsetScaledLogVariance = rhs.grandmasterClockQuality.offsetScaledLogVariance;
            grandmasterPriority2 = rhs.grandmasterPriority2;
            parentPortIdentity.clockIdentity= rhs.sourcePortIdentity.clockIdentity;
            parentPortIdentity.portNumber = rhs.sourcePortIdentity.portNumber;
            return *this;
        };
    };

    struct TimePropertiesDS
    {
    };

    struct PortDS
    {
        DerivedTypes<HostEndian>::PortIdentity portIdentity;
        PortState portState;
        HostEndian::Integer8 logMinDelayReqInterval;
        DerivedTypes<HostEndian>::TimeInterval meanLinkDelay;
        HostEndian::Integer8 logAnnounceInterval;
        HostEndian::UInteger8 announceReceiptTimeout;
        HostEndian::Integer8 logSyncInterval;
        DelayMechanism delayMechanism;
    };

    struct Ebest
    {
        DerivedTypes<HostEndian>::ClockIdentity grandmasterIdentity;
        HostEndian::UInteger8 grandmasterPriority1;
        DerivedTypes<HostEndian>::ClockQuality grandmasterClockQuality;
        NetworkEndian::UInteger8 grandmasterPriority2;
        NetworkEndian::UInteger8 timeSource;
        DerivedTypes<HostEndian>::PortIdentity sourcePortIdentity;
        NetworkEndian::UInteger16 stepsRemoved;
        HostEndian::Integer16 currentUtcOffset;

        template <class T>
        Ebest &operator=(const T &rhs)
        {
            grandmasterIdentity = rhs.grandmasterIdentity;
            grandmasterPriority1 = rhs.grandmasterPriority1;
            grandmasterClockQuality.clockAccuracy = rhs.grandmasterClockQuality.clockAccuracy;
            grandmasterClockQuality.clockClass = rhs.grandmasterClockQuality.clockClass;
            grandmasterClockQuality.offsetScaledLogVariance = rhs.grandmasterClockQuality.offsetScaledLogVariance;
            grandmasterPriority2 = rhs.grandmasterPriority2;
            timeSource = rhs.timeSource;
            sourcePortIdentity.clockIdentity = rhs.header.sourcePortIdentity.clockIdentity;
            sourcePortIdentity.portNumber = rhs.header.sourcePortIdentity.portNumber;
            stepsRemoved = rhs.stepsRemoved;
            currentUtcOffset = rhs.currentUtcOffset;
            return *this;
        };

        template <class T>
        bool operator==(const T &rhs) const
        {
            return grandmasterIdentity == rhs.grandmasterIdentity;
        };

        template <class T>
        bool operator>(const T &rhs) const
        {
            bool rv = grandmasterPriority1 > rhs.grandmasterPriority1;
            if (rv)
                return true;
            rv = grandmasterClockQuality.clockClass > rhs.grandmasterClockQuality.clockClass;
            if (rv)
                return true;
            rv = grandmasterClockQuality.clockAccuracy > rhs.grandmasterClockQuality.clockAccuracy;
            if (rv)
                return true;
            rv = grandmasterClockQuality.offsetScaledLogVariance > rhs.grandmasterClockQuality.offsetScaledLogVariance;
            if (rv)
                return true;
            rv = grandmasterPriority2 > rhs.grandmasterPriority2;
            if (rv)
                return true;
            if (memcmp(grandmasterIdentity.value, rhs.grandmasterIdentity.value, 8) > 0)
            {
                return true; // when memcmp returns >0 lhs was bigger then rhs
            }
            else
            {
                return false;
            }
        };
        template <class T>
        bool operator<(const T &rhs) const
        {
            // this is not technically fully correct but it will do for now.
            return !(((*this)==rhs) || ((*this)>rhs));
        };
    };
};
#endif