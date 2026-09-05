#pragma once

#include <cstdint>
#include <vector>

#include "EthernetFrame.h"
#include "MacAddress.h"
#include "VirtualEthernetBus.h"

class EthernetNode
{
public:
    explicit EthernetNode(
        const MacAddress& macAddress
    );

    const MacAddress&
        getMacAddress() const;

    bool transmit(
        VirtualEthernetBus& bus,
        const MacAddress& destinationMac,
        std::uint16_t etherType,
        const std::vector<std::uint8_t>& payload,
        double requestTimeMs = 0.0
    ) const;

    void processReceivedFrames(
        VirtualEthernetBus& bus
    );

    bool hasReceivedFrame() const;

    const std::vector<EthernetFrame>&
        getReceivedFrames() const;

    void clearReceivedFrames();

private:
    MacAddress macAddress;

    std::vector<EthernetFrame>
        receivedFrames;
};
