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

void VehicleModel::update(
    double deltaTimeMs)
{
    const float deltaTimeSeconds =
        static_cast<float>(
            deltaTimeMs / 1000.0
            );

    // Simplified longitudinal dynamics.
    //
    // Full throttle  -> approximately +4 m/s²
    // Full braking   -> approximately -8 m/s²

    const float throttleAcceleration =
        (throttlePercent / 100.0f) * 4.0f;

    const float brakingDeceleration =
        (brakePercent / 100.0f) * 8.0f;

    const float accelerationMs2 =
        throttleAcceleration -
        brakingDeceleration;

    // Convert acceleration from m/s² to km/h per second.
    const float speedChangeKmh =
        accelerationMs2 *
        deltaTimeSeconds *
        3.6f;

    speedKmh += speedChangeKmh;

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