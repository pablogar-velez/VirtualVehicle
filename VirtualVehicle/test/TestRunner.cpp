#include "TestRunner.h"

#include <cmath>
#include <string>
#include <vector>

#include "../simulation/SimulationEngine.h"
#include "../sensor/WheelSpeedSensor.h"
#include "../can/VirtualCanBus.h"
#include "../can/CanMessageDefinitions.h"
#include "../can/VehicleStateCodec.h"
#include "../can/SteeringStateCodec.h"
#include "../can/AbsStateCodec.h"

#include "../ecu/PowertrainEcu.h"
#include "../ecu/SteeringEcu.h"

// ==================================================
// Test configuration
// ==================================================

namespace
{
    constexpr double simulationStepMs =
        10.0;

    constexpr double sensorFaultMaximumResponseTimeMs =
        20.0;

    constexpr double sensorFaultObservationWindowMs =
        500.0;

    constexpr double speedToleranceKmh =
        0.01;

    constexpr double brakeReleasedTolerancePercent =
        0.01;
}

// ==================================================
// Constructor
// ==================================================

TestRunner::TestRunner()
{
}

// ==================================================
// Helper
// Run simulation until target time
// ==================================================

void TestRunner::runUntil(
    SimulationEngine& engine,
    double targetTimeMs,
    double stepMs)
{
    while (
        engine.getCurrentTimeMs() <
        targetTimeMs
        )
    {
        engine.update(
            stepMs
        );
    }
}

// ==================================================
// TC_SYS_001
// Simulation Time Progression
// ==================================================

TestCase TestRunner::runSimulationTimeProgressionTest()
{
    TestCase test;

    test.id =
        "TC_SYS_001";

    test.name =
        "Simulation Time Progression";

    test.description =
        "Verify that VirtualVehicle simulation time "
        "advances when the simulation engine is updated.";

    test.requirementId =
        "SYS-REQ-001";

    test.precondition =
        "Simulation engine shall be initialized and reset.";

    test.stimulus =
        "Advance the simulation by 100 ms.";

    test.expectedResult =
        "Simulation time shall advance from 0 ms to 100 ms.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Simulation
    // ==================================================

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    const double initialTimeMs =
        engine.getCurrentTimeMs();

    runUntil(
        engine,
        100.0
    );

    const double finalTimeMs =
        engine.getCurrentTimeMs();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        finalTimeMs;

    test.result.expected =
        "Simulation time = 100 ms";

    test.result.actual =
        "Simulation time = " +
        std::to_string(
            finalTimeMs
        ) +
        " ms";

    if (
        initialTimeMs == 0.0 &&
        finalTimeMs >= 100.0
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Simulation time progressed correctly.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Simulation time did not progress as expected.";
    }

    return test;
}

// ==================================================
// TC_VEH_001
// Acceleration Scenario
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
// TC_VEH_002
// Cruise Scenario
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
// TC_VEH_003
// Hard Braking Scenario
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
// TC_VEH_004
// Recovery Scenario
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
// TC_ABS_001
// Emergency Braking
// ==================================================

TestCase TestRunner::runAbsEmergencyBrakingTest()
{
    TestCase test;

    test.id =
        "TC_ABS_001";

    test.name =
        "ABS Emergency Braking";

    test.description =
        "Verify that ABS activates during hard braking "
        "when wheel slip is detected.";

    test.requirementId =
        "ABS-REQ-002";

    test.precondition =
        "Vehicle shall be moving before hard braking.";

    test.stimulus =
        "Apply hard braking after acceleration and cruise.";

    test.expectedResult =
        "ABS shall activate when wheel slip is detected.";

    test.result.status =
        TestStatus::Running;

    test.result.maximumAllowedResponseTimeMs =
        0.0;

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

    const double brakingStartTimeMs =
        engine.getCurrentTimeMs();

    engine.startScenario(
        DrivingScenario::HardBraking
    );

    bool absActivated =
        false;

    double activationTimeMs =
        0.0;

    constexpr double observationEndTimeMs =
        4500.0;

    while (
        engine.getCurrentTimeMs() <
        observationEndTimeMs
        )
    {
        engine.update(
            simulationStepMs
        );

        if (
            engine
            .getAbsState()
            .absActive
            )
        {
            absActivated =
                true;

            activationTimeMs =
                engine.getCurrentTimeMs();

            break;
        }
    }

    // ==================================================
    // Result
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS ACTIVE";

    if (absActivated)
    {
        test.result.responseTimeMs =
            activationTimeMs -
            brakingStartTimeMs;

        test.result.status =
            TestStatus::Passed;

        test.result.actual =
            "ABS ACTIVE";

        test.result.message =
            "ABS activated " +
            std::to_string(
                test.result.responseTimeMs
            ) +
            " ms after hard braking started.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "ABS INACTIVE";

        test.result.message =
            "ABS did not activate during emergency braking.";
    }

    return test;
}

// ==================================================
// TC_ABS_002
// Front Left Wheel Sensor Dropout
// ==================================================

TestCase TestRunner::runFrontLeftWheelSensorDropoutTest()
{
    TestCase test;

    test.id =
        "TC_ABS_002";

    test.name =
        "Front Left Wheel Sensor Dropout";

    test.description =
        "Verify that ABS health becomes degraded within "
        "the required response time when the front-left "
        "wheel speed sensor fails.";

    test.requirementId =
        "ABS-REQ-004";

    test.precondition =
        "Vehicle shall be moving with ABS health HEALTHY.";

    test.stimulus =
        "Inject dropout fault into front-left wheel speed sensor.";

    test.expectedResult =
        "ABS health shall transition to DEGRADED "
        "within 20 ms.";

    test.result.status =
        TestStatus::Running;

    test.result.maximumAllowedResponseTimeMs =
        sensorFaultMaximumResponseTimeMs;

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

    // ==================================================
    // Precondition
    // ==================================================

    const bool initialHealthHealthy =
        engine
        .getAbsState()
        .healthStatus ==
        AbsHealthStatus::Healthy;

    if (!initialHealthHealthy)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.executionTimeMs =
            engine.getCurrentTimeMs();

        test.result.expected =
            "ABS HEALTHY before fault injection";

        test.result.actual =
            "ABS not HEALTHY before fault injection";

        test.result.message =
            "Test precondition failed.";

        return test;
    }

    // ==================================================
    // Stimulus
    // ==================================================

    const double faultInjectionTimeMs =
        engine.getCurrentTimeMs();

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    // ==================================================
    // Observe response
    // ==================================================

    bool degradedDetected =
        false;

    double degradedTimeMs =
        0.0;

    const double observationEndTimeMs =
        faultInjectionTimeMs +
        sensorFaultObservationWindowMs;

    while (
        engine.getCurrentTimeMs() <
        observationEndTimeMs
        )
    {
        engine.update(
            simulationStepMs
        );

        if (
            engine
            .getAbsState()
            .healthStatus ==
            AbsHealthStatus::Degraded
            )
        {
            degradedDetected =
                true;

            degradedTimeMs =
                engine.getCurrentTimeMs();

            break;
        }
    }

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS DEGRADED <= 20 ms";

    if (degradedDetected)
    {
        test.result.responseTimeMs =
            degradedTimeMs -
            faultInjectionTimeMs;

        test.result.actual =
            "ABS DEGRADED in " +
            std::to_string(
                test.result.responseTimeMs
            ) +
            " ms";

        if (
            test.result.responseTimeMs <=
            test.result.maximumAllowedResponseTimeMs
            )
        {
            test.result.status =
                TestStatus::Passed;

            test.result.message =
                "Front-left sensor dropout detected and "
                "ABS health transitioned to DEGRADED "
                "within the required 20 ms limit.";
        }
        else
        {
            test.result.status =
                TestStatus::Failed;

            test.result.message =
                "ABS transitioned to DEGRADED, but exceeded "
                "the maximum allowed response time.";
        }
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "ABS did not transition to DEGRADED";

        test.result.message =
            "ABS health did not degrade after "
            "front-left sensor dropout.";
    }

    return test;
}

// ==================================================
// TC_ABS_003
// Front Right Wheel Sensor Dropout
// ==================================================

TestCase TestRunner::runFrontRightWheelSensorDropoutTest()
{
    TestCase test;

    test.id =
        "TC_ABS_003";

    test.name =
        "Front Right Wheel Sensor Dropout";

    test.description =
        "Verify that ABS health becomes degraded within "
        "the required response time when the front-right "
        "wheel speed sensor fails.";

    test.requirementId =
        "ABS-REQ-005";

    test.precondition =
        "Vehicle shall be moving with ABS health HEALTHY.";

    test.stimulus =
        "Inject dropout fault into front-right wheel speed sensor.";

    test.expectedResult =
        "ABS health shall transition to DEGRADED "
        "within 20 ms.";

    test.result.status =
        TestStatus::Running;

    test.result.maximumAllowedResponseTimeMs =
        sensorFaultMaximumResponseTimeMs;

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

    // ==================================================
    // Precondition
    // ==================================================

    const bool initialHealthHealthy =
        engine
        .getAbsState()
        .healthStatus ==
        AbsHealthStatus::Healthy;

    if (!initialHealthHealthy)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.executionTimeMs =
            engine.getCurrentTimeMs();

        test.result.expected =
            "ABS HEALTHY before fault injection";

        test.result.actual =
            "ABS not HEALTHY before fault injection";

        test.result.message =
            "Test precondition failed.";

        return test;
    }

    // ==================================================
    // Stimulus
    // ==================================================

    const double faultInjectionTimeMs =
        engine.getCurrentTimeMs();

    engine.setFrontRightWheelSensorFault(
        SensorFault::Dropout
    );

    // ==================================================
    // Observe response
    // ==================================================

    bool degradedDetected =
        false;

    double degradedTimeMs =
        0.0;

    const double observationEndTimeMs =
        faultInjectionTimeMs +
        sensorFaultObservationWindowMs;

    while (
        engine.getCurrentTimeMs() <
        observationEndTimeMs
        )
    {
        engine.update(
            simulationStepMs
        );

        if (
            engine
            .getAbsState()
            .healthStatus ==
            AbsHealthStatus::Degraded
            )
        {
            degradedDetected =
                true;

            degradedTimeMs =
                engine.getCurrentTimeMs();

            break;
        }
    }

    // ==================================================
    // Evidence
    // ==================================================

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS DEGRADED <= 20 ms";

    if (degradedDetected)
    {
        test.result.responseTimeMs =
            degradedTimeMs -
            faultInjectionTimeMs;

        test.result.actual =
            "ABS DEGRADED in " +
            std::to_string(
                test.result.responseTimeMs
            ) +
            " ms";

        if (
            test.result.responseTimeMs <=
            test.result.maximumAllowedResponseTimeMs
            )
        {
            test.result.status =
                TestStatus::Passed;

            test.result.message =
                "Front-right sensor dropout detected and "
                "ABS health transitioned to DEGRADED "
                "within the required 20 ms limit.";
        }
        else
        {
            test.result.status =
                TestStatus::Failed;

            test.result.message =
                "ABS transitioned to DEGRADED, but exceeded "
                "the maximum allowed response time.";
        }
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "ABS did not transition to DEGRADED";

        test.result.message =
            "ABS health did not degrade after "
            "front-right sensor dropout.";
    }

    return test;
}

// ==================================================
// TC_SNS_001
// Front Left Sensor Dropout Behavior
// ==================================================

TestCase TestRunner::runFrontLeftSensorDropoutTest()
{
    TestCase test;

    test.id =
        "TC_SNS_001";

    test.name =
        "Front Left Sensor Dropout";

    test.description =
        "Verify that a wheel-speed sensor configured "
        "with a Dropout fault produces an invalid reading.";

    test.requirementId =
        "SNS-REQ-005";

    test.precondition =
        "Front-left wheel-speed sensor shall have "
        "a valid physical wheel speed.";

    test.stimulus =
        "Inject SensorFault::Dropout.";

    test.expectedResult =
        "Sensor reading shall be invalid and report 0 km/h.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Sensor setup
    // ==================================================

    WheelSpeedSensor sensor;

    constexpr float physicalSpeedKmh =
        50.0f;

    sensor.setTrueSpeed(
        physicalSpeedKmh
    );

    const WheelSpeedReading normalReading =
        sensor.readSpeed();

    // ==================================================
    // Precondition
    // ==================================================

    if (
        !normalReading.valid ||
        normalReading.speedKmh !=
        physicalSpeedKmh
        )
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Valid sensor reading before fault";

        test.result.actual =
            "Invalid sensor precondition";

        test.result.message =
            "Sensor did not provide the expected "
            "valid reading before fault injection.";

        return test;
    }

    // ==================================================
    // Fault injection
    // ==================================================

    sensor.setFault(
        SensorFault::Dropout
    );

    const WheelSpeedReading dropoutReading =
        sensor.readSpeed();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.expected =
        "valid = false, speed = 0 km/h";

    test.result.actual =
        "valid = " +
        std::string(
            dropoutReading.valid
            ? "true"
            : "false"
        ) +
        ", speed = " +
        std::to_string(
            dropoutReading.speedKmh
        ) +
        " km/h";

    const bool faultConfigured =
        sensor.getFault() ==
        SensorFault::Dropout;

    const bool readingInvalid =
        !dropoutReading.valid;

    const bool speedZero =
        dropoutReading.speedKmh ==
        0.0f;

    if (
        faultConfigured &&
        readingInvalid &&
        speedZero
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Dropout fault produced the expected "
            "invalid wheel-speed measurement.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Dropout behavior did not match "
            "the sensor requirement.";
    }

    return test;
}

// ==================================================
// TC_SNS_002
// Front Right Sensor Dropout Behavior
// ==================================================

TestCase TestRunner::runFrontRightSensorDropoutTest()
{
    TestCase test;

    test.id =
        "TC_SNS_002";

    test.name =
        "Front Right Sensor Dropout";

    test.description =
        "Verify that the front-right wheel-speed sensor "
        "supports Dropout fault injection and produces "
        "an invalid measurement.";

    test.requirementId =
        "SNS-REQ-004";

    test.precondition =
        "Front-right wheel-speed sensor shall provide "
        "a valid measurement before fault injection.";

    test.stimulus =
        "Inject SensorFault::Dropout.";

    test.expectedResult =
        "Sensor shall enter Dropout state and "
        "produce an invalid measurement.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Sensor setup
    // ==================================================

    WheelSpeedSensor sensor;

    constexpr float physicalSpeedKmh =
        65.0f;

    sensor.setTrueSpeed(
        physicalSpeedKmh
    );

    const WheelSpeedReading normalReading =
        sensor.readSpeed();

    // ==================================================
    // Precondition
    // ==================================================

    if (!normalReading.valid)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Valid reading before fault";

        test.result.actual =
            "Invalid reading before fault";

        test.result.message =
            "Sensor precondition failed.";

        return test;
    }

    // ==================================================
    // Fault injection
    // ==================================================

    sensor.setFault(
        SensorFault::Dropout
    );

    const WheelSpeedReading dropoutReading =
        sensor.readSpeed();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.expected =
        "Fault = Dropout, valid = false";

    test.result.actual =
        "Fault configured = " +
        std::string(
            sensor.getFault() ==
            SensorFault::Dropout
            ? "Dropout"
            : "Other"
        ) +
        ", valid = " +
        std::string(
            dropoutReading.valid
            ? "true"
            : "false"
        );

    if (
        sensor.getFault() ==
        SensorFault::Dropout &&
        !dropoutReading.valid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-right Dropout fault injection "
            "behaved as expected.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-right Dropout fault injection failed.";
    }

    return test;
}

// ==================================================
// TC_SNS_003
// Sensor Fault Recovery
// ==================================================

TestCase TestRunner::runSensorFaultRecoveryTest()
{
    TestCase test;

    test.id =
        "TC_SNS_003";

    test.name =
        "Sensor Fault Recovery";

    test.description =
        "Verify that clearing an injected wheel-speed "
        "sensor fault restores normal measurement behavior.";

    test.requirementId =
        "SNS-REQ-006";

    test.precondition =
        "Wheel-speed sensor shall contain an active "
        "Dropout fault.";

    test.stimulus =
        "Clear the injected sensor fault.";

    test.expectedResult =
        "Sensor shall return to SensorFault::None "
        "and provide a valid wheel-speed reading.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Sensor setup
    // ==================================================

    WheelSpeedSensor sensor;

    constexpr float physicalSpeedKmh =
        72.5f;

    sensor.setTrueSpeed(
        physicalSpeedKmh
    );

    sensor.readSpeed();

    sensor.setFault(
        SensorFault::Dropout
    );

    const WheelSpeedReading faultyReading =
        sensor.readSpeed();

    // ==================================================
    // Verify fault precondition
    // ==================================================

    if (
        faultyReading.valid ||
        sensor.getFault() !=
        SensorFault::Dropout
        )
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Active Dropout fault";

        test.result.actual =
            "Dropout precondition not established";

        test.result.message =
            "Could not establish sensor fault "
            "before recovery verification.";

        return test;
    }

    // ==================================================
    // Clear fault
    // ==================================================

    sensor.clearFault();

    const WheelSpeedReading recoveredReading =
        sensor.readSpeed();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.expected =
        "Fault = None, valid = true, speed = 72.5 km/h";

    test.result.actual =
        "Fault = " +
        std::string(
            sensor.getFault() ==
            SensorFault::None
            ? "None"
            : "Active"
        ) +
        ", valid = " +
        std::string(
            recoveredReading.valid
            ? "true"
            : "false"
        ) +
        ", speed = " +
        std::to_string(
            recoveredReading.speedKmh
        ) +
        " km/h";

    const bool faultCleared =
        sensor.getFault() ==
        SensorFault::None;

    const bool readingValid =
        recoveredReading.valid;

    const bool speedRecovered =
        recoveredReading.speedKmh ==
        physicalSpeedKmh;

    if (
        faultCleared &&
        readingValid &&
        speedRecovered
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Sensor returned to normal measurement "
            "behavior after fault clearing.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Sensor did not recover correctly "
            "after clearing the fault.";
    }

    return test;
}

// ==================================================
// Run test by ID
// ==================================================

TestCase TestRunner::runTest(
    const std::string& testId)
{
    // ==================================================
    // System
    // ==================================================

    if (testId == "TC_SYS_001")
    {
        return runSimulationTimeProgressionTest();
    }

    // ==================================================
    // Vehicle / Scenarios
    // ==================================================

    if (testId == "TC_VEH_001")
    {
        return runAccelerationScenarioTest();
    }

    if (testId == "TC_VEH_002")
    {
        return runCruiseScenarioTest();
    }

    if (testId == "TC_VEH_003")
    {
        return runHardBrakingScenarioTest();
    }

    if (testId == "TC_VEH_004")
    {
        return runRecoveryScenarioTest();
    }

    // ==================================================
    // ABS
    // ==================================================

    if (testId == "TC_ABS_001")
    {
        return runAbsEmergencyBrakingTest();
    }

    if (testId == "TC_ABS_002")
    {
        return runFrontLeftWheelSensorDropoutTest();
    }

    if (testId == "TC_ABS_003")
    {
        return runFrontRightWheelSensorDropoutTest();
    }

    // ==================================================
    // Sensors
    // ==================================================

    if (testId == "TC_SNS_001")
    {
        return runFrontLeftSensorDropoutTest();
    }

    if (testId == "TC_SNS_002")
    {
        return runFrontRightSensorDropoutTest();
    }

    if (testId == "TC_SNS_003")
    {
        return runSensorFaultRecoveryTest();
    }

    // ==================================================
    // Powertrain
    // ==================================================

    if (testId == "TC_PT_001")
    {
        return runPowertrainCanIdentifierTest();
    }

    if (testId == "TC_PT_002")
    {
        return runPowertrainCodecRoundtripTest();
    }

    // ==================================================
    // Steering
    // ==================================================

    if (testId == "TC_STR_001")
    {
        return runSteeringCanIdentifierTest();
    }

    if (testId == "TC_STR_002")
    {
        return runSteeringCodecRoundtripTest();
    }

    // ==================================================
    // CAN
    // ==================================================

    if (testId == "TC_CAN_001")
    {
        return runCanArbitrationPriorityTest();
    }

    if (testId == "TC_CAN_002")
    {
        return runCanBitrateTest();
    }

    if (testId == "TC_CAN_003")
    {
        return runCanTraceRecordingTest();
    }

    if (testId == "TC_CAN_004")
    {
        return runCanArbitrationStatisticsTest();
    }

    if (testId == "TC_CAN_005")
    {
        return runAbsMessagePeriodicityTest();
    }

    if (testId == "TC_CAN_006")
    {
        return runPowertrainMessagePeriodicityTest();
    }

    if (testId == "TC_CAN_007")
    {
        return runSteeringMessagePeriodicityTest();
    }

    if (testId == "TC_CAN_008")
    {
        return runCanBusUtilizationTest();
    }

    if (testId == "TC_CAN_009")
    {
        return runCanTraceTimingEvidenceTest();
    }

    // ==================================================
    // Messages
    // ==================================================

    if (testId == "TC_MSG_001")
    {
        return runAbsCodecRoundtripTest();
    }

    // ==================================================
    // Unknown test
    // ==================================================

    TestCase test;

    test.id =
        testId;

    test.name =
        "Unknown Test";

    test.description =
        "The requested test ID is not registered.";

    test.result.status =
        TestStatus::Failed;

    test.result.expected =
        "Valid registered test ID";

    test.result.actual =
        "Unknown test ID";

    test.result.message =
        "TestRunner could not find test: " +
        testId;

    return test;
}

// ==================================================
// TC_PT_001
// Powertrain CAN Identifier
// ==================================================

TestCase TestRunner::runPowertrainCanIdentifierTest()
{
    TestCase test;

    test.id =
        "TC_PT_001";

    test.name =
        "Powertrain CAN Identifier";

    test.description =
        "Verify that the Powertrain ECU transmits "
        "POWERTRAIN_STATE using CAN ID 0x100.";

    test.requirementId =
        "PT-REQ-007";

    test.precondition =
        "Powertrain ECU shall contain a valid vehicle state.";

    test.stimulus =
        "Request transmission of Powertrain State.";

    test.expectedResult =
        "Transmitted CAN frame shall use identifier 0x100.";

    test.result.status =
        TestStatus::Running;

    VehicleState state{};

    state.speedKmh =
        80.0f;

    state.engineRpm =
        2500;

    state.gear =
        4;

    state.brakeApplied =
        false;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    PowertrainEcu ecu;

    ecu.setVehicleState(
        state
    );

    ecu.transmitVehicleState(
        bus,
        0.0
    );

    const CanFrame frame =
        bus.receive(
            0.0
        );

    test.result.expected =
        "CAN ID = 0x100";

    test.result.actual =
        frame.arbitrationId ==
        0x100
        ? "CAN ID = 0x100"
        : "Unexpected CAN ID";

    if (
        frame.arbitrationId ==
        CanMessageDefinitions::POWERTRAIN_STATE.id
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Powertrain ECU transmitted POWERTRAIN_STATE "
            "using CAN ID 0x100.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Powertrain ECU used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================
// TC_PT_002
// Powertrain Codec Roundtrip
// ==================================================

TestCase TestRunner::runPowertrainCodecRoundtripTest()
{
    TestCase test;

    test.id =
        "TC_PT_002";

    test.name =
        "Powertrain Codec Roundtrip";

    test.description =
        "Verify that POWERTRAIN_STATE encode/decode "
        "preserves the defined vehicle-state signals.";

    test.requirementId =
        "MSG-PT-REQ-006";

    test.precondition =
        "VehicleState shall contain valid test values.";

    test.stimulus =
        "Encode and decode the VehicleState.";

    test.expectedResult =
        "Decoded signals shall match the encoded values "
        "within message resolution.";

    test.result.status =
        TestStatus::Running;

    VehicleState original{};

    original.speedKmh =
        82.5f;

    original.engineRpm =
        2140;

    original.gear =
        3;

    original.brakeApplied =
        true;

    const CanFrame frame =
        VehicleStateCodec::encode(
            original
        );

    const VehicleState decoded =
        VehicleStateCodec::decode(
            frame
        );

    const bool speedMatches =
        decoded.speedKmh ==
        original.speedKmh;

    const bool rpmMatches =
        decoded.engineRpm ==
        original.engineRpm;

    const bool gearMatches =
        decoded.gear ==
        original.gear;

    const bool brakeMatches =
        decoded.brakeApplied ==
        original.brakeApplied;

    test.result.expected =
        "Speed=82.5, RPM=2140, Gear=3, Brake=true";

    test.result.actual =
        "Speed=" +
        std::to_string(
            decoded.speedKmh
        ) +
        ", RPM=" +
        std::to_string(
            decoded.engineRpm
        ) +
        ", Gear=" +
        std::to_string(
            static_cast<int>(
                decoded.gear
                )
        ) +
        ", Brake=" +
        std::string(
            decoded.brakeApplied
            ? "true"
            : "false"
        );

    if (
        speedMatches &&
        rpmMatches &&
        gearMatches &&
        brakeMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "POWERTRAIN_STATE codec preserved "
            "all defined signals.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "POWERTRAIN_STATE codec roundtrip mismatch.";
    }

    return test;
}

// ==================================================
// TC_STR_001
// Steering CAN Identifier
// ==================================================

TestCase TestRunner::runSteeringCanIdentifierTest()
{
    TestCase test;

    test.id =
        "TC_STR_001";

    test.name =
        "Steering CAN Identifier";

    test.description =
        "Verify that the Steering ECU transmits "
        "STEERING_STATE using CAN ID 0x120.";

    test.requirementId =
        "STR-REQ-004";

    test.precondition =
        "Steering ECU shall contain a valid steering state.";

    test.stimulus =
        "Request Steering State transmission.";

    test.expectedResult =
        "Transmitted frame shall use CAN ID 0x120.";

    test.result.status =
        TestStatus::Running;

    SteeringState state{};

    state.steeringAngleDeg =
        12.5f;

    state.steeringTorqueNm =
        1.75f;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    SteeringEcu ecu;

    ecu.setSteeringState(
        state
    );

    ecu.transmitSteeringState(
        bus,
        0.0
    );

    const CanFrame frame =
        bus.receive(
            0.0
        );

    test.result.expected =
        "CAN ID = 0x120";

    test.result.actual =
        frame.arbitrationId ==
        0x120
        ? "CAN ID = 0x120"
        : "Unexpected CAN ID";

    if (
        frame.arbitrationId ==
        CanMessageDefinitions::STEERING_STATE.id
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Steering ECU transmitted STEERING_STATE "
            "using CAN ID 0x120.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Steering ECU used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================
// TC_STR_002
// Steering Codec Roundtrip
// ==================================================

TestCase TestRunner::runSteeringCodecRoundtripTest()
{
    TestCase test;

    test.id =
        "TC_STR_002";

    test.name =
        "Steering Codec Roundtrip";

    test.description =
        "Verify that STEERING_STATE encode/decode "
        "preserves steering angle and torque.";

    test.requirementId =
        "MSG-STR-REQ-003";

    test.precondition =
        "SteeringState shall contain valid test values.";

    test.stimulus =
        "Encode and decode the SteeringState.";

    test.expectedResult =
        "Decoded angle and torque shall match "
        "the encoded values.";

    test.result.status =
        TestStatus::Running;

    SteeringState original{};

    original.steeringAngleDeg =
        -15.3f;

    original.steeringTorqueNm =
        2.45f;

    const CanFrame frame =
        SteeringStateCodec::encode(
            original
        );

    const SteeringState decoded =
        SteeringStateCodec::decode(
            frame
        );

    const bool angleMatches =
        decoded.steeringAngleDeg ==
        original.steeringAngleDeg;

    const bool torqueMatches =
        decoded.steeringTorqueNm ==
        original.steeringTorqueNm;

    test.result.expected =
        "Angle=-15.3 deg, Torque=2.45 Nm";

    test.result.actual =
        "Angle=" +
        std::to_string(
            decoded.steeringAngleDeg
        ) +
        " deg, Torque=" +
        std::to_string(
            decoded.steeringTorqueNm
        ) +
        " Nm";

    if (
        angleMatches &&
        torqueMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "STEERING_STATE codec preserved "
            "the defined signals.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "STEERING_STATE codec roundtrip mismatch.";
    }

    return test;
}

// ==================================================
// TC_CAN_001
// CAN Arbitration Priority
// ==================================================

TestCase TestRunner::runCanArbitrationPriorityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_001";

    test.name =
        "CAN Arbitration Priority";

    test.description =
        "Verify that the CAN frame with the numerically "
        "lowest identifier wins arbitration.";

    test.requirementId =
        "CAN-REQ-003";

    test.precondition =
        "Multiple frames shall be pending at the same time.";

    test.stimulus =
        "Queue CAN IDs 0x120, 0x100 and 0x080.";

    test.expectedResult =
        "CAN ID 0x080 shall win arbitration.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame steeringFrame{};
    steeringFrame.arbitrationId =
        0x120;
    steeringFrame.dlc =
        4;

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId =
        0x100;
    powertrainFrame.dlc =
        6;

    CanFrame absFrame{};
    absFrame.arbitrationId =
        0x080;
    absFrame.dlc =
        5;

    bus.transmit(
        steeringFrame,
        0.0
    );

    bus.transmit(
        powertrainFrame,
        0.0
    );

    bus.transmit(
        absFrame,
        0.0
    );

    const CanFrame winner =
        bus.receive(
            0.0
        );

    test.result.expected =
        "Winner = 0x080";

    test.result.actual =
        winner.arbitrationId ==
        0x080
        ? "Winner = 0x080"
        : "Unexpected arbitration winner";

    if (
        winner.arbitrationId ==
        CanMessageDefinitions::ABS_WHEEL_STATE.id
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Lowest CAN identifier correctly won arbitration.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN arbitration priority was incorrect.";
    }

    return test;
}

// ==================================================
// TC_CAN_002
// CAN Bitrate
// ==================================================

TestCase TestRunner::runCanBitrateTest()
{
    TestCase test;

    test.id =
        "TC_CAN_002";

    test.name =
        "CAN Bitrate";

    test.description =
        "Verify that the Virtual CAN Bus operates "
        "at the configured 500 kbit/s bitrate.";

    test.requirementId =
        "CAN-REQ-001";

    test.precondition =
        "Virtual CAN Bus shall be constructed.";

    test.stimulus =
        "Read configured CAN bitrate.";

    test.expectedResult =
        "Bitrate shall equal 500000 bit/s.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    const std::uint32_t bitrate =
        bus.getBitrate();

    test.result.expected =
        "500000 bit/s";

    test.result.actual =
        std::to_string(
            bitrate
        ) +
        " bit/s";

    if (bitrate == 500000)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Virtual CAN Bus bitrate is configured correctly.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Virtual CAN Bus bitrate does not match requirement.";
    }

    return test;
}

// ==================================================
// TC_CAN_003
// CAN Trace Recording
// ==================================================

TestCase TestRunner::runCanTraceRecordingTest()
{
    TestCase test;

    test.id =
        "TC_CAN_003";

    test.name =
        "CAN Trace Recording";

    test.description =
        "Verify that a transmitted CAN frame "
        "is recorded in the CAN trace.";

    test.requirementId =
        "CAN-REQ-008";

    test.precondition =
        "CAN trace shall initially be empty.";

    test.stimulus =
        "Transmit and receive one CAN frame.";

    test.expectedResult =
        "CAN trace shall contain one transmitted frame.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        0x100;

    frame.dlc =
        2;

    frame.data[0] =
        0xAA;

    frame.data[1] =
        0x55;

    bus.transmit(
        frame,
        0.0
    );

    bus.receive(
        0.0
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Trace size = 1, ID = 0x100";

    if (!trace.empty())
    {
        test.result.actual =
            "Trace size = " +
            std::to_string(
                trace.size()
            );

        if (
            trace.size() == 1 &&
            trace[0].arbitrationId == 0x100 &&
            trace[0].data[0] == 0xAA &&
            trace[0].data[1] == 0x55
            )
        {
            test.result.status =
                TestStatus::Passed;

            test.result.message =
                "CAN transmission was recorded correctly.";
        }
        else
        {
            test.result.status =
                TestStatus::Failed;

            test.result.message =
                "CAN trace content did not match transmission.";
        }
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was created.";
    }

    return test;
}

// ==================================================
// TC_CAN_004
// CAN Arbitration Statistics
// ==================================================

TestCase TestRunner::runCanArbitrationStatisticsTest()
{
    TestCase test;

    test.id =
        "TC_CAN_004";

    test.name =
        "CAN Arbitration Statistics";

    test.description =
        "Verify that CAN arbitration events "
        "are recorded in statistics.";

    test.requirementId =
        "CAN-REQ-014";

    test.precondition =
        "At least two CAN frames shall be pending.";

    test.stimulus =
        "Request transmission of multiple frames.";

    test.expectedResult =
        "Arbitration counter shall increment.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frameA{};
    frameA.arbitrationId =
        0x120;
    frameA.dlc =
        1;

    CanFrame frameB{};
    frameB.arbitrationId =
        0x080;
    frameB.dlc =
        1;

    bus.transmit(
        frameA,
        0.0
    );

    bus.transmit(
        frameB,
        0.0
    );

    bus.receive(
        0.0
    );

    const std::uint64_t arbitrationCount =
        bus
        .getStatistics()
        .getArbitrationCount();

    test.result.expected =
        "Arbitration count >= 1";

    test.result.actual =
        "Arbitration count = " +
        std::to_string(
            arbitrationCount
        );

    if (arbitrationCount >= 1)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN arbitration event was recorded.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN arbitration statistics were not updated.";
    }

    return test;
}

// ==================================================
// TC_MSG_001
// ABS Codec Roundtrip
// ==================================================

TestCase TestRunner::runAbsCodecRoundtripTest()
{
    TestCase test;

    test.id =
        "TC_MSG_001";

    test.name =
        "ABS Codec Roundtrip";

    test.description =
        "Verify that ABS_WHEEL_STATE encode/decode "
        "preserves wheel speeds and ABS status.";

    test.requirementId =
        "MSG-ABS-REQ-003";

    test.precondition =
        "ABS state shall contain valid signal values.";

    test.stimulus =
        "Encode and decode ABS state.";

    test.expectedResult =
        "Decoded ABS state shall match encoded state.";

    test.result.status =
        TestStatus::Running;

    AbsState original{};

    original.wheelSpeedFrontLeft =
        58.4f;

    original.wheelSpeedFrontRight =
        59.1f;

    original.absActive =
        true;

    original.frontLeftSensorValid =
        true;

    original.frontRightSensorValid =
        false;

    original.healthStatus =
        AbsHealthStatus::Degraded;

    const CanFrame frame =
        AbsStateCodec::encode(
            original
        );

    const AbsState decoded =
        AbsStateCodec::decode(
            frame
        );

    const bool frontLeftMatches =
        decoded.wheelSpeedFrontLeft ==
        original.wheelSpeedFrontLeft;

    const bool frontRightMatches =
        decoded.wheelSpeedFrontRight ==
        original.wheelSpeedFrontRight;

    const bool absMatches =
        decoded.absActive ==
        original.absActive;

    const bool flValidMatches =
        decoded.frontLeftSensorValid ==
        original.frontLeftSensorValid;

    const bool frValidMatches =
        decoded.frontRightSensorValid ==
        original.frontRightSensorValid;

    const bool healthMatches =
        decoded.healthStatus ==
        original.healthStatus;

    test.result.expected =
        "ABS signals preserved";

    test.result.actual =
        "FL=" +
        std::to_string(
            decoded.wheelSpeedFrontLeft
        ) +
        ", FR=" +
        std::to_string(
            decoded.wheelSpeedFrontRight
        ) +
        ", ABS=" +
        std::string(
            decoded.absActive
            ? "true"
            : "false"
        );

    if (
        frontLeftMatches &&
        frontRightMatches &&
        absMatches &&
        flValidMatches &&
        frValidMatches &&
        healthMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "ABS_WHEEL_STATE codec preserved "
            "all defined signals.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "ABS_WHEEL_STATE codec roundtrip mismatch.";
    }

    return test;
}


// ==================================================
// TC_CAN_005
// ABS Message Periodicity
// ==================================================

TestCase TestRunner::runAbsMessagePeriodicityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_005";

    test.name =
        "ABS Message Periodicity";

    test.description =
        "Verify that ABS_WHEEL_STATE transmission requests "
        "occur every 10 ms in simulation time.";

    test.requirementId =
        "ABS-REQ-007";

    test.precondition =
        "Simulation engine shall be reset.";

    test.stimulus =
        "Run the simulation for 100 ms.";

    test.expectedResult =
        "ABS transmission request period shall equal 10 ms.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    const auto& trace =
        engine.getCanTrace();

    bool previousFound =
        false;

    double previousRequestTimeMs =
        0.0;

    bool periodValid =
        true;

    std::size_t frameCount =
        0;

    constexpr double toleranceMs =
        0.001;

    for (const CanTraceEntry& entry : trace)
    {
        if (
            entry.arbitrationId !=
            CanMessageDefinitions::ABS_WHEEL_STATE.id
            )
        {
            continue;
        }

        ++frameCount;

        if (previousFound)
        {
            const double periodMs =
                entry.requestTimeMs -
                previousRequestTimeMs;

            if (
                std::fabs(
                    periodMs -
                    CanMessageDefinitions::ABS_WHEEL_STATE.periodMs
                ) >
                toleranceMs
                )
            {
                periodValid =
                    false;

                break;
            }
        }

        previousRequestTimeMs =
            entry.requestTimeMs;

        previousFound =
            true;
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS request period = 10.000 ms";

    test.result.actual =
        "ABS frames checked = " +
        std::to_string(
            frameCount
        );

    if (
        frameCount >= 2 &&
        periodValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "ABS_WHEEL_STATE periodicity matched the "
            "configured 10 ms period.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "ABS_WHEEL_STATE request periodicity did not "
            "match the configured 10 ms period.";
    }

    return test;
}

// ==================================================
// TC_CAN_006
// Powertrain Message Periodicity
// ==================================================

TestCase TestRunner::runPowertrainMessagePeriodicityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_006";

    test.name =
        "Powertrain Message Periodicity";

    test.description =
        "Verify that POWERTRAIN_STATE transmission requests "
        "occur every 20 ms in simulation time.";

    test.requirementId =
        "PT-REQ-006";

    test.precondition =
        "Simulation engine shall be reset.";

    test.stimulus =
        "Run the simulation for 100 ms.";

    test.expectedResult =
        "Powertrain transmission request period shall equal 20 ms.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    const auto& trace =
        engine.getCanTrace();

    bool previousFound =
        false;

    double previousRequestTimeMs =
        0.0;

    bool periodValid =
        true;

    std::size_t frameCount =
        0;

    constexpr double toleranceMs =
        0.001;

    for (const CanTraceEntry& entry : trace)
    {
        if (
            entry.arbitrationId !=
            CanMessageDefinitions::POWERTRAIN_STATE.id
            )
        {
            continue;
        }

        ++frameCount;

        if (previousFound)
        {
            const double periodMs =
                entry.requestTimeMs -
                previousRequestTimeMs;

            if (
                std::fabs(
                    periodMs -
                    CanMessageDefinitions::POWERTRAIN_STATE.periodMs
                ) >
                toleranceMs
                )
            {
                periodValid =
                    false;

                break;
            }
        }

        previousRequestTimeMs =
            entry.requestTimeMs;

        previousFound =
            true;
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Powertrain request period = 20.000 ms";

    test.result.actual =
        "Powertrain frames checked = " +
        std::to_string(
            frameCount
        );

    if (
        frameCount >= 2 &&
        periodValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "POWERTRAIN_STATE periodicity matched the "
            "configured 20 ms period.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "POWERTRAIN_STATE request periodicity did not "
            "match the configured 20 ms period.";
    }

    return test;
}

// ==================================================
// TC_CAN_007
// Steering Message Periodicity
// ==================================================

TestCase TestRunner::runSteeringMessagePeriodicityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_007";

    test.name =
        "Steering Message Periodicity";

    test.description =
        "Verify that STEERING_STATE transmission requests "
        "occur every 20 ms in simulation time.";

    test.requirementId =
        "STR-REQ-003";

    test.precondition =
        "Simulation engine shall be reset.";

    test.stimulus =
        "Run the simulation for 100 ms.";

    test.expectedResult =
        "Steering transmission request period shall equal 20 ms.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    const auto& trace =
        engine.getCanTrace();

    bool previousFound =
        false;

    double previousRequestTimeMs =
        0.0;

    bool periodValid =
        true;

    std::size_t frameCount =
        0;

    constexpr double toleranceMs =
        0.001;

    for (const CanTraceEntry& entry : trace)
    {
        if (
            entry.arbitrationId !=
            CanMessageDefinitions::STEERING_STATE.id
            )
        {
            continue;
        }

        ++frameCount;

        if (previousFound)
        {
            const double periodMs =
                entry.requestTimeMs -
                previousRequestTimeMs;

            if (
                std::fabs(
                    periodMs -
                    CanMessageDefinitions::STEERING_STATE.periodMs
                ) >
                toleranceMs
                )
            {
                periodValid =
                    false;

                break;
            }
        }

        previousRequestTimeMs =
            entry.requestTimeMs;

        previousFound =
            true;
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Steering request period = 20.000 ms";

    test.result.actual =
        "Steering frames checked = " +
        std::to_string(
            frameCount
        );

    if (
        frameCount >= 2 &&
        periodValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "STEERING_STATE periodicity matched the "
            "configured 20 ms period.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "STEERING_STATE request periodicity did not "
            "match the configured 20 ms period.";
    }

    return test;
}

// ==================================================
// TC_CAN_008
// CAN Bus Utilization
// ==================================================

TestCase TestRunner::runCanBusUtilizationTest()
{
    TestCase test;

    test.id =
        "TC_CAN_008";

    test.name =
        "CAN Bus Utilization";

    test.description =
        "Verify that CAN bus utilization is calculated "
        "from accumulated transmission time and simulation time.";

    test.requirementId =
        "CAN-REQ-018";

    test.precondition =
        "Simulation shall transmit CAN frames.";

    test.stimulus =
        "Run the simulation for 1000 ms.";

    test.expectedResult =
        "Bus utilization shall be greater than 0 and less than 100 percent.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        1000.0
    );

    const double simulationTimeMs =
        engine.getCurrentTimeMs();

    const double utilizationPercent =
        engine
        .getCanStatistics()
        .getBusUtilization(
            simulationTimeMs
        );

    test.result.executionTimeMs =
        simulationTimeMs;

    test.result.expected =
        "0% < utilization < 100%";

    test.result.actual =
        "Utilization = " +
        std::to_string(
            utilizationPercent
        ) +
        "%";

    if (
        utilizationPercent > 0.0 &&
        utilizationPercent < 100.0
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN bus utilization was calculated and "
            "is within the expected physical range.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN bus utilization was outside the "
            "expected range.";
    }

    return test;
}

// ==================================================
// TC_CAN_009
// CAN Trace Timing Evidence
// ==================================================

TestCase TestRunner::runCanTraceTimingEvidenceTest()
{
    TestCase test;

    test.id =
        "TC_CAN_009";

    test.name =
        "CAN Trace Timing Evidence";

    test.description =
        "Verify that CAN trace entries record request, "
        "start, end, waiting, and transmission timing evidence.";

    test.requirementId =
        "CAN-REQ-009";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Queue one CAN frame at 5 ms and transmit it at 7 ms.";

    test.expectedResult =
        "Trace timing fields shall contain consistent values.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    frame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    constexpr double requestTimeMs =
        5.0;

    constexpr double txStartTimeMs =
        7.0;

    bus.transmit(
        frame,
        requestTimeMs
    );

    bus.receive(
        txStartTimeMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "request=5 ms, start=7 ms, wait=2 ms, end>start, txTime>0";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace evidence was generated.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    const bool requestValid =
        std::fabs(
            entry.requestTimeMs -
            requestTimeMs
        ) <
        0.001;

    const bool startValid =
        std::fabs(
            entry.txStartTimeMs -
            txStartTimeMs
        ) <
        0.001;

    const bool waitValid =
        std::fabs(
            entry.waitingTimeMs -
            2.0
        ) <
        0.001;

    const bool transmissionValid =
        entry.transmissionTimeMs >
        0.0;

    const bool endValid =
        entry.txEndTimeMs >
        entry.txStartTimeMs;

    const bool timingConsistent =
        std::fabs(
            entry.txEndTimeMs -
            (
                entry.txStartTimeMs +
                entry.transmissionTimeMs
                )
        ) <
        0.001;

    test.result.actual =
        "Request=" +
        std::to_string(
            entry.requestTimeMs
        ) +
        ", Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        ", End=" +
        std::to_string(
            entry.txEndTimeMs
        ) +
        ", Wait=" +
        std::to_string(
            entry.waitingTimeMs
        ) +
        ", TX=" +
        std::to_string(
            entry.transmissionTimeMs
        ) +
        " ms";

    if (
        requestValid &&
        startValid &&
        waitValid &&
        transmissionValid &&
        endValid &&
        timingConsistent
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace contains consistent transmission "
            "timing evidence.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace timing evidence is incomplete "
            "or inconsistent.";
    }

    return test;
}

// ==================================================
// Run complete test suite
// ==================================================

std::vector<TestCase>
TestRunner::runAll()
{
    std::vector<TestCase> results;

    results.reserve(
        25
    );

    // ==================================================
    // System
    // ==================================================

    results.push_back(
        runSimulationTimeProgressionTest()
    );

    // ==================================================
    // Vehicle / Scenarios
    // ==================================================

    results.push_back(
        runAccelerationScenarioTest()
    );

    results.push_back(
        runCruiseScenarioTest()
    );

    results.push_back(
        runHardBrakingScenarioTest()
    );

    results.push_back(
        runRecoveryScenarioTest()
    );

    // ==================================================
    // ABS
    // ==================================================

    results.push_back(
        runAbsEmergencyBrakingTest()
    );

    results.push_back(
        runFrontLeftWheelSensorDropoutTest()
    );

    results.push_back(
        runFrontRightWheelSensorDropoutTest()
    );

    // ==================================================
// Sensors
// ==================================================

    results.push_back(
        runFrontLeftSensorDropoutTest()
    );

    results.push_back(
        runFrontRightSensorDropoutTest()
    );

    results.push_back(
        runSensorFaultRecoveryTest()
    );

    // ==================================================
// Powertrain
// ==================================================

    results.push_back(
        runPowertrainCanIdentifierTest()
    );

    results.push_back(
        runPowertrainCodecRoundtripTest()
    );

    // ==================================================
    // Steering
    // ==================================================

    results.push_back(
        runSteeringCanIdentifierTest()
    );

    results.push_back(
        runSteeringCodecRoundtripTest()
    );

    // ==================================================
    // CAN
    // ==================================================

    results.push_back(
        runCanArbitrationPriorityTest()
    );

    results.push_back(
        runCanBitrateTest()
    );

    results.push_back(
        runCanTraceRecordingTest()
    );

    results.push_back(
        runCanArbitrationStatisticsTest()
    );

    results.push_back(
        runAbsMessagePeriodicityTest()
    );

    results.push_back(
        runPowertrainMessagePeriodicityTest()
    );

    results.push_back(
        runSteeringMessagePeriodicityTest()
    );

    results.push_back(
        runCanBusUtilizationTest()
    );

    results.push_back(
        runCanTraceTimingEvidenceTest()
    );

    // ==================================================
    // Messages
    // ==================================================

    results.push_back(
        runAbsCodecRoundtripTest()
    );

    return results;
}