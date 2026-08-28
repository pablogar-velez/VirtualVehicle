#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>

struct CanMessageStatistics
{
    std::size_t frameCount{ 0 };

    double totalWaitingTimeMs{ 0.0 };
    double maximumWaitingTimeMs{ 0.0 };

    double getAverageWaitingTimeMs() const;
};

class CanStatistics
{
public:
    // ==================================================
    // Recording
    // ==================================================

    void recordTransmission(
        std::uint32_t arbitrationId,
        double waitingTimeMs,
        double transmissionTimeMs
    );

    void recordArbitration();

    // ==================================================
    // Global statistics getters
    // ==================================================

    std::size_t getFramesTransmitted() const;

    std::size_t getArbitrationCount() const;

    double getAverageWaitingTimeMs() const;

    double getMaximumWaitingTimeMs() const;

    double getTotalTransmissionTimeMs() const;

    double getBusUtilization(
        double simulationTimeMs
    ) const;

    // ==================================================
    // Per-message statistics
    // ==================================================

    const std::unordered_map<
        std::uint32_t,
        CanMessageStatistics
    >& getMessageStatistics() const;

    // ==================================================
    // Console output
    // ==================================================

    void printSummary(
        std::uint32_t bitrate,
        double simulationTimeMs
    ) const;

    // ==================================================
    // Reset
    // ==================================================

    void clear();

private:
    std::size_t framesTransmitted{ 0 };
    std::size_t arbitrationCount{ 0 };

    double totalWaitingTimeMs{ 0.0 };
    double maximumWaitingTimeMs{ 0.0 };

    double totalTransmissionTimeMs{ 0.0 };

    std::unordered_map<
        std::uint32_t,
        CanMessageStatistics
    > messageStatistics;
};