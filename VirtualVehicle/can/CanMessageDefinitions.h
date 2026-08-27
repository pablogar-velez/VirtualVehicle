#pragma once

#include <cstdint>

// ==================================================
// CAN message metadata
// ==================================================

struct CanMessageDefinition
{
    std::uint32_t id;
    const char* name;
    std::uint8_t dlc;
    double periodMs;
};

// ==================================================
// Virtual Vehicle CAN database
// ==================================================

namespace CanMessageDefinitions
{
    constexpr CanMessageDefinition ABS_WHEEL_STATE
    {
        0x080,
        "ABS_WHEEL_STATE",
        5,
        10.0
    };

    constexpr CanMessageDefinition POWERTRAIN_STATE
    {
        0x100,
        "POWERTRAIN_STATE",
        6,
        20.0
    };

    constexpr CanMessageDefinition STEERING_STATE
    {
        0x120,
        "STEERING_STATE",
        4,
        20.0
    };
}