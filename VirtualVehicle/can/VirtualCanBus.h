#pragma once

#include <cstdint>
#include <vector>

#include "CanFrame.h"
#include "PendingCanFrame.h"
#include "CanStatistics.h"

class VirtualCanBus
{
public:
    explicit VirtualCanBus(
        std::uint32_t bitrate = 500000
    );

    void transmit(
        const CanFrame& frame,
        double requestTimeMs
    );

    bool hasFrame() const;

    CanFrame receive(
        double currentTimeMs
    );

    double getBusyUntilMs() const;

    void printStatistics(
        double simulationTimeMs
    ) const;

private:
    std::vector<PendingCanFrame> frames;

    std::uint32_t bitrate;
    double busyUntilMs{};

    CanStatistics statistics;
};