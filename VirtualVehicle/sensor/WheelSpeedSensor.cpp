#include "WheelSpeedSensor.h"

void WheelSpeedSensor::setTrueSpeed(
    float speedKmh)
{
    trueSpeedKmh = speedKmh;
}

WheelSpeedReading WheelSpeedSensor::readSpeed()
{
    WheelSpeedReading reading{};

    switch (fault)
    {
    case SensorFault::None:
        reading.speedKmh = trueSpeedKmh;
        reading.valid = true;

        lastValidReadingKmh = reading.speedKmh;
        hasLastValidReading = true;
        break;

    case SensorFault::Stuck:
        if (hasLastValidReading)
        {
            reading.speedKmh = lastValidReadingKmh;
            reading.valid = true;
        }
        else
        {
            reading.speedKmh = trueSpeedKmh;
            reading.valid = true;

            lastValidReadingKmh = reading.speedKmh;
            hasLastValidReading = true;
        }
        break;

    case SensorFault::Dropout:
        reading.speedKmh = 0.0f;
        reading.valid = false;
        break;

    case SensorFault::OutOfRange:
        reading.speedKmh = 999.0f;
        reading.valid = true;
        break;
    }

    return reading;
}

void WheelSpeedSensor::setFault(
    SensorFault newFault)
{
    fault = newFault;
}

void WheelSpeedSensor::clearFault()
{
    fault = SensorFault::None;
}

SensorFault WheelSpeedSensor::getFault() const
{
    return fault;
}