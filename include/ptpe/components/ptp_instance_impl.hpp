#ifndef PTPE_PTP_INSTANCE_HPP
	#error "do not include this file directly!"
	#include "ptp_instance.hpp" // just for IDE
#endif

namespace ptpe
{

	template <ClockServo clock, class... portList>
	DefaultDS PTPInstance<clock, portList...>::defaultDS{};

	template <ClockServo clock, class... portList>
	ParentDS PTPInstance<clock, portList...>::parentDS{};

	template <ClockServo clock, class... portList>
	CurrentDS PTPInstance<clock, portList...>::currentDS{};

	template <ClockServo clock, class... portList>
	PTPInstance<clock, portList...>::InstanceState PTPInstance<clock, portList...>::state{0};

	template <ClockServo clock, class... portList>
	bool
	PTPInstance<clock, portList...>::run()
	{

		if (state != InstanceState::INITIALIZE)
		{
			// advance all port state machines one cycle
			((portList::run()), ...);
		}

		switch (state)
		{
		case InstanceState::INITIALIZE:
		{
			unsigned int num = 0;
			// enumerate ports and initialize them
			((portList::start(num++, defaultDS)), ...);
			defaultDS.numberPorts = num;
			state = InstanceState::PRE_OP;
		}
		break;
		case InstanceState::PRE_OP:

			// wait for the AnnounceReceiptTimeout or the reception of announce messages
			// on all ports
			//
			// do a StateDecisionEvent on all ports, perform BMCA with the per-Port BMCA results
			//
			// put the receiver of the BMC into slave state and the others into Master state
			// if nothing was received on any port goto master state for all ports.
			//
			// change the Instance State
			//
			//
			// for now just goto slave state on the first port, && evaluation stops after fist cause of , false

			//((portList::recommendState(PortState::SLAVE), false) && ...);
			if (((portList::hasAnnounce()) || ...) || false /*here would be the AnnounceTimeout*/)
			{
				// DO BMCA
				BMCA<portList...>();
				state = InstanceState::ORDINARY;
			}
			// we might get management messages before our ports get a StateDecisionEvent
			((HandlePort<portList>()), ...);
			break;
		case InstanceState::FAULTY:
			// do fault handling
			break;
		default:
			/**
			 * now we are operational
			 * call HandlePort on all ports
			 */
			((HandlePort<portList>()), ...);
			break;
		}

		return true;
	};

	template <ClockServo clock, class... portList>
	template <PortConcept port>
	void
	PTPInstance<clock, portList...>::HandlePort()
	{
		if (port::hasTimingMsg())
		{
			// MDSyncReceive handled here
			const MDSyncReceive &msg = port::getTimingMsg();
			updateOffsetFromMaster(msg);
		}
		if (port::hasNontimingMsg())
		{
			/// TODO: implement nontiming handling
		}

	};

	/// TODO: consider rewrite. this might bloat binary size on an ordinary clock
	// vvv Recursive templated implementation of the BMCA vvv
	template <ClockServo clock, class... portList>
	template <PortConcept Pcomp, PortConcept... others>
		requires(sizeof...(others) > 0)
	void PTPInstance<clock, portList...>::BMCA()
	{
		const Ebest &EBestPcomp = Pcomp::StateDecisionEvent();
		bool PcompHasBestMaster = ((EBestPcomp > others::StateDecisionEvent()) && ...);

		if (PcompHasBestMaster)
		{
			// we have the best GM connected to port Pcomp so we want this port to get the time
			// and distribute it to the others
			Pcomp::recommendState(PortState::SLAVE);
			parentDS = EBestPcomp;
			((others::recommendState(PortState::MASTER)), ...);
		}
		else
		{
			// the best GM is connected to one of the other ports so we will distribute the time
			// from another port via Pcomp
			Pcomp::recommendState(PortState::MASTER);
			// move on to the next port
			BMCA<others...>();
		}
	};

	template <ClockServo clock, class... portList>
	template <PortConcept Pcomp, PortConcept... others>
		requires(sizeof...(others) == 0)
	void PTPInstance<clock, portList...>::BMCA()
	{
		// no other ports left to compare so we are the only one left and need to get the time
		parentDS = Pcomp::StateDecisionEvent();
		Pcomp::recommendState(PortState::SLAVE);
	};

};
