#pragma once

#include "../VirtualVehicle.h"
#include "../can/VirtualCanBus.h"
#include "../sensor/WheelSpeedSensor.h"

class AbsEcu
{
public:
    void updateSensors(
        const WheelSpeedReading& frontLeft,
        const WheelSpeedReading& frontRight
    );

    void transmitAbsState(
        VirtualCanBus& bus,
        double requestTimeMs
    );

    const AbsState& getState() const;

private:
    AbsState absState{};
};