#pragma once

#include <cstdint>
#include <vector>

#include "CanFrame.h"
#include "PendingCanFrame.h"
#include "CanStatistics.h"
#include "CanTraceEntry.h"

#include "../simulation/LogLevel.h"

class VirtualCanBus
{
public:
    explicit VirtualCanBus(
        std::uint32_t bitrate = 500000,
        LogLevel logLevel = LogLevel::Verbose
    );

    const std::vector<CanTraceEntry>&
        getTrace() const;

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

    std::vector<CanTraceEntry> trace;

    std::uint32_t bitrate;
    double busyUntilMs{};

    CanStatistics statistics;

    LogLevel logLevel;
};