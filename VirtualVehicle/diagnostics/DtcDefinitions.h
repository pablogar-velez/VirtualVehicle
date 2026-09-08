#pragma once

#include <cstdint>

namespace DtcDefinitions
{
    // ========================================================
    // Wheel-speed sensor faults
    // ========================================================

    constexpr std::uint32_t FrontLeftWheelSpeedSensor =
        0xC0031;

    constexpr std::uint32_t FrontRightWheelSpeedSensor =
        0xC0034;

    // ========================================================
    // Communication faults
    // ========================================================

    constexpr std::uint32_t AbsCanCommunication =
        0xC1001;

    constexpr std::uint32_t PowertrainCanCommunication =
        0xC1002;

    constexpr std::uint32_t SteeringCanCommunication =
        0xC1003;

    constexpr std::uint32_t EthernetNodeACommunication =
        0xC2001;
}
