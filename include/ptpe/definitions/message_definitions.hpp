/**
 * @file message_definitions.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @brief Currently somewhat cluttered. Supposed to declare the PTP Messages here
 * @version 0.1
 * @date 2022-11-06
 * @copyright Copyright (c) 2022
 */
#ifndef PTPE_MESSAGE_DEFINITIONS_HPP
#define PTPE_MESSAGE_DEFINITIONS_HPP
#include <concepts>
#include <ptpe/interface/platform.hpp>
#include <ptpe/definitions/type_definitions.hpp>
#include <ptpe/helpers/fixed_endian_typed.hpp>
#include <ptpe/definitions/enum_definitions.hpp>
#include <ptpe/components/time.hpp>
namespace ptpe
{
	constexpr unsigned int MAX_PTP_MESSAGE_SIZE = 64; // TODO To be determined
	typedef uint8_t PTPMessageBuffer[MAX_PTP_MESSAGE_SIZE];
	/**
	 * @brief PTP Message types
	 *
	 */
	template <typename T>
	concept PTPPayload = requires {
							 std::is_same_v<decltype(T::msgType), PTPMessageType>;
						 };

	struct EthernetHeader
	{
		uint8_t destination[6] = {0x01, 0x1B, 0x19, 0x00, 0x00, 0x00};
		uint8_t source[6] = {};
		uint8_t etherType[2] = {0x88, 0xF7};
	} PACKED_STRUCT;

	struct PTPHeader
	{
		NetworkEndian::UInteger8 messageType{};
		NetworkEndian::UInteger8 versionPTP{2};
		NetworkEndian::UInteger16 messageLength{34};
		NetworkEndian::UInteger8 domainNumber{};
		NetworkEndian::UInteger8 minorSdoID{0};
		NetworkEndian::UInteger8 flagField[2]; // set Bit1 for twoStepFlag
		NetworkEndian::Integer64 correctionField{};
		NetworkEndian::UInteger8 messageTypeSpecific[4]{};
		DerivedTypes<NetworkEndian>::PortIdentity sourcePortIdentity{};
		NetworkEndian::UInteger16 sequenceId{};
		NetworkEndian::UInteger8 controlField{};
		NetworkEndian::Integer8 logMessageInterval{};
	} PACKED_STRUCT;

	struct Sync
	{
		// sadly no reflection in c++20 so we need to do it the verbose way
		PTPHeader header{
			.messageType = uint8_t(PTPMessageType::Sync),
			.versionPTP{2},
			.messageLength = sizeof(Sync),
			.domainNumber{},
			.minorSdoID{},
			.flagField{},
			.correctionField{},
			.messageTypeSpecific{},
			.sourcePortIdentity{},
			.sequenceId{},
			.controlField{},
			.logMessageInterval{}};
		DerivedTypes<NetworkEndian>::Timestamp originTimestamp;
	} PACKED_STRUCT;

	struct Delay_Req
	{

		PTPHeader header{
			.messageType = uint8_t(PTPMessageType::Delay_Req),
			.versionPTP{2},
			.messageLength = sizeof(Delay_Req),
			.domainNumber{},
			.minorSdoID{},
			.flagField{},
			.correctionField{},
			.messageTypeSpecific{},
			.sourcePortIdentity{},
			.sequenceId{},
			.controlField{},
			.logMessageInterval{}};
		DerivedTypes<NetworkEndian>::Timestamp originTimestamp;
	} PACKED_STRUCT;

	struct Follow_Up
	{
		PTPHeader header{
			.messageType = uint8_t(PTPMessageType::Follow_Up),
			.versionPTP{2},
			.messageLength = sizeof(Follow_Up),
			.domainNumber{},
			.minorSdoID{},
			.flagField{},
			.correctionField{},
			.messageTypeSpecific{},
			.sourcePortIdentity{},
			.sequenceId{},
			.controlField{},
			.logMessageInterval{}};
		DerivedTypes<NetworkEndian>::Timestamp preciseOriginTimestamp;
	} PACKED_STRUCT;

	struct Delay_Resp
	{
		PTPHeader header{
			.messageType = uint8_t(PTPMessageType::Delay_Resp),
			.versionPTP{2},
			.messageLength = sizeof(Delay_Resp),
			.domainNumber{},
			.minorSdoID{},
			.flagField{},
			.correctionField{},
			.messageTypeSpecific{},
			.sourcePortIdentity{},
			.sequenceId{},
			.controlField{},
			.logMessageInterval{}};
		DerivedTypes<NetworkEndian>::Timestamp receiveTimestamp;
		DerivedTypes<NetworkEndian>::PortIdentity requestingPortIdentity;
	} PACKED_STRUCT;

	struct Announce
	{
		PTPHeader header = {
			.messageType = uint8_t(PTPMessageType::Announce),
			.versionPTP{2},
			.messageLength = sizeof(Announce),
			.domainNumber{},
			.minorSdoID{},
			.flagField{},
			.correctionField{},
			.messageTypeSpecific{},
			.sourcePortIdentity{},
			.sequenceId{},
			.controlField{},
			.logMessageInterval{}};
		DerivedTypes<NetworkEndian>::Timestamp originTimestamp;
		NetworkEndian::Integer16 currentUtcOffset;
		NetworkEndian::UInteger8 grandmasterPriority1;
		DerivedTypes<NetworkEndian>::ClockQuality grandmasterClockQuality;
		NetworkEndian::UInteger8 grandmasterPriority2;
		DerivedTypes<NetworkEndian>::ClockIdentity grandmasterIdentity;
		NetworkEndian::UInteger16 stepsRemoved;
		NetworkEndian::UInteger8 timeSource;

	} PACKED_STRUCT;

	union PTPMessage
	{
		PTPHeader header;
		Sync sync;
		Delay_Req delay_Req;
		Announce announce;
		Delay_Resp delay_Resp;
		Follow_Up follow_Up;
		uint8_t buffer[MAX_PTP_MESSAGE_SIZE];
	} PACKED_STRUCT;

	/**
	 * @brief "A logical access point of a PTP Instance for PTP Communication to the communications network."
	 *  has the PortDS and implements the PortState Statemachine.
	 *
	 * The Special PTP MD Adapter block:
	 * 	- measures mean path delay;
	 * 	- receives time synchronization information from the PTP Port at the other end
	 *    of the attached link if the PTP Port is a PTP Port in the SLAVE state;
	 * 	- sends respective information across the MDMI interface via an MDSyncReceive structure;
	 * 	- receives information sent across the MDMI interface via an MDSyncSend structure;
	 * 	- and sends the respective time synchronization information to the PTP Port at the other end of the attached link.
	 * eqivalence for E2E:
	 * 			let the classical e2e times be t1, t2, t3, t4
	 * 			with meanPathDelay mpd, the correctionField time cf
	 * 			classical E2E eqn.: offsetFromMaster =  t2 - t1 -cf -
	 */
	struct MDSyncReceive
	{

		HostEndian::UInteger8 domainNumber;
		HostEndian::UInteger8 sdoId;
		DerivedTypes<HostEndian>::PortIdentity sourcePortIdentity;

		/**
		 * 	A value whose data type is Timestamp (see 5.3.3) such that, when
		 * 	combined with the correctionField (see 11.5.3.1.5), the result is the value of the Local PTP Clock of the
		 * 	upstream PTP Instance at an arbitrarily selected time X. The time X represents the same instant of time in
		 * 	11.5.3.1.4, 11.5.3.1.5, and 11.5.3.1.7, and it is selected by the PTP MD Adapter block of the sending port of
		 * 	the upstream PTP Instance,
		 *
		 * my understanding: this is t1 on GM-Time in whole nanoseconds eg. the <syncEgressTimestamp> from Sync(1 step) or the follow_up (2 step) message
		 */
		// DerivedTypes<HostEndian>::Timestamp originOrPreciseOriginTimestamp;
		TimeInternal originOrPreciseOriginTimestamp;

		/**
		 * A value whose data type is Integer64 and whose units are 2–16 ns such that, when
		 * combined with the originOrPreciseOriginTimestamp (see 11.5.3.1.4), the result is the value of the Local
		 * PTP Clock of the upstream PTP Instance at the arbitrarily selected time X
		 *
		 * my understanding: this is the coorection field of the sync message <correctedSyncCorrectionField>
		 */
		// HostEndian::Integer64 correctionField;
		TimeInternal correctionField;
		HostEndian::Integer8 logMessageInterval;

		/**
		 * 	The upstreamTxTime is equal to the value of the Timestamping Clock of this PTP instance at the arbitrarily
		 * 	selected time X
		 * 	t2-mpd corrected for assymetry.
		 */
		// DerivedTypes<HostEndian>::Timestamp
		TimeInternal upstreamTxTime;
		HostEndian::Integer64 cumulativeRateRatio;
	};
};

#endif