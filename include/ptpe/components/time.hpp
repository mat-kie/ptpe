/**
 * @file time.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @version 0.1
 * @date 2022-11-06
 * @copyright Copyright (c) 2022
 */
#ifndef PTPE_TIME_HPP
#define PTPE_TIME_HPP
#include <ptpe/interface/platform.hpp>
#include <ptpe/definitions/type_definitions.hpp>

namespace ptpe
{

	class TimeInternal
	{

		static constexpr long long subsecs_per_sec = 65'536'000'000'000; // 2^16 * 1e9
		TimeInternal(int64_t secs, int64_t subsecs) : seconds(secs), subseconds(subsecs){};

	public:
		int64_t seconds;	///< store time in seconds
		int64_t subseconds; ///<  store time in nanoseconds multiplied by 2^16 (<<16)
		int getNanoSeconds()const {return (subseconds*1'000'000'000) / subsecs_per_sec;};
		TimeInternal(uint64_t seconds, uint32_t nanoseconds, int64_t correctionField);
		TimeInternal() : seconds(0), subseconds(0){};
		TimeInternal(const TimeInternal &rhs) : seconds(rhs.seconds), subseconds(rhs.subseconds){};
		TimeInternal(const DerivedTypes<HostEndian>::Timestamp &rhs);
		TimeInternal(const DerivedTypes<NetworkEndian>::Timestamp &rhs);
		TimeInternal(const DerivedTypes<HostEndian>::TimeInterval &rhs);
		TimeInternal(const DerivedTypes<NetworkEndian>::TimeInterval &rhs);
		TimeInternal operator+(const TimeInternal &rhs) const;
		TimeInternal operator-(const TimeInternal &rhs) const;
		TimeInternal operator/(const int rhs) const;
		inline bool operator==(const TimeInternal& rhs) const;
		inline bool operator<(const TimeInternal& rhs) const;
		inline bool operator>(const TimeInternal& rhs) const;
		inline bool operator>=(const TimeInternal& rhs) const;
		inline bool operator<=(const TimeInternal& rhs) const;

		TimeInternal &operator=(const TimeInternal &rhs);
		TimeInternal &operator=(const DerivedTypes<NetworkEndian>::Timestamp &rhs);
		operator DerivedTypes<HostEndian>::Timestamp() const;
		operator DerivedTypes<NetworkEndian>::Timestamp() const;
		operator DerivedTypes<HostEndian>::TimeInterval() const;
		operator DerivedTypes<NetworkEndian>::TimeInterval() const;
	};

};
#endif