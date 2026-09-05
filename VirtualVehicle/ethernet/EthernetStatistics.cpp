#include "EthernetStatistics.h"

namespace
{
    const EthernetNodeStatistics EmptyNodeStatistics{};
}

void EthernetStatistics::recordTransmission(
    const EthernetFrame& frame)
{
    ++frameCount;

    payloadByteCount +=
        frame.payload.size();

    EthernetNodeStatistics& sourceStatistics =
        nodeStatistics[
            frame.sourceMac
        ];

    ++sourceStatistics.framesTransmitted;

    sourceStatistics.payloadBytesTransmitted +=
        frame.payload.size();
}

std::size_t EthernetStatistics::getFrameCount() const
{
    return frameCount;
}

std::size_t EthernetStatistics::getPayloadByteCount() const
{
    return payloadByteCount;
}

const EthernetNodeStatistics&
EthernetStatistics::getNodeStatistics(
    const MacAddress& macAddress) const
{
    const auto iterator =
        nodeStatistics.find(
            macAddress
        );

    if (
        iterator ==
        nodeStatistics.end()
        )
    {
        return EmptyNodeStatistics;
    }

    return iterator->second;
}

bool EthernetStatistics::hasNodeStatistics(
    const MacAddress& macAddress) const
{
    return
        nodeStatistics.find(
            macAddress
        )
        !=
        nodeStatistics.end();
}

void EthernetStatistics::reset()
{
    frameCount = 0;
    payloadByteCount = 0;
    nodeStatistics.clear();
}
