#pragma once

class VehicleModel
{
public:
    void setThrottle(float percent);
    void setBrake(float percent);
    void setSteeringAngle(float angleDeg);

    void setFrontLeftWheelSlip(bool active);

    // Enables a small simulated rolling-resistance effect.
    // Used by Recovery so the vehicle can coast down with
    // brake pressure remaining at 0%.
    void setCoasting(bool active);

    void update(double deltaTimeMs);

    float getVehicleSpeedKmh() const;

    float getFrontLeftWheelSpeedKmh() const;
    float getFrontRightWheelSpeedKmh() const;

    float getSteeringAngleDeg() const;

    float getBrakePercent() const;
    float getThrottlePercent() const;
    bool isCoasting() const;

private:
    float speedKmh{ 0.0f };

    float throttlePercent{ 0.0f };
    float brakePercent{ 0.0f };

    float steeringAngleDeg{ 0.0f };

    bool frontLeftWheelSlip{ false };
    bool coastingActive{ false };
};
