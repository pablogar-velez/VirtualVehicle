#include "CanTiming.h"

std::uint32_t CanTiming::estimateFrameBits(
    const CanFrame& frame)
{
    // Simplified Classical CAN frame estimation.
    //
    // SOF                  = 1
    // Arbitration          = 12
    // Control              = 6
    // Data                 = DLC * 8
    // CRC                  = 16
    // ACK                  = 2
    // EOF                  = 7
    // Intermission         = 3
    //
    // Bit stuffing is intentionally not modeled yet.

    const std::uint32_t baseBits =
        1 +
        12 +
        6 +
        (static_cast<std::uint32_t>(frame.dlc) * 8) +
        16 +
        2 +
        7 +
        3;

    return baseBits;
}

double CanTiming::calculateTransmissionTimeMs(
    const CanFrame& frame,
    std::uint32_t bitrate)
{
    const std::uint32_t bits =
        estimateFrameBits(frame);

    const double seconds =
        static_cast<double>(bits) /
        static_cast<double>(bitrate);

    return seconds * 1000.0;
}