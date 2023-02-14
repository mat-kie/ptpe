#include <gtest/gtest.h>
#include<ptpe/definitions/message_definitions.hpp>
#include "testMessages.hpp"
#include <cstdlib>
#include <memory.h>

TEST(MessageTest, TestFields){
    static const test::id_t gmId{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    std::srand(1);
    constexpr int corr  = 10;
    constexpr int ts    = 10;
    constexpr int nts   = 10;
    constexpr int seqId = 0xABCD;

    ptpe::Sync tsync = *reinterpret_cast<const ptpe::Sync*>(&test::UndirectedMessage<gmId,seqId, corr, ts, nts>::Sync);
    tsync.header.sourcePortIdentity.clockIdentity = *reinterpret_cast<const ptpe::EndianIndifferent::ClockIdentity*>(gmId);

    tsync.header.sequenceId = seqId;
    tsync.header.correctionField = corr;
    tsync.originTimestamp.secondsField = ptpe::uint48_t(uint64_t(ts));
    tsync.originTimestamp.nanosecondsField = nts;
    tsync.header.sourcePortIdentity.portNumber = 1;
    tsync.header.flagField[0] = 2;
    ASSERT_TRUE(sizeof(ptpe::Sync)==44);
    ASSERT_TRUE(memcmp(&tsync,test::UndirectedMessage<gmId,seqId, corr, ts, nts>::Sync,44 )==0);

};