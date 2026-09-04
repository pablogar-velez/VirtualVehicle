#include "TestRunner.h"
#include "TestConfig.h"

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include "../simulation/SimulationEngine.h"
#include "../sensor/WheelSpeedSensor.h"
#include "../can/VirtualCanBus.h"
#include "../can/CanMessageDefinitions.h"
#include "../can/CanTiming.h"
#include "../can/VehicleStateCodec.h"
#include "../can/SteeringStateCodec.h"
#include "../can/AbsStateCodec.h"

#include "../ecu/PowertrainEcu.h"
#include "../ecu/SteeringEcu.h"

using namespace TestConfig;

// ==================================================

TestCase TestRunner::runAccelerationScenarioTest()
{
    TestCase test;

    test.id =
        "TC_VEH_001";

    test.name =
        "Acceleration Scenario";

    test.description =
        "Verify that vehicle speed increases during "
        "the Acceleration driving scenario.";

    test.requirementId =
        "VEH-REQ-007";

    test.precondition =
        "Vehicle simulation shall be reset.";

    test.stimulus =
        "Execute the Acceleration scenario for 2000 ms.";

    test.expectedResult =
        "Vehicle speed shall increase from its initial value.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Simulation
    // ==================================================

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    const double initialSpeedKmh =
        engine
        .getVehicleModel()
        .getVehicleSpeedKmh();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        2000.0
    );

    const double finalSpeedKmh =
        engine
        .getVehicleModel()
        .getVehicleSpeedKmh();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Final speed > initial speed";

    test.result.actual =
        "Initial = " +
        std::to_string(
            initialSpeedKmh
        ) +
        " km/h, Final = " +
        std::to_string(
            finalSpeedKmh
        ) +
        " km/h";

    if (
        finalSpeedKmh >
        initialSpeedKmh +
        speedToleranceKmh
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Vehicle speed increased during Acceleration.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Vehicle speed did not increase during Acceleration.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCruiseScenarioTest()
{
    TestCase test;

    test.id =
        "TC_VEH_002";

    test.name =
        "Cruise Scenario";

    test.description =
        "Verify execution of the Cruise driving scenario "
        "after vehicle acceleration.";

    test.requirementId =
        "SYS-REQ-002";

    test.precondition =
        "Vehicle shall be moving before Cruise starts.";

    test.stimulus =
        "Transition from Acceleration to Cruise.";

    test.expectedResult =
        "Vehicle shall remain moving and brake input "
        "shall remain released during Cruise.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Simulation
    // ==================================================

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        2000.0
    );

    const double speedBeforeCruiseKmh =
        engine
        .getVehicleModel()
        .getVehicleSpeedKmh();

    engine.startScenario(
        DrivingScenario::Cruise
    );

    runUntil(
        engine,
        3000.0
    );

    const double speedDuringCruiseKmh =
        engine
        .getVehicleModel()
        .getVehicleSpeedKmh();

    const double brakePercent =
        engine
        .getVehicleModel()
        .getBrakePercent();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Vehicle moving, Brake = 0%";

    test.result.actual =
        "Speed before Cruise = " +
        std::to_string(
            speedBeforeCruiseKmh
        ) +
        " km/h, Speed during Cruise = " +
        std::to_string(
            speedDuringCruiseKmh
        ) +
        " km/h, Brake = " +
        std::to_string(
            brakePercent
        ) +
        "%";

    const bool vehicleMoving =
        speedDuringCruiseKmh >
        speedToleranceKmh;

    const bool brakeReleased =
        brakePercent <=
        brakeReleasedTolerancePercent;

    if (
        vehicleMoving &&
        brakeReleased
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Cruise scenario executed with vehicle moving "
            "and brake released.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Cruise scenario state did not match "
            "the expected vehicle behavior.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runHardBrakingScenarioTest()
{
    TestCase test;

    test.id =
        "TC_VEH_003";

    test.name =
        "Hard Braking Scenario";

    test.description =
        "Verify that vehicle speed decreases when "
        "the Hard Braking scenario is executed.";

    test.requirementId =
        "VEH-REQ-008";

    test.precondition =
        "Vehicle shall be moving before hard braking.";

    test.stimulus =
        "Execute Hard Braking after Acceleration and Cruise.";

    test.expectedResult =
        "Brake input shall be applied and vehicle speed "
        "shall decrease.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Simulation
    // ==================================================

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        2000.0
    );

    engine.startScenario(
        DrivingScenario::Cruise
    );

    runUntil(
        engine,
        3000.0
    );

    const double speedBeforeBrakingKmh =
        engine
        .getVehicleModel()
        .getVehicleSpeedKmh();

    engine.startScenario(
        DrivingScenario::HardBraking
    );

    runUntil(
        engine,
        3500.0
    );

    const double speedDuringBrakingKmh =
        engine
        .getVehicleModel()
        .getVehicleSpeedKmh();

    const double brakePercent =
        engine
        .getVehicleModel()
        .getBrakePercent();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Brake > 0% and final speed < initial speed";

    test.result.actual =
        "Before = " +
        std::to_string(
            speedBeforeBrakingKmh
        ) +
        " km/h, After = " +
        std::to_string(
            speedDuringBrakingKmh
        ) +
        " km/h, Brake = " +
        std::to_string(
            brakePercent
        ) +
        "%";

    const bool brakeApplied =
        brakePercent >
        brakeReleasedTolerancePercent;

    const bool vehicleDecelerated =
        speedDuringBrakingKmh <
        speedBeforeBrakingKmh;

    if (
        brakeApplied &&
        vehicleDecelerated
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Vehicle decelerated correctly during Hard Braking.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Vehicle did not produce the expected "
            "Hard Braking behavior.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runRecoveryScenarioTest()
{
    TestCase test;

    test.id =
        "TC_VEH_004";

    test.name =
        "Recovery Scenario";

    test.description =
        "Verify that brake input is released when the "
        "vehicle transitions from Hard Braking to Recovery.";

    test.requirementId =
        "SYS-REQ-002";

    test.precondition =
        "Vehicle shall be executing Hard Braking.";

    test.stimulus =
        "Transition from Hard Braking to Recovery.";

    test.expectedResult =
        "Brake input shall return to the released state.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Simulation
    // ==================================================

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        2000.0
    );

    engine.startScenario(
        DrivingScenario::Cruise
    );

    runUntil(
        engine,
        3000.0
    );

    engine.startScenario(
        DrivingScenario::HardBraking
    );

    runUntil(
        engine,
        3500.0
    );

    const double brakeDuringHardBraking =
        engine
        .getVehicleModel()
        .getBrakePercent();

    engine.startScenario(
        DrivingScenario::Recovery
    );

    runUntil(
        engine,
        4000.0
    );

    const double brakeDuringRecovery =
        engine
        .getVehicleModel()
        .getBrakePercent();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Brake applied before Recovery, "
        "Brake = 0% during Recovery";

    test.result.actual =
        "Hard Braking = " +
        std::to_string(
            brakeDuringHardBraking
        ) +
        "%, Recovery = " +
        std::to_string(
            brakeDuringRecovery
        ) +
        "%";

    const bool brakingWasActive =
        brakeDuringHardBraking >
        brakeReleasedTolerancePercent;

    const bool brakeReleased =
        brakeDuringRecovery <=
        brakeReleasedTolerancePercent;

    if (
        brakingWasActive &&
        brakeReleased
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Recovery scenario released the brake input.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Recovery scenario did not release "
            "the brake input as expected.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runVehicleSpeedStateTest()
{
    TestCase test;

    test.id = "TC_VEH_005";
    test.name = "Vehicle Speed State";
    test.description =
        "Verify that the vehicle model provides simulated vehicle-speed information.";
    test.requirementId = "VEH-REQ-001";
    test.precondition = "Vehicle simulation shall be reset.";
    test.stimulus = "Execute Acceleration for 1000 ms and read vehicle speed.";
    test.expectedResult = "Vehicle speed shall be available and greater than zero.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const double speedKmh =
        engine.getVehicleModel().getVehicleSpeedKmh();

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "Vehicle speed > 0 km/h";
    test.result.actual = "Vehicle speed = " + std::to_string(speedKmh) + " km/h";

    if (std::isfinite(speedKmh) && speedKmh > speedToleranceKmh)
    {
        test.result.status = TestStatus::Passed;
        test.result.message = "Vehicle model provided a valid simulated vehicle speed.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message = "Vehicle speed was not available as expected.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runFrontLeftWheelSpeedStateTest()
{
    TestCase test;

    test.id = "TC_VEH_006";
    test.name = "Front-Left Wheel Speed State";
    test.description =
        "Verify that the vehicle model provides simulated front-left wheel speed.";
    test.requirementId = "VEH-REQ-002";
    test.precondition = "Vehicle simulation shall be reset.";
    test.stimulus = "Execute Acceleration for 1000 ms and read front-left wheel speed.";
    test.expectedResult = "Front-left wheel speed shall be available and greater than zero.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const double wheelSpeedKmh =
        engine.getVehicleModel().getFrontLeftWheelSpeedKmh();

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "Front-left wheel speed > 0 km/h";
    test.result.actual =
        "Front-left wheel speed = " + std::to_string(wheelSpeedKmh) + " km/h";

    if (std::isfinite(wheelSpeedKmh) && wheelSpeedKmh > speedToleranceKmh)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Vehicle model provided a valid front-left wheel speed.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Front-left wheel speed was not available as expected.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runFrontRightWheelSpeedStateTest()
{
    TestCase test;

    test.id = "TC_VEH_007";
    test.name = "Front-Right Wheel Speed State";
    test.description =
        "Verify that the vehicle model provides simulated front-right wheel speed.";
    test.requirementId = "VEH-REQ-003";
    test.precondition = "Vehicle simulation shall be reset.";
    test.stimulus = "Execute Acceleration for 1000 ms and read front-right wheel speed.";
    test.expectedResult = "Front-right wheel speed shall be available and greater than zero.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const double wheelSpeedKmh =
        engine.getVehicleModel().getFrontRightWheelSpeedKmh();

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "Front-right wheel speed > 0 km/h";
    test.result.actual =
        "Front-right wheel speed = " + std::to_string(wheelSpeedKmh) + " km/h";

    if (std::isfinite(wheelSpeedKmh) && wheelSpeedKmh > speedToleranceKmh)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Vehicle model provided a valid front-right wheel speed.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Front-right wheel speed was not available as expected.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runBrakeInputStateTest()
{
    TestCase test;

    test.id = "TC_VEH_008";
    test.name = "Brake Input State";
    test.description =
        "Verify that the vehicle model provides simulated brake input as a percentage.";
    test.requirementId = "VEH-REQ-004";
    test.precondition = "Vehicle shall be moving before Hard Braking.";
    test.stimulus = "Transition to Hard Braking and read brake percentage.";
    test.expectedResult = "Brake input shall be observable as a positive percentage.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 2000.0);
    engine.startScenario(DrivingScenario::Cruise);
    runUntil(engine, 3000.0);
    engine.startScenario(DrivingScenario::HardBraking);
    runUntil(engine, 3200.0);

    const double brakePercent =
        engine.getVehicleModel().getBrakePercent();

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "Brake input > 0%";
    test.result.actual = "Brake input = " + std::to_string(brakePercent) + "%";

    if (std::isfinite(brakePercent) &&
        brakePercent > brakeReleasedTolerancePercent)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Vehicle model exposed the simulated brake input percentage.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Vehicle brake input was not observable as expected.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runSteeringAngleStateTest()
{
    TestCase test;

    test.id = "TC_VEH_009";
    test.name = "Steering Angle State";
    test.description =
        "Verify that the vehicle model provides simulated steering-angle information.";
    test.requirementId = "VEH-REQ-005";
    test.precondition = "Vehicle simulation shall be reset to its defined initial state.";
    test.stimulus = "Read the simulated steering angle.";
    test.expectedResult = "Steering angle shall be observable at the defined 3.2 degree initial value.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();

    const double steeringAngleDeg =
        engine.getVehicleModel().getSteeringAngleDeg();

    constexpr double expectedAngleDeg = 3.2;
    constexpr double toleranceDeg = 0.001;

    test.result.expected = "Steering angle = 3.2 deg";
    test.result.actual =
        "Steering angle = " + std::to_string(steeringAngleDeg) + " deg";

    if (std::fabs(steeringAngleDeg - expectedAngleDeg) <= toleranceDeg)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Vehicle model exposed the defined simulated steering angle.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Vehicle steering angle did not match the defined initial state.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runVehicleDynamicStateUpdateTest()
{
    TestCase test;

    test.id = "TC_VEH_010";
    test.name = "Vehicle Dynamic State Update";
    test.description =
        "Verify that vehicle dynamic state changes as simulation time advances.";
    test.requirementId = "VEH-REQ-006";
    test.precondition = "Vehicle simulation shall be reset.";
    test.stimulus = "Advance the Acceleration scenario from 0 ms to 1000 ms.";
    test.expectedResult =
        "Simulation time shall advance and vehicle speed shall change from its initial value.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();

    const double initialTimeMs = engine.getCurrentTimeMs();
    const double initialSpeedKmh =
        engine.getVehicleModel().getVehicleSpeedKmh();

    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const double finalTimeMs = engine.getCurrentTimeMs();
    const double finalSpeedKmh =
        engine.getVehicleModel().getVehicleSpeedKmh();

    const bool timeAdvanced = finalTimeMs > initialTimeMs;
    const bool stateChanged =
        std::fabs(finalSpeedKmh - initialSpeedKmh) > speedToleranceKmh;

    test.result.executionTimeMs = finalTimeMs;
    test.result.expected = "Time advances and vehicle speed changes";
    test.result.actual =
        "Time=" + std::to_string(initialTimeMs) + "->" +
        std::to_string(finalTimeMs) + " ms, Speed=" +
        std::to_string(initialSpeedKmh) + "->" +
        std::to_string(finalSpeedKmh) + " km/h";

    if (timeAdvanced && stateChanged)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Vehicle dynamic state updated as simulation time advanced.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Vehicle dynamic state did not update as expected.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runIndependentWheelSpeedsTest()
{
    TestCase test;

    test.id = "TC_VEH_011";
    test.name = "Independent Wheel Speeds";
    test.description =
        "Verify that the vehicle model exposes separate front-left and "
        "front-right wheel-speed signals for ABS processing.";
    test.requirementId = "VEH-REQ-009";
    test.precondition = "Vehicle shall be moving.";
    test.stimulus =
        "Run Acceleration and read both front wheel-speed signals independently.";
    test.expectedResult =
        "Both front-left and front-right wheel-speed values shall be independently observable and valid.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const double frontLeftKmh =
        engine.getVehicleModel().getFrontLeftWheelSpeedKmh();
    const double frontRightKmh =
        engine.getVehicleModel().getFrontRightWheelSpeedKmh();

    const bool frontLeftValid =
        std::isfinite(frontLeftKmh) && frontLeftKmh > speedToleranceKmh;
    const bool frontRightValid =
        std::isfinite(frontRightKmh) && frontRightKmh > speedToleranceKmh;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "Front-left valid=true, Front-right valid=true";
    test.result.actual =
        "Front-left=" + std::to_string(frontLeftKmh) +
        " km/h, Front-right=" + std::to_string(frontRightKmh) + " km/h";

    if (frontLeftValid && frontRightValid)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Separate front-left and front-right wheel-speed signals were observable.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "One or both independent wheel-speed signals were not valid.";
    }

    return test;
}

