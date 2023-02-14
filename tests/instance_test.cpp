#include <gtest/gtest.h>

#include "TestInterface.hpp"

#include <ptpe/components/ptp_instance.hpp>
#include <ptpe/components/default_md_adapter.hpp>
#include <ptpe/components/ptp_port.hpp>

#include "testMessages.hpp"
using namespace ptpe;

/// TODO: build a proper test suite....


TEST(IntanceTest, AnnounceReceive)
{
    using MyMDAdapter = DefaultMDAdapter<TestInterface>;
    using MyPort = PTPPort<MyMDAdapter>;
    using MyInstance = PTPInstance<TestInterface, MyPort>;
    static constexpr DerivedTypes<HostEndian>::ClockIdentity gmId{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    ptpe::Announce announce1{

        .grandmasterIdentity = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}};
    announce1.header.sourcePortIdentity = DerivedTypes<NetworkEndian>::PortIdentity{
        .clockIdentity = {0, 1, 2, 3, 4, 5, 6, 7},
        .portNumber = 1};

    MyInstance::defaultDS.domainNumber = 0;
    MyInstance::defaultDS.numberPorts = 1;
    MyInstance::defaultDS.slaveOnly = true;
    // initialize instance by calling run the first time
    MyInstance::run();
    ASSERT_TRUE(MyInstance::InstanceState::PRE_OP == MyInstance::getState());
    // put Annouce with GM on testInterface
    TestInterface::simulateRx(&announce1, sizeof(announce1));
    // call run()
    MyInstance::run();
    //  check if we are in Ordinary clock state
    ASSERT_TRUE(MyInstance::InstanceState::ORDINARY == MyInstance::getState());
    // check parentDS port Identity
    ASSERT_TRUE(MyInstance::parentDS.grandmasterIdentity == gmId);
    // check PortState of MyPort
    ASSERT_TRUE(MyPort::getState() == PortState::SLAVE);

    // put sync on rx path
    Sync sync1{};
      sync1.header.sourcePortIdentity = DerivedTypes<NetworkEndian>::PortIdentity{
        .clockIdentity = {0, 1, 2, 3, 4, 5, 6, 7},
        .portNumber = 1};
    sync1.header.sequenceId = 10;
    sync1.header.flagField[0] = 0;
    sync1.header.logMessageInterval = 3;
    sync1.originTimestamp = TimeInternal(10,0,0);
    TestInterface::eventRx = TimeInternal(5,0,0);
    TestInterface::simulateRx(&sync1, sizeof(sync1));
    // call run
    MyInstance::run();
    // check if we got an OffsetFromMaster event ?
    ASSERT_TRUE(TestInterface::log2SyncInterval == 3);
    ASSERT_TRUE(TestInterface::ofm.seconds == -5); // we have t1 = 10s t2 = 5s and a mpd and corr. of 0
    ASSERT_TRUE(TestInterface::ofm.subseconds == 0);
    // check if we have a delay req on tx path with correct sequence id and source port ID
    // put delay resp on  rx path
    // call run
    // check if mean Path delay was updated? possibly not possible
    //put another sync on rx path
    // now we must get a offsetFromMaster event with the correct mean path delay considered
};
