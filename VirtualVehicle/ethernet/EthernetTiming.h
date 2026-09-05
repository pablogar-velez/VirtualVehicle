#pragma once

#include <cstddef>
#include <cstdint>

#include "EthernetFrame.h"

class EthernetTiming
{
public:
    static constexpr std::uint64_t DefaultLinkRate =
        100000000ULL;

    static constexpr std::size_t HeaderBytes =
        14;

    static std::size_t calculateFrameBytes(
        const EthernetFrame& frame
    );

    static std::uint64_t calculateFrameBits(
        const EthernetFrame& frame
    );

    static double calculateTransmissionTimeMs(
        const EthernetFrame& frame,
        std::uint64_t linkRateBitsPerSecond = DefaultLinkRate
    );
};
