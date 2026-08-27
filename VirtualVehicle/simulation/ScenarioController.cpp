#include "ScenarioController.h"

void ScenarioController::startScenario(
    DrivingScenario scenario,
    double currentTimeMs,
    EventLogger& eventLogger)
{
    // Avoid restarting the same scenario repeatedly.
    if (scenario == currentScenario)
    {
        return;
    }

    currentScenario = scenario;

    logScenarioStart(
        currentTimeMs,
        scenario,
        eventLogger
    );
}

void ScenarioController::update(
    VehicleModel& vehicleModel)
{
    switch (currentScenario)
    {
    case DrivingScenario::Idle:

        vehicleModel.setThrottle(0.0f);
        vehicleModel.setBrake(0.0f);
        vehicleModel.setFrontLeftWheelSlip(false);

        break;

    case DrivingScenario::Acceleration:

        vehicleModel.setThrottle(70.0f);
        vehicleModel.setBrake(0.0f);
        vehicleModel.setFrontLeftWheelSlip(false);

        break;

    case DrivingScenario::Cruise:

        vehicleModel.setThrottle(25.0f);
        vehicleModel.setBrake(0.0f);
        vehicleModel.setFrontLeftWheelSlip(false);

        break;

    case DrivingScenario::HardBraking:

        vehicleModel.setThrottle(0.0f);
        vehicleModel.setBrake(90.0f);

        // Simplified wheel-slip condition.
        vehicleModel.setFrontLeftWheelSlip(true);

        break;

    case DrivingScenario::Recovery:

        vehicleModel.setThrottle(0.0f);
        vehicleModel.setBrake(0.0f);
        vehicleModel.setFrontLeftWheelSlip(false);

        break;
    }
}

DrivingScenario
ScenarioController::getCurrentScenario() const
{
    return currentScenario;
}

void ScenarioController::logScenarioStart(
    double currentTimeMs,
    DrivingScenario scenario,
    EventLogger& eventLogger)
{
    switch (scenario)
    {
    case DrivingScenario::Idle:

        eventLogger.log(
            currentTimeMs,
            "SCENARIO",
            "Idle"
        );

        break;

    case DrivingScenario::Acceleration:

        eventLogger.log(
            currentTimeMs,
            "SCENARIO",
            "Acceleration started"
        );

        break;

    case DrivingScenario::Cruise:

        eventLogger.log(
            currentTimeMs,
            "SCENARIO",
            "Cruise started"
        );

        break;

    case DrivingScenario::HardBraking:

        eventLogger.log(
            currentTimeMs,
            "SCENARIO",
            "HARD BRAKING started"
        );

        break;

    case DrivingScenario::Recovery:

        eventLogger.log(
            currentTimeMs,
            "SCENARIO",
            "Recovery started"
        );

        break;
    }
}