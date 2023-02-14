#include <ptpe/components/time.hpp>
#include <cmath>
using namespace ptpe;


TimeInternal::TimeInternal(uint64_t seconds, uint32_t nanoseconds, int64_t correctionField)
{
    this->seconds = seconds;
    if (correctionField != 0 || nanoseconds != 0)
    {
        if (nanoseconds != 0)
        {
            correctionField += (65536ll * nanoseconds);
        }
        int additionalSeconds = correctionField / subsecs_per_sec;
        this->seconds += additionalSeconds;
        correctionField = correctionField - additionalSeconds * subsecs_per_sec;
        this->subseconds = correctionField;
    }
    else
    {
        this->subseconds = 0;
    }
};

TimeInternal
TimeInternal::operator+(const TimeInternal &rhs) const
{
    long long susSum = subseconds + rhs.subseconds;
    long long secSum = this->seconds + rhs.seconds;
    if (susSum >= subsecs_per_sec)
    {
        secSum += 1;
        susSum -= subsecs_per_sec;
    }
    else if (-susSum >= subsecs_per_sec)
    {
        secSum -= 1;
        susSum += subsecs_per_sec;
    }
    if (secSum > 0 && susSum < 0)
    {
        secSum -= 1;
        susSum += subsecs_per_sec;
    }
    else if (secSum < 0 && susSum > 0)
    {
        secSum += 1;
        susSum -= subsecs_per_sec;
    }

    return TimeInternal(secSum, susSum);
};

TimeInternal &
TimeInternal::operator=(const TimeInternal &rhs)
{
    this->seconds = rhs.seconds;
    this->subseconds = rhs.subseconds;
    return *this;
};


TimeInternal &
TimeInternal::operator=(const DerivedTypes<NetworkEndian>::Timestamp &rhs)
{

    return operator=(TimeInternal(uint48_t(rhs.secondsField), rhs.nanosecondsField, 0));
};

TimeInternal
TimeInternal::operator-(const TimeInternal &rhs) const
{

    long long susDelta = (subseconds - rhs.subseconds);
    long long secDelta = seconds - rhs.seconds;
    if (susDelta >= subsecs_per_sec)
    {
        susDelta -= subsecs_per_sec;
        secDelta += 1;
    }
    else if (-susDelta >= subsecs_per_sec)
    {
        susDelta += subsecs_per_sec;
        secDelta -= 1;
    };

    if (susDelta < 0 && secDelta > 0)
    {
        secDelta -= 1;
        susDelta += subsecs_per_sec;
    }
    else if (susDelta > 0 && secDelta < 0)
    {
        secDelta += 1;
        susDelta -= subsecs_per_sec;
    }
    return TimeInternal(secDelta, susDelta);
};

TimeInternal
TimeInternal::operator/(const int rhs) const
{
    long long newSec = seconds / rhs;
    int rest = seconds - newSec * rhs;
    long long newSub = (subseconds + rest * subsecs_per_sec) / rhs;
    if (newSub >= subsecs_per_sec)
    {
        newSec += 1;
        newSub -= subsecs_per_sec;
    }
    else if (-newSub >= subsecs_per_sec)
    {
        newSec -= 1;
        newSub += subsecs_per_sec;
    }
    return TimeInternal(newSec, newSub);
};

TimeInternal::operator DerivedTypes<NetworkEndian>::Timestamp() const{
    return DerivedTypes<NetworkEndian>::Timestamp{
        .secondsField = uint48_t(abs(seconds)),
        .nanosecondsField = (subseconds * 1'000'000'000)/subsecs_per_sec
    };
}
