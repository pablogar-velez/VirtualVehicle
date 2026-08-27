#pragma once

#include <array>
#include <cstdint>

struct CanTraceEntry
{
    double requestTimeMs{};
    double txStartTimeMs{};
    double txEndTimeMs{};

    double waitingTimeMs{};
    double transmissionTimeMs{};

    std::uint32_t arbitrationId{};
    std::uint8_t dlc{};

    std::array<std::uint8_t, 8> data{};
};