#ifndef PTPE_ENUM_DEFINITIONS_HPP
#define PTPE_ENUM_DEFINITIONS_HPP
#include <ptpe/interface/platform.hpp>
namespace ptpe{

	enum class PTPTransportType : uint8_t
	{
		IPv4,
		IPv6,
		IEEE802_3
	};

	enum class PTPMessageType : uint8_t
	{
		Sync = 0,
		Delay_Req = 1,
		Pdelay_Req = 2,
		Pdelay_Resp = 3,
		Follow_Up = 8,
		Delay_Resp = 9,
		Pdelay_Resp_Follow_Up = 0xA,
		Announce = 0xB,
		Signaling = 0xC,
		Management = 0xD,
		NoMessage=0xFF
	};
	enum class PortState
	{
		INITIALIZING,
		FAULTY,
		LISTENING,
		UNCALIBRATED,
		SLAVE,
		MASTER
	};
	enum class DelayMechanism : uint8_t
	{
		E2E = 0x01,
		P2P = 0x02,
		NO_MECHANISM = 0xFE,
		COMMON_P2P = 0x03,
		SPECIAL = 0x04
	};
};
#endif