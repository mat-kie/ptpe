#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <gtest/gtest.h>
#include <ptpe/components/time.hpp>


TEST(TimeTest, TestTimeMath){
    using namespace ptpe;
    TimeInternal oneSecCor(0, 0, 65536'000'000'000ull);
    TimeInternal oneSecNano(0, 1'000'000'000, 0);
    TimeInternal oneSec(1, 0, 0);
    ASSERT_TRUE(
        oneSec.seconds == 1
    );
    ASSERT_TRUE(
        oneSec.subseconds == 0
    );
    ASSERT_TRUE(
        oneSecNano.seconds == 1
    );
    ASSERT_TRUE(
        oneSecNano.subseconds == 0
    );
    ASSERT_TRUE(
        oneSecCor.seconds == 1
    );
    ASSERT_TRUE(
        oneSecCor.subseconds == 0
    );
};

TEST(TimeTest, TestTimeMath2){
    using namespace ptpe;
    TimeInternal halfSecCorr(0, 0, 32768'000'000'000ull);
    TimeInternal oneSecNano(0, 1'000'000'000, 0);
    TimeInternal oneSec(1, 0, 0);
    ASSERT_TRUE(
        (halfSecCorr+halfSecCorr).seconds == 1
    );
    ASSERT_TRUE(
        (halfSecCorr+halfSecCorr).subseconds == 0
    );
    ASSERT_TRUE(
        (oneSec-halfSecCorr).subseconds == halfSecCorr.subseconds
    );
    ASSERT_TRUE(
        (oneSec-halfSecCorr).seconds == 0
    );
    ASSERT_TRUE(
        (oneSec-halfSecCorr-oneSec).subseconds == -halfSecCorr.subseconds
    );
    ASSERT_TRUE(
        (oneSec-halfSecCorr-oneSec-halfSecCorr).seconds == -1
    );
};

TEST(TimeTest, TestTimeMath3){
    using namespace ptpe;
    TimeInternal halfSecCorr(0, 0, 32768'000'000'000ull);
    TimeInternal oneSecNano(0, 1'000'000'000, 0);
    TimeInternal oneSec(1, 0, 0);
    ASSERT_TRUE(
        (oneSec/2).seconds == 0
    );
    ASSERT_TRUE(
        (oneSec/2).subseconds == halfSecCorr.subseconds
    );
};