#ifndef PTPE_TEST_MESSAGES_HPP
#define PTPE_TEST_MESSAGES_HPP
#include <stdint.h>

namespace test
{
    typedef uint8_t id_t[8];
    template <const id_t& grandMasterId, uint16_t SeqId, int64_t corr, uint32_t tsSec, uint32_t tsNano>
    struct UndirectedMessage
    {
        static constexpr uint8_t lowSeqId = SeqId &(0x00FFu);
        static constexpr uint8_t highSeqId =  (SeqId &(0xFF00u))>>8;
        static constexpr uint8_t correction[] = {
            (corr &(0xFF00000000000000))>>56,
            (corr &(0x00FF000000000000))>>48,
            (corr &(0x0000FF0000000000))>>40,
            (corr &(0x000000FF00000000))>>32,
            (corr &(0x00000000FF000000))>>24,
            (corr &(0x0000000000FF0000))>>16,
            (corr &(0x000000000000FF00))>>8,
            (corr &(0x00000000000000FF))
        };

        static constexpr uint8_t btsSec[] = {
            (tsSec & 0xFF000000)>>24,
            (tsSec & 0x00FF0000)>>16,
            (tsSec & 0x0000FF00)>>8,
            (tsSec & 0x000000FF)
        };

        static constexpr uint8_t btsNsec[] = {
            (tsNano & 0xFF000000)>>24,
            (tsNano & 0x00FF0000)>>16,
            (tsNano & 0x0000FF00)>>8,
            (tsNano & 0x000000FF)
        };

        static constexpr uint8_t Announce[] = {
            0x0b, 0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, grandMasterId[0], grandMasterId[1], grandMasterId[2], grandMasterId[3], grandMasterId[4], grandMasterId[5], grandMasterId[6], grandMasterId[7], 0x00, 0x01, highSeqId, lowSeqId,
            0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x80,
            0xf8, 0xfe, 0xff, 0xff, 0x80, grandMasterId[0], grandMasterId[1], grandMasterId[2], grandMasterId[3], grandMasterId[4], grandMasterId[5], grandMasterId[6], grandMasterId[7], 0x00, 0x00, 0xa0

        };

        static constexpr uint8_t Sync[] = {
            0x00, 0x02, 0x00, 0x2c, 0x00, 0x00, 0x02, 0x00, correction[0], correction[1], correction[2], correction[3], correction[4], correction[5], correction[6], correction[7],
            0x00, 0x00, 0x00, 0x00, grandMasterId[0], grandMasterId[1], grandMasterId[2], grandMasterId[3], grandMasterId[4], grandMasterId[5], grandMasterId[6], grandMasterId[7], 0x00, 0x01, highSeqId, lowSeqId,
            0x00, 0x00, 0x00, 0x00, btsSec[0], btsSec[1], btsSec[2], btsSec[3], btsNsec[0], btsNsec[1], btsNsec[2], btsNsec[3]

        };

        static constexpr uint8_t FollowUp[] = {
            0x08, 0x02, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, correction[0], correction[1], correction[2], correction[3], correction[4], correction[5], correction[6], correction[7],
            0x00, 0x00, 0x00, 0x00, grandMasterId[0], grandMasterId[1], grandMasterId[2], grandMasterId[3], grandMasterId[4], grandMasterId[5], grandMasterId[6], grandMasterId[7], 0x00, 0x01, highSeqId, lowSeqId,
            0x02, 0x00, 0x00, 0x00,btsSec[0], btsSec[1], btsSec[2], btsSec[3], btsNsec[0], btsNsec[1], btsNsec[2], btsNsec[3]

        };
    };

    template <const id_t& grandMasterId, uint16_t SeqId, const id_t& rpi, int64_t corr, uint32_t tsSec, uint32_t tsNano>
    struct DirectedMessage
    {
        static constexpr uint8_t lowSeqId = SeqId &(0x00FFu);
        static constexpr uint8_t highSeqId =  (SeqId &(0xFF00u))>>8;
                static constexpr uint8_t correction[] = {
            (corr &(0xFF00000000000000))>>56,
            (corr &(0x00FF000000000000))>>48,
            (corr &(0x0000FF0000000000))>>40,
            (corr &(0x000000FF00000000))>>32,
            (corr &(0x00000000FF000000))>>24,
            (corr &(0x0000000000FF0000))>>16,
            (corr &(0x000000000000FF00))>>8,
            (corr &(0x00000000000000FF))
        };

        static constexpr uint8_t btsSec[] = {
            (tsSec & 0xFF000000)>>24,
            (tsSec & 0x00FF0000)>>16,
            (tsSec & 0x0000FF00)>>8,
            (tsSec & 0x000000FF)
        };

        static constexpr uint8_t btsNsec[] = {
            (tsNano & 0xFF000000)>>24,
            (tsNano & 0x00FF0000)>>16,
            (tsNano & 0x0000FF00)>>8,
            (tsNano & 0x000000FF)
        };

        static constexpr uint8_t Delay_Resp[] = {
            0x09, 0x02, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, correction[0], correction[1], correction[2], correction[3], correction[4], correction[5], correction[6], correction[7],
            0x00, 0x00, 0x00, 0x00, grandMasterId[0], grandMasterId[1], grandMasterId[2], grandMasterId[3], grandMasterId[4], grandMasterId[5], grandMasterId[6], grandMasterId[7], 0x00, 0x01, highSeqId, lowSeqId,
            0x03, 0x00, 0x00, 0x00,btsSec[0], btsSec[1], btsSec[2], btsSec[3], btsNsec[0], btsNsec[1], btsNsec[2], btsNsec[3], rpi[0], rpi[1], rpi[2], rpi[3],
            rpi[4], rpi[5], rpi[6], rpi[7], 0x00, 0x00

        };
    };

};

#endif