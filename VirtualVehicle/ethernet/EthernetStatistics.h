#pragma once

#include <cstddef>
#include <map>

#include "EthernetFrame.h"
#include "MacAddress.h"

struct EthernetNodeStatistics
{
    std::size_t framesTransmitted{ 0 };
    std::size_t payloadBytesTransmitted{ 0 };
};

class EthernetStatistics
{
public:
    void recordTransmission(
        const EthernetFrame& frame
    );

    std::size_t getFrameCount() const;
    std::size_t getPayloadByteCount() const;

    const EthernetNodeStatistics&
        getNodeStatistics(
            const MacAddress& macAddress
        ) const;

    bool hasNodeStatistics(
        const MacAddress& macAddress
    ) const;

    void reset();

private:
    std::size_t frameCount{ 0 };
    std::size_t payloadByteCount{ 0 };

    std::map<
        MacAddress,
        EthernetNodeStatistics
    >
        nodeStatistics;
};
