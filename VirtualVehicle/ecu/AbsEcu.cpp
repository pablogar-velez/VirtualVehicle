#include "AbsEcu.h"

#include "../can/AbsStateCodec.h"

void AbsEcu::updateSensors(
    const WheelSpeedReading& frontLeft,
    const WheelSpeedReading& frontRight)
{
    absState.frontLeftSensorValid =
        frontLeft.valid;

    absState.frontRightSensorValid =
        frontRight.valid;

    if (frontLeft.valid)
    {
        absState.wheelSpeedFrontLeft =
            frontLeft.speedKmh;
    }

    if (frontRight.valid)
    {
        absState.wheelSpeedFrontRight =
            frontRight.speedKmh;
    }

    // ==================================================
    // Sensor health
    // ==================================================

    if (
        !frontLeft.valid ||
        !frontRight.valid
        )
    {
        absState.healthStatus =
            AbsHealthStatus::Degraded;

        absState.absActive = false;

        return;
    }

    absState.healthStatus =
        AbsHealthStatus::Healthy;

    // ==================================================
    // Simplified wheel-slip detection
    // ==================================================

    const float wheelSpeedDifference =
        frontRight.speedKmh -
        frontLeft.speedKmh;

    constexpr float slipThresholdKmh = 5.0f;
    constexpr float minimumAbsSpeedKmh = 10.0f;

    if (
        frontRight.speedKmh > minimumAbsSpeedKmh &&
        wheelSpeedDifference > slipThresholdKmh
        )
    {
        absState.absActive = true;
    }
    else
    {
        absState.absActive = false;
    }
}

void AbsEcu::transmitAbsState(
    VirtualCanBus& bus,
    double requestTimeMs)
{
    const CanFrame frame =
        AbsStateCodec::encode(absState);

    bus.transmit(
        frame,
        requestTimeMs
    );
}

const AbsState& AbsEcu::getState() const
{
    return absState;
}