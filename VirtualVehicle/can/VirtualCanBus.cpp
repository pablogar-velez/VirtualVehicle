#include "VirtualCanBus.h"

#include <iostream>

#include "CanArbitration.h"
#include "CanTiming.h"

VirtualCanBus::VirtualCanBus(
    std::uint32_t bitrate,
    LogLevel logLevel)
    : bitrate(bitrate),
    logLevel(logLevel)
{
}

void VirtualCanBus::transmit(
    const CanFrame& frame,
    double requestTimeMs)
{
    PendingCanFrame pending{};

    pending.frame = frame;
    pending.requestTimeMs = requestTimeMs;

    frames.push_back(pending);
}

bool VirtualCanBus::hasFrame() const
{
    return !frames.empty();
}

CanFrame VirtualCanBus::receive(
    double currentTimeMs)
{
    // --------------------------------------------------
    // Arbitration
    // --------------------------------------------------

    std::size_t winnerIndex = 0;

    if (frames.size() > 1)
    {
        statistics.recordArbitration();

        for (std::size_t i = 1; i < frames.size(); ++i)
        {
            const std::uint32_t winnerId =
                CanArbitration::selectWinner(
                    frames[winnerIndex]
                    .frame
                    .arbitrationId,

                    frames[i]
                    .frame
                    .arbitrationId
                );

            if (
                winnerId ==
                frames[i].frame.arbitrationId
                )
            {
                winnerIndex = i;
            }
        }
    }

    // --------------------------------------------------
    // Select winning frame
    // --------------------------------------------------

    PendingCanFrame winner =
        frames[winnerIndex];

    frames.erase(
        frames.begin() + winnerIndex
    );

    // --------------------------------------------------
    // CAN transmission timing
    // --------------------------------------------------

    const double txTimeMs =
        CanTiming::calculateTransmissionTimeMs(
            winner.frame,
            bitrate
        );

    const double startTimeMs =
        currentTimeMs;

    const double finishTimeMs =
        startTimeMs + txTimeMs;

    const double waitingTimeMs =
        startTimeMs -
        winner.requestTimeMs;

    busyUntilMs =
        finishTimeMs;

    // --------------------------------------------------
    // Statistics
    // --------------------------------------------------

    statistics.recordTransmission(
        winner.frame.arbitrationId,
        waitingTimeMs,
        txTimeMs
    );

    // --------------------------------------------------
    // CAN trace
    // --------------------------------------------------

    trace.push_back(
        CanTraceEntry{
            winner.requestTimeMs,
            startTimeMs,
            finishTimeMs,
            waitingTimeMs,
            txTimeMs,
            winner.frame.arbitrationId,
            winner.frame.dlc,
            winner.frame.data
        }
    );

    // --------------------------------------------------
    // Console output
    // --------------------------------------------------

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout << "[CAN BUS]\n";

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

double VirtualCanBus::getBusyUntilMs() const
{
    return busyUntilMs;
}

const std::vector<CanTraceEntry>&
VirtualCanBus::getTrace() const
{
    return trace;
}

void VirtualCanBus::printStatistics(
    double simulationTimeMs) const
{
    statistics.printSummary(
        bitrate,
        simulationTimeMs
    );
}