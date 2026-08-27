#pragma once

#include "SensorFault.h"

struct WheelSpeedReading
{
    float speedKmh{ 0.0f };
    bool valid{ true };
};

class WheelSpeedSensor
{
public:
    void setTrueSpeed(float speedKmh);

    WheelSpeedReading readSpeed();

    void setFault(SensorFault fault);
    void clearFault();

    SensorFault getFault() const;

private:
    float trueSpeedKmh{ 0.0f };

    SensorFault fault{ SensorFault::None };

    float lastValidReadingKmh{ 0.0f };
    bool hasLastValidReading{ false };
};