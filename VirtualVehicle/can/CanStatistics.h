#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>

struct CanMessageStatistics
{
    std::size_t frameCount{};
    double totalWaitingTimeMs{};
    double maximumWaitingTimeMs{};
};

class CanStatistics
{
public:
    void recordTransmission(
        std::uint32_t arbitrationId,
        double waitingTimeMs,
        double transmissionTimeMs
    );

    void recordArbitration();

    void printSummary(
        std::uint32_t bitrate,
        double simulationTimeMs
    ) const;

private:
    std::size_t totalFramesTransmitted{};
    std::size_t arbitrationCount{};

    double totalWaitingTimeMs{};
    double maximumWaitingTimeMs{};
    double totalTransmissionTimeMs{};

    std::unordered_map<
        std::uint32_t,
        CanMessageStatistics
    > messageStatistics;
};