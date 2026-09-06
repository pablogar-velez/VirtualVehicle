#include "VehicleModel.h"

#include <algorithm>

void VehicleModel::setThrottle(float percent)
{
    throttlePercent =
        std::clamp(percent, 0.0f, 100.0f);
}

void VehicleModel::setBrake(float percent)
{
    brakePercent =
        std::clamp(percent, 0.0f, 100.0f);
}

void VehicleModel::setSteeringAngle(
    float angleDeg)
{
    steeringAngleDeg = angleDeg;
}

void VehicleModel::setFrontLeftWheelSlip(
    bool active)
{
    frontLeftWheelSlip = active;
}

void VehicleModel::setCoasting(
    bool active)
{
    coastingActive = active;
}

void VehicleModel::update(
    double deltaTimeMs)
{
    const float deltaTimeSeconds =
        static_cast<float>(
            deltaTimeMs / 1000.0
            );

    // ==================================================
    // Simplified longitudinal dynamics
    // ==================================================

    const float throttleAcceleration =
        (throttlePercent / 100.0f) *
        4.0f;

    const float brakingDeceleration =
        (brakePercent / 100.0f) *
        8.0f;

    // Recovery uses a small rolling-resistance model so
    // the vehicle naturally loses speed without applying
    // the service brake.
    const float coastingDeceleration =
        (
            coastingActive &&
            speedKmh > 0.0f
            )
        ? 0.65f
        : 0.0f;

    const float accelerationMs2 =
        throttleAcceleration -
        brakingDeceleration -
        coastingDeceleration;

    speedKmh +=
        accelerationMs2 *
        deltaTimeSeconds *
        3.6f;

    if (speedKmh < 0.0f)
    {
        speedKmh = 0.0f;
    }
}

float VehicleModel::getVehicleSpeedKmh() const
{
    return speedKmh;
}

float VehicleModel::getFrontLeftWheelSpeedKmh() const
{
    if (frontLeftWheelSlip)
    {
        // Simplified locked/slipping-wheel behavior.
        return speedKmh * 0.60f;
    }

    return speedKmh;
}

float VehicleModel::getFrontRightWheelSpeedKmh() const
{
    return speedKmh;
}

float VehicleModel::getSteeringAngleDeg() const
{
    return steeringAngleDeg;
}

float VehicleModel::getBrakePercent() const
{
    return brakePercent;
}


float VehicleModel::getThrottlePercent() const
{
    return throttlePercent;
}

bool VehicleModel::isCoasting() const
{
    return coastingActive;
}
