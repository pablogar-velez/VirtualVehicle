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

    // ==================================================
    // CAN trace
    // ==================================================

    const std::vector<CanTraceEntry>&
        getTrace() const;

    // ==================================================
    // Transmission
    // ==================================================

    void transmit(
        const CanFrame& frame,
        double requestTimeMs
    );

    bool hasFrame() const;

    CanFrame receive(
        double currentTimeMs
    );

    // ==================================================
    // CAN timing
    // ==================================================

    double getBusyUntilMs() const;

    // ==================================================
    // CAN statistics access
    // ==================================================

    const CanStatistics&
        getStatistics() const;

    std::uint32_t
        getBitrate() const;

    // ==================================================
    // Console statistics
    // ==================================================

    void printStatistics(
        double simulationTimeMs
    ) const;

private:
    // ==================================================
    // Pending frames
    // ==================================================

    std::vector<PendingCanFrame> frames;

    // ==================================================
    // CAN trace
    // ==================================================

    std::vector<CanTraceEntry> trace;

    // ==================================================
    // Configuration
    // ==================================================

    std::uint32_t bitrate;

    LogLevel logLevel;

    // ==================================================
    // Timing
    // ==================================================

    double busyUntilMs{};

    // ==================================================
    // Statistics
    // ==================================================

    CanStatistics statistics;
};