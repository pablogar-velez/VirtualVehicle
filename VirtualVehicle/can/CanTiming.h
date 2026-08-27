#pragma once

#include <cstdint>

#include "CanFrame.h"

class CanTiming
{
public:
    static double calculateTransmissionTimeMs(
        const CanFrame& frame,
        std::uint32_t bitrate
    );

private:
    static std::uint32_t estimateFrameBits(
        const CanFrame& frame
    );
};