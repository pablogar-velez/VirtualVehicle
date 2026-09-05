#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <vector>

#include "EthernetFrame.h"
#include "EthernetStatistics.h"
#include "EthernetTiming.h"
#include "EthernetTraceEntry.h"
#include "MacAddress.h"

class VirtualEthernetBus
{
public:
    explicit VirtualEthernetBus(
        std::uint64_t linkRateBitsPerSecond = EthernetTiming::DefaultLinkRate
    );

    void registerNode(
        const MacAddress& macAddress
    );

    bool isNodeRegistered(
        const MacAddress& macAddress
    ) const;

    std::size_t getRegisteredNodeCount() const;

    bool transmit(
        const EthernetFrame& frame,
        double requestTimeMs = 0.0
    );

    bool hasFrameFor(
        const MacAddress& macAddress
    ) const;

    EthernetFrame receive(
        const MacAddress& macAddress
    );

    std::size_t getPendingFrameCount(
        const MacAddress& macAddress
    ) const;

    std::size_t getTotalPendingFrameCount() const;

    std::uint64_t getLinkRate() const;
    double getBusyUntilMs() const;

    const std::vector<EthernetTraceEntry>&
        getTrace() const;

    const EthernetStatistics&
        getStatistics() const;

    void clear();

private:
    std::vector<MacAddress>
        registeredNodes;

    std::map<
        MacAddress,
        std::deque<EthernetFrame>
    >
        receiveQueues;

    std::uint64_t linkRateBitsPerSecond;
    double busyUntilMs{ 0.0 };

    std::vector<EthernetTraceEntry>
        trace;

    EthernetStatistics statistics;
};
