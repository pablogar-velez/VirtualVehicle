#include "CanStatistics.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "CanMessageRegistry.h"

// ==================================================
// Message average waiting time
// ==================================================

double CanMessageStatistics::getAverageWaitingTimeMs() const
{
    if (frameCount == 0)
    {
        return 0.0;
    }

    return
        totalWaitingTimeMs /
        static_cast<double>(
            frameCount
            );
}

// ==================================================
// Record CAN transmission
// ==================================================

void CanStatistics::recordTransmission(
    std::uint32_t arbitrationId,
    double waitingTimeMs,
    double transmissionTimeMs)
{
    ++framesTransmitted;

    totalWaitingTimeMs +=
        waitingTimeMs;

    maximumWaitingTimeMs =
        std::max(
            maximumWaitingTimeMs,
            waitingTimeMs
        );

    totalTransmissionTimeMs +=
        transmissionTimeMs;

    // ==================================================
    // Per-message statistics
    // ==================================================

    CanMessageStatistics& messageStats =
        messageStatistics[
            arbitrationId
        ];

    ++messageStats.frameCount;

    messageStats.totalWaitingTimeMs +=
        waitingTimeMs;

    messageStats.maximumWaitingTimeMs =
        std::max(
            messageStats.maximumWaitingTimeMs,
            waitingTimeMs
        );
}

// ==================================================
// Arbitration
// ==================================================

void CanStatistics::recordArbitration()
{
    ++arbitrationCount;
}

// ==================================================
// Global getters
// ==================================================

std::size_t
CanStatistics::getFramesTransmitted() const
{
    return framesTransmitted;
}

std::size_t
CanStatistics::getArbitrationCount() const
{
    return arbitrationCount;
}

double
CanStatistics::getAverageWaitingTimeMs() const
{
    if (framesTransmitted == 0)
    {
        return 0.0;
    }

    return
        totalWaitingTimeMs /
        static_cast<double>(
            framesTransmitted
            );
}

double
CanStatistics::getMaximumWaitingTimeMs() const
{
    return maximumWaitingTimeMs;
}

double
CanStatistics::getTotalTransmissionTimeMs() const
{
    return totalTransmissionTimeMs;
}

double
CanStatistics::getBusUtilization(
    double simulationTimeMs) const
{
    if (simulationTimeMs <= 0.0)
    {
        return 0.0;
    }

    return
        (
            totalTransmissionTimeMs /
            simulationTimeMs
            ) *
        100.0;
}

// ==================================================
// Per-message statistics getter
// ==================================================

const std::unordered_map<
    std::uint32_t,
    CanMessageStatistics
>&
CanStatistics::getMessageStatistics() const
{
    return messageStatistics;
}

// ==================================================
// Clear
// ==================================================

void CanStatistics::clear()
{
    framesTransmitted =
        0;

    arbitrationCount =
        0;

    totalWaitingTimeMs =
        0.0;

    maximumWaitingTimeMs =
        0.0;

    totalTransmissionTimeMs =
        0.0;

    messageStatistics.clear();
}

// ==================================================
// Console summary
// ==================================================

void CanStatistics::printSummary(
    std::uint32_t bitrate,
    double simulationTimeMs) const
{
    std::cout << "\n";
    std::cout << "CAN BUS STATISTICS\n";
    std::cout << "==================\n\n";

    std::cout
        << "Bitrate:             "
        << bitrate / 1000
        << " kbps\n";

    std::cout
        << "Simulation time:     "
        << simulationTimeMs
        << " ms\n\n";

    std::cout
        << "Frames transmitted:  "
        << framesTransmitted
        << "\n";

    std::cout
        << "Arbitrations:         "
        << arbitrationCount
        << "\n";

    std::cout
        << "Average wait:         "
        << getAverageWaitingTimeMs()
        << " ms\n";

    std::cout
        << "Maximum wait:         "
        << maximumWaitingTimeMs
        << " ms\n";

    std::cout
        << "Bus utilization:      "
        << getBusUtilization(
            simulationTimeMs
        )
        << " %\n";

    // ==================================================
    // Message Statistics
    // ==================================================

    std::cout << "\n";
    std::cout << "MESSAGE STATISTICS\n";
    std::cout << "==================\n\n";

    std::cout
        << std::left
        << std::setw(12)
        << "CAN ID"
        << std::setw(24)
        << "Message"
        << std::setw(12)
        << "Frames"
        << std::setw(16)
        << "Avg Wait"
        << "Max Wait\n";

    std::cout
        << "----------------------------------------------------------------------------\n";

    // unordered_map has no deterministic ordering, so sort IDs.
    std::vector<std::uint32_t> ids;

    ids.reserve(
        messageStatistics.size()
    );

    for (
        const auto& entry :
        messageStatistics
        )
    {
        ids.push_back(
            entry.first
        );
    }

    std::sort(
        ids.begin(),
        ids.end()
    );

    for (
        std::uint32_t id :
    ids
        )
    {
        const CanMessageStatistics& stats =
            messageStatistics.at(
                id
            );

        std::cout
            << "0x"
            << std::hex
            << id
            << std::dec;

        // Restore spacing after hexadecimal ID.
        std::cout
            << std::setw(
                static_cast<int>(
                    12 -
                    2
                    )
            )
            << " ";

        std::cout
            << std::left
            << std::setw(24)
            << CanMessageRegistry::getMessageName(
                id
            )

            << std::setw(12)
            << stats.frameCount

            << std::setw(16)
            << stats.getAverageWaitingTimeMs()

            << stats.maximumWaitingTimeMs
            << "\n";
    }
}