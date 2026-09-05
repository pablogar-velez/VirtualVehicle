#include "EthernetTiming.h"

#include <stdexcept>

std::size_t EthernetTiming::calculateFrameBytes(
    const EthernetFrame& frame)
{
    return
        HeaderBytes +
        frame.payload.size();
}

std::uint64_t EthernetTiming::calculateFrameBits(
    const EthernetFrame& frame)
{
    return
        static_cast<std::uint64_t>(
            calculateFrameBytes(frame)
            ) *
        8ULL;
}

double EthernetTiming::calculateTransmissionTimeMs(
    const EthernetFrame& frame,
    std::uint64_t linkRateBitsPerSecond)
{
    if (
        linkRateBitsPerSecond ==
        0
        )
    {
        throw std::invalid_argument(
            "Ethernet link rate must be greater than zero."
        );
    }

    const double frameBits =
        static_cast<double>(
            calculateFrameBits(frame)
            );

    const double linkRate =
        static_cast<double>(
            linkRateBitsPerSecond
            );

    return
        (frameBits / linkRate) *
        1000.0;
}
