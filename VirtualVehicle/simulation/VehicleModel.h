#pragma once

class VehicleModel
{
public:
    void setThrottle(float percent);
    void setBrake(float percent);
    void setSteeringAngle(float angleDeg);

    void update(double deltaTimeMs);

    float getVehicleSpeedKmh() const;
    float getFrontLeftWheelSpeedKmh() const;
    float getFrontRightWheelSpeedKmh() const;

    float getSteeringAngleDeg() const;

private:
    float speedKmh{ 0.0f };

    float throttlePercent{ 0.0f };
    float brakePercent{ 0.0f };

    float steeringAngleDeg{ 0.0f };
};