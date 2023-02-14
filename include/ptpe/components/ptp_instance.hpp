
/**
 * @file ptp_instance.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @version 0.1
 * @date 2022-11-06
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PTPE_PTP_INSTANCE_HPP
#define PTPE_PTP_INSTANCE_HPP
#include <ptpe/interface/platform.hpp>
#include <ptpe/components/time.hpp>

#include <ptpe/definitions/dataset_definitions.hpp>
#include <ptpe/interface/concepts.hpp>
#include <ptpe/definitions/message_definitions.hpp>
#include <ptpe/components/ptp_port.hpp>
/** NOTES:
 * The PTPinstance provides the high level protocol state machine.
 * it takes a Transport to get the PTP messages from a port and
 * and forwards MDSyncRecieve messages to the ClockServo
 * The ClockServo is responsible of disciplining the platform's clock.
 *
 */
namespace ptpe
{

	template <ClockServo clock, class ...portList>
	requires (PortConcept<portList>, ...)
	class PTPInstance
	{


		static void updateOffsetFromMaster(const MDSyncReceive& td)
		{
			TimeInternal ofm = td.upstreamTxTime - td.originOrPreciseOriginTimestamp - td.correctionField;
			clock::updateOffsetFromMaster(ofm, td.logMessageInterval);
		};
		bool BMCAlhsGreaterOrEqualRhs(const ParentDS& lhs, const ParentDS& rhs)const;

		template<PortConcept Pcomp, PortConcept ...others>
		requires (sizeof...(others)>0)
		static void BMCA();

		template<PortConcept Pcomp, PortConcept ...others>
		requires (sizeof...(others)==0)
		static void BMCA();
	public:
		enum class InstanceState{
			INITIALIZE,
			PRE_OP,
			ORDINARY,
			BOUNDARY,
			FAULTY
		};
		//typedef void(StateCallback *)(uint8_t, PortState); //< port number and new state of port

		static DefaultDS defaultDS;
		static CurrentDS currentDS;
		static ParentDS parentDS;

		static InstanceState getState(){return state;};
		static bool run();
		//static void registerStateChangeCallback(StateCallback cb);
		static void clearStateChangeCallback();

		/**handle the ports
		 * if InstanceState INITIALIZE:
		 * call initialize on port, go on to preop
		 * if instance State is preop
		 * query if all ports have received announce messages, or the instance AnnounceRecto is exp
		 * 		call stateDecision event on each port, store Ebest from each port
		 * perform bmca on the per port Ebests
		 * dependent on the result and the number of ports on this instance
		 * goto states ORDINARY or BOUNDARY and call the needed recommendedState on each port
		 *
		 * if in state ORDINARY:
		 * query the PortState, if port state has changed, call statechangecallback.
		 * query for MDSyncReceive, if present, get it and do OffsetFromMasterUpdate with it.
		 * query normal PTP messages and do stuff depending on them. (to be implemented later)
		*/
		template<PortConcept port>
		static void HandlePort();
private:
static InstanceState state;

	};

};

#include "ptp_instance_impl.hpp"

#endif