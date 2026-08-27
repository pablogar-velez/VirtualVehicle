#pragma once

#include "VehicleModel.h"
#include "EventLogger.h"

enum class DrivingScenario
{
    Idle,
    Acceleration,
    Cruise,
    HardBraking,
    Recovery
};

class ScenarioController
{
public:
    ScenarioController() = default;

    void startScenario(
        DrivingScenario scenario,
        double currentTimeMs,
        EventLogger& eventLogger
    );

    void update(
        VehicleModel& vehicleModel
    );

    DrivingScenario getCurrentScenario() const;

private:
    DrivingScenario currentScenario{
        DrivingScenario::Idle
    };

    void logScenarioStart(
        double currentTimeMs,
        DrivingScenario scenario,
        EventLogger& eventLogger
    );
};