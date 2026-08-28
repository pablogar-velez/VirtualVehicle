#include "VirtualCanBus.h"

#include <iostream>

#include "CanArbitration.h"
#include "CanTiming.h"

// ==================================================
// Constructor
// ==================================================

VirtualCanBus::VirtualCanBus(
    std::uint32_t bitrate,
    LogLevel logLevel)
    : bitrate(bitrate),
    logLevel(logLevel)
{
}

// ==================================================
// CAN trace access
// ==================================================

const std::vector<CanTraceEntry>&
VirtualCanBus::getTrace() const
{
    return trace;
}

// ==================================================
// Queue frame for transmission
// ==================================================

void VirtualCanBus::transmit(
    const CanFrame& frame,
    double requestTimeMs)
{
    PendingCanFrame pending{};

    pending.frame =
        frame;

    pending.requestTimeMs =
        requestTimeMs;

    frames.push_back(
        pending
    );
}

// ==================================================
// Pending frame check
// ==================================================

bool VirtualCanBus::hasFrame() const
{
    return !frames.empty();
}

// ==================================================
// Receive / transmit winning CAN frame
// ==================================================

CanFrame VirtualCanBus::receive(
    double currentTimeMs)
{
    // ==================================================
    // Arbitration
    // ==================================================

    std::size_t winnerIndex =
        0;

    if (
        frames.size() >
        1
        )
    {
        statistics.recordArbitration();

        for (
            std::size_t index = 1;
            index < frames.size();
            ++index
            )
        {
            const std::uint32_t winnerId =
                CanArbitration::selectWinner(
                    frames[
                        winnerIndex
                    ].frame.arbitrationId,

                    frames[
                        index
                    ].frame.arbitrationId
                            );

            if (
                winnerId ==
                frames[
                    index
                ].frame.arbitrationId
                )
            {
                winnerIndex =
                    index;
            }
        }
    }

    // ==================================================
    // Select arbitration winner
    // ==================================================

    PendingCanFrame winner =
        frames[
            winnerIndex
        ];

    frames.erase(
        frames.begin() +
        winnerIndex
    );

    // ==================================================
    // Transmission timing
    // ==================================================

    const double txTimeMs =
        CanTiming::calculateTransmissionTimeMs(
            winner.frame,
            bitrate
        );

    const double startTimeMs =
        currentTimeMs;

    const double finishTimeMs =
        startTimeMs +
        txTimeMs;

    const double waitingTimeMs =
        startTimeMs -
        winner.requestTimeMs;

    busyUntilMs =
        finishTimeMs;

    // ==================================================
    // Statistics
    // ==================================================

    statistics.recordTransmission(
        winner.frame.arbitrationId,
        waitingTimeMs,
        txTimeMs
    );

    // ==================================================
    // CAN trace
    // ==================================================

    CanTraceEntry traceEntry{};

    traceEntry.txStartTimeMs =
        startTimeMs;

    traceEntry.arbitrationId =
        winner.frame.arbitrationId;

    traceEntry.dlc =
        winner.frame.dlc;

    traceEntry.waitingTimeMs =
        waitingTimeMs;

    traceEntry.data =
        winner.frame.data;

    trace.push_back(
        traceEntry
    );

    // ==================================================
    // Console output
    // ==================================================

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout
            << "[CAN BUS]\n";

        std::cout
            << "ID: 0x"
            << std::hex
            << winner.frame.arbitrationId
            << std::dec
            << "\n";

        std::cout
            << "Requested: "
            << winner.requestTimeMs
            << " ms\n";

        std::cout
            << "TX Start:  "
            << startTimeMs
            << " ms\n";

        std::cout
            << "TX End:    "
            << finishTimeMs
            << " ms\n";

        std::cout
            << "Wait:      "
            << waitingTimeMs
            << " ms\n";

        std::cout
            << "TX Time:   "
            << txTimeMs
            << " ms\n\n";
    }

    return winner.frame;
}

// ==================================================
// CAN busy-until time
// ==================================================

double VirtualCanBus::getBusyUntilMs() const
{
    return busyUntilMs;
}

// ==================================================
// CAN statistics access
// ==================================================

const CanStatistics&
VirtualCanBus::getStatistics() const
{
    return statistics;
}

// ==================================================
// CAN bitrate access
// ==================================================

std::uint32_t
VirtualCanBus::getBitrate() const
{
    return bitrate;
}

// ==================================================
// Console statistics
// ==================================================

void VirtualCanBus::printStatistics(
    double simulationTimeMs) const
{
    statistics.printSummary(
        bitrate,
        simulationTimeMs
    );
}