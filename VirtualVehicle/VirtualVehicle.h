#pragma once

#include <cstdint>

struct VehicleState
{
    float speedKmh{};
    std::uint16_t engineRpm{};
    std::uint8_t gear{};
    bool brakeApplied{};
};

enum class AbsHealthStatus
{
    Healthy,
    Degraded
};

struct AbsState
{
    float wheelSpeedFrontLeft{};
    float wheelSpeedFrontRight{};

    bool absActive{};

    bool frontLeftSensorValid{ true };
    bool frontRightSensorValid{ true };

    AbsHealthStatus healthStatus{
        AbsHealthStatus::Healthy
    };
};

struct SteeringState
{
    float steeringAngleDeg{};
    float steeringTorqueNm{};
};