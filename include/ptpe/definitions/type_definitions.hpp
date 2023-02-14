#ifndef PTPE_TYPE_DEFINITIONS
#define PTPE_TYPE_DEFINITIONS
#include <ptpe/helpers/fixed_endian_typed.hpp>
#include <cstring>
namespace ptpe
{

    struct uint48_t
    {
    private:
        uint64_t x : 48;

    public:
        uint48_t() : x(0){};
        uint48_t(const uint64_t &rhs) : x(rhs){};
        operator uint64_t() const { return x; };

    } PACKED_STRUCT;

    struct HostEndian
    {
        typedef uint8_t UInteger8;
        typedef int8_t Integer8;
        typedef uint16_t UInteger16;
        typedef int16_t Integer16;
        typedef uint32_t UInteger32;
        typedef uint48_t UInteger48;
        typedef int32_t Integer32;
        typedef int64_t Integer64;
        typedef unsigned char Octet;
    };
    struct EndianIndifferent{
        typedef uint8_t UInteger8;
        typedef int8_t Integer8;
        typedef unsigned char Octet;

        struct ClockIdentity{
            Octet value[8];

            ClockIdentity& operator=(const ClockIdentity& rhs){
                memcpy(value, rhs.value, sizeof(value));
                return *this;
            };

            bool operator==(const ClockIdentity& rhs)const{
                return (memcmp(value, rhs.value, sizeof(value)) == 0);
            };
        }PACKED_STRUCT;

    };
    struct NetworkEndian
    {
        typedef uint8_t UInteger8;
        typedef int8_t Integer8;
        typedef BigEndian<uint16_t> UInteger16;
        typedef BigEndian<int16_t> Integer16;
        typedef BigEndian<uint32_t> UInteger32;
        typedef BigEndian<uint48_t> UInteger48;
        typedef BigEndian<int32_t> Integer32;
        typedef BigEndian<int64_t> Integer64;
        typedef unsigned char Octet;
    };

    template <class ByteType>
    struct DerivedTypes: public EndianIndifferent
    {


        // typedef uint8_t uint48_t[6];
        // typedef uint64_t uint48_t;
        struct PortIdentity
        {
            ClockIdentity clockIdentity;
            ByteType::UInteger16 portNumber;
        } PACKED_STRUCT;

        struct Timestamp
        {
            // uint48_t seconds;
            ByteType::UInteger48 secondsField;
            ByteType::UInteger32 nanosecondsField;
        } PACKED_STRUCT;

        struct PortAddress
        {
            ByteType::UInteger16 networkProtocol;
            ByteType::UInteger16 addressLength;
            Octet addressField[6]; ///< TODO: only valid for mac transport
        } PACKED_STRUCT;

        struct ClockQuality
        {
            UInteger8 clockClass;
            UInteger8 clockAccuracy;
            ByteType::UInteger16 offsetScaledLogVariance;
        } PACKED_STRUCT;

        typedef ByteType::Integer64 TimeInterval;
        //typedef ByteType::Integer64 RelativeDifference;
    };
};

#endif