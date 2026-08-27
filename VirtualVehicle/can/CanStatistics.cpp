#include "CanStatistics.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "CanMessageRegistry.h"

void CanStatistics::recordTransmission(
    std::uint32_t arbitrationId,
    double waitingTimeMs,
    double transmissionTimeMs)
{
    ++totalFramesTransmitted;

    totalWaitingTimeMs += waitingTimeMs;
    totalTransmissionTimeMs += transmissionTimeMs;

    maximumWaitingTimeMs =
        std::max(
            maximumWaitingTimeMs,
            waitingTimeMs
        );

    auto& message =
        messageStatistics[arbitrationId];

    ++message.frameCount;

    message.totalWaitingTimeMs +=
        waitingTimeMs;

    message.maximumWaitingTimeMs =
        std::max(
            message.maximumWaitingTimeMs,
            waitingTimeMs
        );
}

void CanStatistics::recordArbitration()
{
    ++arbitrationCount;
}

void CanStatistics::printSummary(
    std::uint32_t bitrate,
    double simulationTimeMs) const
{
    double averageWaitingTimeMs = 0.0;
    double busUtilization = 0.0;

    // ==================================================
    // Global statistics
    // ==================================================

    if (totalFramesTransmitted > 0)
    {
        averageWaitingTimeMs =
            totalWaitingTimeMs /
            static_cast<double>(
                totalFramesTransmitted
                );
    }

    if (simulationTimeMs > 0.0)
    {
        busUtilization =
            (totalTransmissionTimeMs /
                simulationTimeMs) * 100.0;
    }

    std::cout << std::fixed
        << std::setprecision(3);

    std::cout << "\n";
    std::cout << "CAN BUS STATISTICS\n";
    std::cout << "==================\n\n";

    std::cout << "Bitrate:             "
        << bitrate / 1000
        << " kbps\n";

    std::cout << "Simulation time:     "
        << simulationTimeMs
        << " ms\n\n";

    std::cout << "Frames transmitted:  "
        << totalFramesTransmitted
        << "\n";

    std::cout << "Arbitrations:         "
        << arbitrationCount
        << "\n";

    std::cout << "Average wait:         "
        << averageWaitingTimeMs
        << " ms\n";

    std::cout << "Maximum wait:         "
        << maximumWaitingTimeMs
        << " ms\n";

    std::cout << "Bus utilization:      "
        << busUtilization
        << " %\n\n";

    // ==================================================
    // Per-message statistics
    // ==================================================

    std::vector<std::uint32_t> ids;

    ids.reserve(
        messageStatistics.size()
    );

    for (const auto& entry : messageStatistics)
    {
        ids.push_back(entry.first);
    }

    std::sort(
        ids.begin(),
        ids.end()
    );

    std::cout << "MESSAGE STATISTICS\n";
    std::cout << "==================\n\n";

    std::cout
        << std::left
        << std::setw(12) << "CAN ID"
        << std::setw(24) << "Message"
        << std::setw(12) << "Frames"
        << std::setw(16) << "Avg Wait"
        << std::setw(16) << "Max Wait"
        << "\n";

    std::cout
        << "----------------------------------------------------------------------------\n";

    for (const std::uint32_t id : ids)
    {
        const auto& stats =
            messageStatistics.at(id);

        double averageMessageWait = 0.0;

        if (stats.frameCount > 0)
        {
            averageMessageWait =
                stats.totalWaitingTimeMs /
                static_cast<double>(
                    stats.frameCount
                    );
        }

        const std::string messageName =
            CanMessageRegistry::getMessageName(id);

        // CAN ID
        std::cout
            << "0x"
            << std::hex
            << std::uppercase
            << std::setw(10)
            << id
            << std::dec
            << std::nouppercase;

        // Message name
        std::cout
            << std::setw(24)
            << messageName;

        // Number of transmitted frames
        std::cout
            << std::setw(12)
            << stats.frameCount;

        // Average waiting time
        std::cout
            << std::setw(16)
            << averageMessageWait;

        // Maximum waiting time
        std::cout
            << std::setw(16)
            << stats.maximumWaitingTimeMs;

        std::cout << "\n";
    }
}