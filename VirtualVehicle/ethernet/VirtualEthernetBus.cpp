#include "VirtualEthernetBus.h"

#include <algorithm>
#include <stdexcept>

VirtualEthernetBus::VirtualEthernetBus(
    std::uint64_t linkRateBitsPerSecond)
    : linkRateBitsPerSecond(
        linkRateBitsPerSecond
    )
{
    if (
        this->linkRateBitsPerSecond ==
        0
        )
    {
        throw std::invalid_argument(
            "Ethernet link rate must be greater than zero."
        );
    }
}

// ============================================================
// Node registration
// ============================================================

void VirtualEthernetBus::registerNode(
    const MacAddress& macAddress)
{
    if (
        isNodeRegistered(
            macAddress
        )
        )
    {
        return;
    }

    registeredNodes.push_back(
        macAddress
    );

    receiveQueues.emplace(
        macAddress,
        std::deque<EthernetFrame>{}
    );
}

// ============================================================
// Node lookup
// ============================================================

bool VirtualEthernetBus::isNodeRegistered(
    const MacAddress& macAddress) const
{
    return
        std::find(
            registeredNodes.begin(),
            registeredNodes.end(),
            macAddress
        )
        !=
        registeredNodes.end();
}

std::size_t
VirtualEthernetBus::getRegisteredNodeCount() const
{
    return
        registeredNodes.size();
}

// ============================================================
// Transmission
// ============================================================

bool VirtualEthernetBus::transmit(
    const EthernetFrame& frame,
    double requestTimeMs)
{
    if (
        requestTimeMs <
        0.0
        )
    {
        throw std::invalid_argument(
            "Ethernet request time cannot be negative."
        );
    }

    const auto queueIterator =
        receiveQueues.find(
            frame.destinationMac
        );

    if (
        queueIterator ==
        receiveQueues.end()
        )
    {
        return false;
    }

    const double transmissionTimeMs =
        EthernetTiming::calculateTransmissionTimeMs(
            frame,
            linkRateBitsPerSecond
        );

    const double startTimeMs =
        std::max(
            requestTimeMs,
            busyUntilMs
        );

    const double completionTimeMs =
        startTimeMs +
        transmissionTimeMs;

    const double waitingTimeMs =
        startTimeMs -
        requestTimeMs;

    busyUntilMs =
        completionTimeMs;

    queueIterator
        ->second
        .push_back(
            frame
        );

    EthernetTraceEntry traceEntry;
    traceEntry.frame = frame;
    traceEntry.requestTimeMs = requestTimeMs;
    traceEntry.startTimeMs = startTimeMs;
    traceEntry.completionTimeMs = completionTimeMs;
    traceEntry.transmissionTimeMs = transmissionTimeMs;
    traceEntry.waitingTimeMs = waitingTimeMs;

    trace.push_back(
        traceEntry
    );

    statistics.recordTransmission(
        frame
    );

    return true;
}

// ============================================================
// Receive state
// ============================================================

bool VirtualEthernetBus::hasFrameFor(
    const MacAddress& macAddress) const
{
    const auto queueIterator =
        receiveQueues.find(
            macAddress
        );

    if (
        queueIterator ==
        receiveQueues.end()
        )
    {
        return false;
    }

    return
        !queueIterator
        ->second
        .empty();
}

// ============================================================
// Receive
// ============================================================

EthernetFrame VirtualEthernetBus::receive(
    const MacAddress& macAddress)
{
    const auto queueIterator =
        receiveQueues.find(
            macAddress
        );

    if (
        queueIterator ==
        receiveQueues.end()
        )
    {
        throw std::runtime_error(
            "Ethernet receive requested for unregistered MAC address."
        );
    }

    if (
        queueIterator
        ->second
        .empty()
        )
    {
        throw std::runtime_error(
            "No Ethernet frame is pending for the requested MAC address."
        );
    }

    EthernetFrame frame =
        queueIterator
        ->second
        .front();

    queueIterator
        ->second
        .pop_front();

    return frame;
}

// ============================================================
// Queue statistics
// ============================================================

std::size_t
VirtualEthernetBus::getPendingFrameCount(
    const MacAddress& macAddress) const
{
    const auto queueIterator =
        receiveQueues.find(
            macAddress
        );

    if (
        queueIterator ==
        receiveQueues.end()
        )
    {
        return 0;
    }

    return
        queueIterator
        ->second
        .size();
}

std::size_t
VirtualEthernetBus::getTotalPendingFrameCount() const
{
    std::size_t total =
        0;

    for (
        const auto& entry :
        receiveQueues
        )
    {
        total +=
            entry
            .second
            .size();
    }

    return total;
}

// ============================================================
// Timing
// ============================================================

std::uint64_t VirtualEthernetBus::getLinkRate() const
{
    return linkRateBitsPerSecond;
}

double VirtualEthernetBus::getBusyUntilMs() const
{
    return busyUntilMs;
}

// ============================================================
// Verification evidence
// ============================================================

const std::vector<EthernetTraceEntry>&
VirtualEthernetBus::getTrace() const
{
    return trace;
}

const EthernetStatistics&
VirtualEthernetBus::getStatistics() const
{
    return statistics;
}

// ============================================================
// Reset
// ============================================================

void VirtualEthernetBus::clear()
{
    registeredNodes.clear();
    receiveQueues.clear();

    busyUntilMs = 0.0;

    trace.clear();
    statistics.reset();
}
