#include "DashboardEcu.h"

#include <iostream>

#include "../can/VehicleStateCodec.h"
#include "../can/AbsStateCodec.h"
#include "../can/SteeringStateCodec.h"
#include "../can/CanMessageDefinitions.h"

void DashboardEcu::receiveFrame(const CanFrame& frame)
{
    // Powertrain message
    if (
        frame.arbitrationId ==
        CanMessageDefinitions::POWERTRAIN_STATE.id
        )
    {
        vehicleState =
            VehicleStateCodec::decode(frame);
    }

    // ABS message
    else if (
        frame.arbitrationId ==
        CanMessageDefinitions::ABS_WHEEL_STATE.id
        )
    {
        absState =
            AbsStateCodec::decode(frame);
    }

    // Steering message
    else if (
        frame.arbitrationId ==
        CanMessageDefinitions::STEERING_STATE.id
        )
    {
        steeringState =
            SteeringStateCodec::decode(frame);
    }
}

void DashboardEcu::displayVehicleState() const
{
    std::cout << "\n";
    std::cout << "DASHBOARD ECU\n";
    std::cout << "=============\n\n";

    // ==================================================
    // Vehicle state
    // ==================================================

    std::cout << "VEHICLE STATE\n";
    std::cout << "-------------\n";

    std::cout << "Speed: "
        << vehicleState.speedKmh
        << " km/h\n";

    std::cout << "RPM:   "
        << vehicleState.engineRpm
        << "\n";

    std::cout << "Gear:  "
        << static_cast<int>(vehicleState.gear)
        << "\n";

    std::cout << "Brake: "
        << (
            vehicleState.brakeApplied
            ? "APPLIED"
            : "RELEASED"
            )
        << "\n";

    // ==================================================
    // ABS state
    // ==================================================

    std::cout << "\n";
    std::cout << "ABS STATE\n";
    std::cout << "---------\n";

    std::cout << "Front Left Wheel:  ";

    if (absState.frontLeftSensorValid)
    {
        std::cout
            << absState.wheelSpeedFrontLeft
            << " km/h\n";
    }
    else
    {
        std::cout << "INVALID\n";
    }

    std::cout << "Front Right Wheel: ";

    if (absState.frontRightSensorValid)
    {
        std::cout
            << absState.wheelSpeedFrontRight
            << " km/h\n";
    }
    else
    {
        std::cout << "INVALID\n";
    }

    std::cout << "ABS: "
        << (
            absState.absActive
            ? "ACTIVE"
            : "INACTIVE"
            )
        << "\n";

    std::cout << "ABS Health: "
        << (
            absState.healthStatus ==
            AbsHealthStatus::Healthy
            ? "HEALTHY"
            : "DEGRADED"
            )
        << "\n";

    // ==================================================
    // ABS warnings
    // ==================================================

    if (
        absState.healthStatus ==
        AbsHealthStatus::Degraded
        )
    {
        std::cout << "\n";
        std::cout << "!!! WARNING !!!\n";
        std::cout << "ABS SYSTEM DEGRADED\n";

        if (!absState.frontLeftSensorValid)
        {
            std::cout
                << "Front Left Wheel Speed Sensor Fault\n";
        }

        if (!absState.frontRightSensorValid)
        {
            std::cout
                << "Front Right Wheel Speed Sensor Fault\n";
        }
    }

    // ==================================================
    // Steering state
    // ==================================================

    std::cout << "\n";
    std::cout << "STEERING STATE\n";
    std::cout << "--------------\n";

    std::cout << "Angle:  "
        << steeringState.steeringAngleDeg
        << " deg\n";

    std::cout << "Torque: "
        << steeringState.steeringTorqueNm
        << " Nm\n";
}