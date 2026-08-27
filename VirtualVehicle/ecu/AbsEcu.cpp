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

    if (
        !frontLeft.valid ||
        !frontRight.valid
        )
    {
        absState.healthStatus =
            AbsHealthStatus::Degraded;

        absState.absActive = false;
    }
    else
    {
        absState.healthStatus =
            AbsHealthStatus::Healthy;
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