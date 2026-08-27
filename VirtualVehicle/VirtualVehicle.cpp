#include <iomanip>
#include <iostream>

#include "simulation/SimulationEngine.h"
#include "simulation/LogLevel.h"

int main()
{
    std::cout
        << std::fixed
        << std::setprecision(3);

    std::cout
        << "=================================\n"
        << "     VIRTUAL VEHICLE PLATFORM    \n"
        << "=================================\n\n";

    SimulationEngine engine(
        LogLevel::Events
    );

    // ==================================================
    // Start vehicle
    // ==================================================

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    // ==================================================
    // 0 -> 2000 ms
    // Normal operation
    // ==================================================

    while (
        engine.getCurrentTimeMs() <
        2000.0
        )
    {
        engine.update(10.0);
    }

    // ==================================================
    // Inject FL wheel sensor fault
    // ==================================================

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    // ==================================================
    // 2000 -> 2500 ms
    // Fault active
    // ==================================================

    while (
        engine.getCurrentTimeMs() <
        2500.0
        )
    {
        engine.update(10.0);
    }

    // ==================================================
    // Clear fault
    // ==================================================

    engine.clearFrontLeftWheelSensorFault();

    // ==================================================
    // 2500 -> 3000 ms
    // Recovery
    // ==================================================

    while (
        engine.getCurrentTimeMs() <
        3000.0
        )
    {
        engine.update(10.0);
    }

    // ==================================================
    // Results
    // ==================================================

    engine.printResults();

    return 0;
}