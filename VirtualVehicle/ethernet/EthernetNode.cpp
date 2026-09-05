#include "EthernetNode.h"

EthernetNode::EthernetNode(
    const MacAddress& macAddress)
    : macAddress(
        macAddress
    )
{
}

const MacAddress&
EthernetNode::getMacAddress() const
{
    return macAddress;
}

bool EthernetNode::transmit(
    VirtualEthernetBus& bus,
    const MacAddress& destinationMac,
    std::uint16_t etherType,
    const std::vector<std::uint8_t>& payload,
    double requestTimeMs) const
{
    EthernetFrame frame;
    frame.sourceMac = macAddress;
    frame.destinationMac = destinationMac;
    frame.etherType = etherType;
    frame.payload = payload;

    return bus.transmit(
        frame,
        requestTimeMs
    );
}

void EthernetNode::processReceivedFrames(
    VirtualEthernetBus& bus)
{
    while (
        bus.hasFrameFor(
            macAddress
        )
        )
    {
        receivedFrames.push_back(
            bus.receive(
                macAddress
            )
        );
    }
}

bool EthernetNode::hasReceivedFrame() const
{
    return
        !receivedFrames.empty();
}

const std::vector<EthernetFrame>&
EthernetNode::getReceivedFrames() const
{
    return receivedFrames;
}

void EthernetNode::clearReceivedFrames()
{
    receivedFrames.clear();
}
