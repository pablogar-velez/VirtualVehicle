#include "TestRunner.h"

#include <cmath>
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
// TC_SYS_002
// Simulation Reset
// ==================================================

TestCase TestRunner::runSimulationResetTest()
{
    TestCase test;

    test.id =
        "TC_SYS_002";

    test.name =
        "Simulation Reset";

    test.description =
        "Verify that reset restores the simulation to its "
        "defined initial state and clears runtime evidence.";

    test.requirementId =
        "SYS-REQ-005";

    test.precondition =
        "Simulation shall have advanced from its initial state "
        "and contain runtime activity.";

    test.stimulus =
        "Run the simulation, inject a sensor fault, then call reset().";

    test.expectedResult =
        "Simulation time returns to 0 ms, runtime trace/events are cleared, "
        "vehicle state returns to its initial values, and ABS health is HEALTHY.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        200.0
    );

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        230.0
    );

    const bool preconditionEstablished =
        engine.getCurrentTimeMs() >
        0.0 &&
        !engine.getCanTrace().empty();

    if (!preconditionEstablished)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Simulation activity established before reset";

        test.result.actual =
            "Precondition not established";

        test.result.message =
            "The simulation did not produce sufficient runtime activity "
            "before reset verification.";

        return test;
    }

    engine.reset();

    const VehicleState& vehicleState =
        engine.getVehicleState();

    const SteeringState& steeringState =
        engine.getSteeringState();

    const AbsState& absState =
        engine.getAbsState();

    const bool timeReset =
        engine.getCurrentTimeMs() ==
        0.0;

    const bool traceCleared =
        engine.getCanTrace().empty();

    const bool eventsCleared =
        engine.getEvents().empty();

    const bool vehicleReset =
        vehicleState.speedKmh ==
        0.0f &&
        vehicleState.engineRpm ==
        0 &&
        vehicleState.gear ==
        0 &&
        !vehicleState.brakeApplied;

    const bool steeringReset =
        std::fabs(
            steeringState.steeringAngleDeg -
            3.2f
        ) <=
        0.001f &&
        std::fabs(
            steeringState.steeringTorqueNm -
            1.45f
        ) <=
        0.001f;

    const bool absHealthy =
        absState.healthStatus ==
        AbsHealthStatus::Healthy;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "time=0 ms, trace=0, events=0, vehicle=initial, "
        "steering=initial, ABS=HEALTHY";

    test.result.actual =
        "time=" +
        std::to_string(
            engine.getCurrentTimeMs()
        ) +
        " ms, trace=" +
        std::to_string(
            engine.getCanTrace().size()
        ) +
        ", events=" +
        std::to_string(
            engine.getEvents().size()
        ) +
        ", speed=" +
        std::to_string(
            vehicleState.speedKmh
        ) +
        " km/h, ABS=" +
        std::string(
            absHealthy
            ? "HEALTHY"
            : "NOT HEALTHY"
        );

    if (
        timeReset &&
        traceCleared &&
        eventsCleared &&
        vehicleReset &&
        steeringReset &&
        absHealthy
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Simulation reset restored the defined initial state "
            "and cleared runtime evidence.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "One or more simulation states were not restored "
            "by reset().";
    }

    return test;
}

// ==================================================
// TC_ABS_004
// ABS Healthy State
// ==================================================

TestCase TestRunner::runAbsHealthyStateTest()
{
    TestCase test;

    test.id =
        "TC_ABS_004";

    test.name =
        "ABS Healthy State";

    test.description =
        "Verify that the ABS ECU reports HEALTHY when required "
        "wheel-speed sensor inputs are valid and no monitored "
        "sensor fault is present.";

    test.requirementId =
        "ABS-REQ-003";

    test.precondition =
        "Simulation shall be reset with no injected wheel-speed sensor faults.";

    test.stimulus =
        "Advance the simulation with valid wheel-speed sensor inputs.";

    test.expectedResult =
        "ABS health status shall remain HEALTHY.";

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
        20.0
    );

    const AbsState& absState =
        engine.getAbsState();

    const bool healthy =
        absState.healthStatus ==
        AbsHealthStatus::Healthy;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS health = HEALTHY";

    test.result.actual =
        "ABS health = " +
        std::string(
            healthy
            ? "HEALTHY"
            : "DEGRADED"
        );

    if (healthy)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "ABS ECU remained HEALTHY with valid wheel-speed "
            "sensor inputs and no monitored fault.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "ABS ECU did not report HEALTHY under nominal "
            "sensor conditions.";
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

    if (testId == "TC_SYS_002")
    {
        return runSimulationResetTest();
    }

    if (testId == "TC_SYS_003")
    {
        return runEcuCommunicationTest();
    }

    if (testId == "TC_SYS_004")
    {
        return runValidationIsolationTest();
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

    if (testId == "TC_VEH_005")
    {
        return runVehicleSpeedStateTest();
    }

    if (testId == "TC_VEH_006")
    {
        return runFrontLeftWheelSpeedStateTest();
    }

    if (testId == "TC_VEH_007")
    {
        return runFrontRightWheelSpeedStateTest();
    }

    if (testId == "TC_VEH_008")
    {
        return runBrakeInputStateTest();
    }

    if (testId == "TC_VEH_009")
    {
        return runSteeringAngleStateTest();
    }

    if (testId == "TC_VEH_010")
    {
        return runVehicleDynamicStateUpdateTest();
    }

    if (testId == "TC_VEH_011")
    {
        return runIndependentWheelSpeedsTest();
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

    if (testId == "TC_ABS_004")
    {
        return runAbsHealthyStateTest();
    }

    if (testId == "TC_ABS_005")
    {
        return runAbsInterventionStateObservabilityTest();
    }

    if (testId == "TC_ABS_006")
    {
        return runAbsCanIdentifierTest();
    }

    if (testId == "TC_ABS_007")
    {
        return runAbsWheelSlipProcessingTest();
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

    if (testId == "TC_SNS_004")
    {
        return runFrontLeftWheelSpeedMeasurementTest();
    }

    if (testId == "TC_SNS_005")
    {
        return runFrontRightWheelSpeedMeasurementTest();
    }

    if (testId == "TC_SNS_006")
    {
        return runFrontLeftDropoutInjectionTest();
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

    if (testId == "TC_PT_003")
    {
        return runPowertrainStateTest();
    }

    if (testId == "TC_PT_004")
    {
        return runPowertrainVehicleSpeedTest();
    }

    if (testId == "TC_PT_005")
    {
        return runPowertrainEngineSpeedTest();
    }

    if (testId == "TC_PT_006")
    {
        return runPowertrainGearStateTest();
    }

    if (testId == "TC_PT_007")
    {
        return runPowertrainBrakeStateTest();
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

    if (testId == "TC_STR_003")
    {
        return runSteeringStateTest();
    }

    if (testId == "TC_STR_004")
    {
        return runSteeringAngleTest();
    }

    // ==================================================
    // Diagnostics
    // ==================================================

    if (testId == "TC_DIA_001")
    {
        return runFrontLeftDiagnosticFaultInjectionTest();
    }

    if (testId == "TC_DIA_002")
    {
        return runFrontRightDiagnosticFaultInjectionTest();
    }

    if (testId == "TC_DIA_003")
    {
        return runFrontLeftDiagnosticFaultClearTest();
    }

    if (testId == "TC_DIA_004")
    {
        return runFrontRightDiagnosticFaultClearTest();
    }

    if (testId == "TC_DIA_005")
    {
        return runDiagnosticHealthObservabilityTest();
    }

    if (testId == "TC_DIA_006")
    {
        return runRuntimeFaultInjectionTest();
    }

    if (testId == "TC_DIA_007")
    {
        return runRuntimeFaultRecoveryTest();
    }

    // ==================================================
    // Validation Framework
    // ==================================================

    if (testId == "TC_VAL_001")
    {
        return runIndividualTestExecutionVerification();
    }

    if (testId == "TC_VAL_002")
    {
        return runValidationTestIsolationVerification();
    }

    if (testId == "TC_VAL_003")
    {
        return runTestStatusVerification();
    }

    if (testId == "TC_VAL_004")
    {
        return runExecutionTimeVerification();
    }

    if (testId == "TC_VAL_005")
    {
        return runResponseTimeEvidenceVerification();
    }

    if (testId == "TC_VAL_006")
    {
        return runTimingPassCriteriaVerification();
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

    if (testId == "TC_CAN_010")
    {
        return runCanTransmissionDurationTest();
    }

    if (testId == "TC_CAN_011")
    {
        return runCanWaitingTimeTest();
    }

    if (testId == "TC_CAN_012")
    {
        return runCanFrameCountTest();
    }

    if (testId == "TC_CAN_013")
    {
        return runCanAverageWaitingTimeTest();
    }

    if (testId == "TC_CAN_014")
    {
        return runCanMaximumWaitingTimeTest();
    }

    if (testId == "TC_CAN_015")
    {
        return runCanTotalTransmissionTimeTest();
    }

    if (testId == "TC_CAN_016")
    {
        return runCanPerMessageStatisticsTest();
    }

    if (testId == "TC_CAN_017")
    {
        return runCanPendingFrameBehaviorTest();
    }

    if (testId == "TC_CAN_018")
    {
        return runCanWaitingTimeTraceEvidenceTest();
    }

    if (testId == "TC_CAN_019")
    {
        return runCanExclusiveBusAccessTest();
    }

    if (testId == "TC_CAN_020")
    {
        return runCanTraceIdentifierTest();
    }

    if (testId == "TC_CAN_021")
    {
        return runCanTracePayloadTest();
    }

    if (testId == "TC_CAN_022")
    {
        return runCanMultipleEcuRequestsTest();
    }

    // ==================================================
    // Messages
    // ==================================================

    if (testId == "TC_MSG_001")
    {
        return runAbsCodecRoundtripTest();
    }

    if (testId == "TC_MSG_002")
    {
        return runAbsMessageIdentifierTest();
    }

    if (testId == "TC_MSG_003")
    {
        return runAbsMessageStateInformationTest();
    }

    if (testId == "TC_MSG_004")
    {
        return runPowertrainMessageIdentifierTest();
    }

    if (testId == "TC_MSG_005")
    {
        return runPowertrainMessageVehicleSpeedTest();
    }

    if (testId == "TC_MSG_006")
    {
        return runPowertrainMessageEngineRpmTest();
    }

    if (testId == "TC_MSG_007")
    {
        return runPowertrainMessageGearStateTest();
    }

    if (testId == "TC_MSG_008")
    {
        return runPowertrainMessageBrakeStateTest();
    }

    if (testId == "TC_MSG_009")
    {
        return runSteeringMessageIdentifierTest();
    }

    if (testId == "TC_MSG_010")
    {
        return runSteeringMessageAngleTest();
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
// TC_CAN_010
// CAN Transmission Duration
// ==================================================

TestCase TestRunner::runCanTransmissionDurationTest()
{
    TestCase test;

    test.id =
        "TC_CAN_010";

    test.name =
        "CAN Transmission Duration";

    test.description =
        "Verify that the Virtual CAN Bus calculates frame "
        "transmission duration using the configured CAN timing "
        "model and nominal bitrate.";

    test.requirementId =
        "CAN-REQ-006";

    test.precondition =
        "Virtual CAN Bus shall be configured at 500 kbit/s "
        "and contain a valid CAN frame.";

    test.stimulus =
        "Calculate the expected transmission duration using "
        "CanTiming and transmit the same frame through VirtualCanBus.";

    test.expectedResult =
        "The transmission duration recorded in the CAN trace "
        "shall match the duration calculated by CanTiming.";

    test.result.status =
        TestStatus::Running;

    constexpr std::uint32_t bitrate =
        500000;

    VirtualCanBus bus(
        bitrate,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    frame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    const double expectedTransmissionTimeMs =
        CanTiming::calculateTransmissionTimeMs(
            frame,
            bitrate
        );

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
        "Trace TX duration = CanTiming calculated duration";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No transmitted CAN frame was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    constexpr double toleranceMs =
        0.000001;

    const bool durationValid =
        expectedTransmissionTimeMs >
        0.0 &&
        std::fabs(
            entry.transmissionTimeMs -
            expectedTransmissionTimeMs
        ) <=
        toleranceMs;

    const bool endTimeConsistent =
        std::fabs(
            entry.txEndTimeMs -
            (
                entry.txStartTimeMs +
                entry.transmissionTimeMs
                )
        ) <=
        toleranceMs;

    test.result.actual =
        "Expected TX=" +
        std::to_string(
            expectedTransmissionTimeMs
        ) +
        " ms, Trace TX=" +
        std::to_string(
            entry.transmissionTimeMs
        ) +
        " ms, Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        " ms, End=" +
        std::to_string(
            entry.txEndTimeMs
        ) +
        " ms";

    if (
        durationValid &&
        endTimeConsistent
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN transmission duration matches the configured "
            "CAN timing model and bitrate.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Recorded CAN transmission duration does not match "
            "the expected timing-model result.";
    }

    return test;
}

// ==================================================
// TC_CAN_011
// CAN Waiting Time
// ==================================================

TestCase TestRunner::runCanWaitingTimeTest()
{
    TestCase test;

    test.id =
        "TC_CAN_011";

    test.name =
        "CAN Waiting Time";

    test.description =
        "Verify that the Virtual CAN Bus calculates the time "
        "a CAN frame waits between its transmission request "
        "and actual transmission start.";

    test.requirementId =
        "CAN-REQ-007";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Request one CAN frame at 5 ms and start its "
        "transmission at 7 ms.";

    test.expectedResult =
        "Calculated CAN waiting time shall be 2 ms.";

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

    constexpr double transmissionStartTimeMs =
        7.0;

    constexpr double expectedWaitingTimeMs =
        transmissionStartTimeMs -
        requestTimeMs;

    bus.transmit(
        frame,
        requestTimeMs
    );

    bus.receive(
        transmissionStartTimeMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Waiting time = 2.000 ms";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was available for waiting-time evidence.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    constexpr double toleranceMs =
        0.001;

    const bool requestValid =
        std::fabs(
            entry.requestTimeMs -
            requestTimeMs
        ) <=
        toleranceMs;

    const bool startValid =
        std::fabs(
            entry.txStartTimeMs -
            transmissionStartTimeMs
        ) <=
        toleranceMs;

    const bool waitingTimeValid =
        std::fabs(
            entry.waitingTimeMs -
            expectedWaitingTimeMs
        ) <=
        toleranceMs;

    test.result.responseTimeMs =
        entry.waitingTimeMs;

    test.result.actual =
        "Request=" +
        std::to_string(
            entry.requestTimeMs
        ) +
        " ms, Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        " ms, Waiting=" +
        std::to_string(
            entry.waitingTimeMs
        ) +
        " ms";

    if (
        requestValid &&
        startValid &&
        waitingTimeValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN waiting time was calculated correctly "
            "from request time to transmission start.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN waiting-time calculation did not match "
            "the expected 2 ms delay.";
    }

    return test;
}


// ==================================================
// TC_CAN_012
// CAN Frame Count
// ==================================================

TestCase TestRunner::runCanFrameCountTest()
{
    TestCase test;
    test.id = "TC_CAN_012";
    test.name = "CAN Frame Count";
    test.description =
        "Verify that CAN statistics count each successfully transmitted frame.";
    test.requirementId = "CAN-REQ-013";
    test.precondition =
        "Virtual CAN Bus statistics shall start with zero transmitted frames.";
    test.stimulus =
        "Transmit three CAN frames through the Virtual CAN Bus.";
    test.expectedResult =
        "CAN statistics shall report exactly three transmitted frames.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame frame1{};
    frame1.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    frame1.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame frame2{};
    frame2.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame2.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame frame3{};
    frame3.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    frame3.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(frame1, 0.0);
    bus.receive(0.0);

    bus.transmit(frame2, 1.0);
    bus.receive(1.0);

    bus.transmit(frame3, 2.0);
    bus.receive(2.0);

    const auto frameCount =
        bus.getStatistics().getFramesTransmitted();

    test.result.expected = "Frames transmitted = 3";
    test.result.actual =
        "Frames transmitted = " + std::to_string(frameCount);

    if (frameCount == 3)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN statistics counted all transmitted frames correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN frame count did not match the number of transmitted frames.";
    }

    return test;
}

// ==================================================
// TC_CAN_013
// CAN Average Waiting Time
// ==================================================

TestCase TestRunner::runCanAverageWaitingTimeTest()
{
    TestCase test;
    test.id = "TC_CAN_013";
    test.name = "CAN Average Waiting Time";
    test.description =
        "Verify that CAN statistics calculate the average waiting time across transmitted frames.";
    test.requirementId = "CAN-REQ-015";
    test.precondition =
        "Virtual CAN Bus shall be empty with zero accumulated waiting time.";
    test.stimulus =
        "Transmit two frames with known waiting times of 2 ms and 4 ms.";
    test.expectedResult =
        "Average waiting time shall be 3 ms.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame frameA{};
    frameA.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frameA.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame frameB{};
    frameB.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    frameB.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(frameA, 5.0);
    bus.receive(7.0);

    bus.transmit(frameB, 10.0);
    bus.receive(14.0);

    const double actualAverageMs =
        bus.getStatistics().getAverageWaitingTimeMs();

    constexpr double expectedAverageMs = 3.0;
    constexpr double toleranceMs = 0.001;

    test.result.expected = "Average waiting time = 3.000 ms";
    test.result.actual =
        "Average waiting time = " + std::to_string(actualAverageMs) + " ms";

    if (std::fabs(actualAverageMs - expectedAverageMs) <= toleranceMs)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN average waiting time was calculated correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN average waiting time did not match the expected value.";
    }

    return test;
}

// ==================================================
// TC_CAN_014
// CAN Maximum Waiting Time
// ==================================================

TestCase TestRunner::runCanMaximumWaitingTimeTest()
{
    TestCase test;
    test.id = "TC_CAN_014";
    test.name = "CAN Maximum Waiting Time";
    test.description =
        "Verify that CAN statistics retain the maximum waiting time observed across transmitted frames.";
    test.requirementId = "CAN-REQ-016";
    test.precondition =
        "Virtual CAN Bus statistics shall start with zero maximum waiting time.";
    test.stimulus =
        "Transmit three frames with waiting times of 1 ms, 5 ms, and 3 ms.";
    test.expectedResult =
        "Maximum waiting time shall be 5 ms.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame frame{};
    frame.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    bus.transmit(frame, 1.0);
    bus.receive(2.0);

    bus.transmit(frame, 10.0);
    bus.receive(15.0);

    bus.transmit(frame, 20.0);
    bus.receive(23.0);

    const double actualMaximumMs =
        bus.getStatistics().getMaximumWaitingTimeMs();

    constexpr double expectedMaximumMs = 5.0;
    constexpr double toleranceMs = 0.001;

    test.result.expected = "Maximum waiting time = 5.000 ms";
    test.result.actual =
        "Maximum waiting time = " + std::to_string(actualMaximumMs) + " ms";

    if (std::fabs(actualMaximumMs - expectedMaximumMs) <= toleranceMs)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN maximum waiting time was recorded correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN maximum waiting time did not match the expected value.";
    }

    return test;
}

// ==================================================
// TC_CAN_015
// CAN Total Transmission Time
// ==================================================

TestCase TestRunner::runCanTotalTransmissionTimeTest()
{
    TestCase test;
    test.id = "TC_CAN_015";
    test.name = "CAN Total Transmission Time";
    test.description =
        "Verify that CAN statistics accumulate the total transmission time of all transmitted frames.";
    test.requirementId = "CAN-REQ-017";
    test.precondition =
        "Virtual CAN Bus total transmission time shall initially be zero.";
    test.stimulus =
        "Transmit three frames and compare accumulated transmission time against the sum of CanTiming results.";
    test.expectedResult =
        "Total transmission time shall equal the sum of individual CAN frame transmission durations.";
    test.result.status = TestStatus::Running;

    constexpr std::uint32_t bitrate = 500000;
    VirtualCanBus bus(bitrate, LogLevel::Quiet);

    CanFrame frame1{};
    frame1.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    frame1.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame frame2{};
    frame2.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame2.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame frame3{};
    frame3.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    frame3.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    const double expectedTotalMs =
        CanTiming::calculateTransmissionTimeMs(frame1, bitrate) +
        CanTiming::calculateTransmissionTimeMs(frame2, bitrate) +
        CanTiming::calculateTransmissionTimeMs(frame3, bitrate);

    bus.transmit(frame1, 0.0);
    bus.receive(0.0);

    bus.transmit(frame2, 1.0);
    bus.receive(1.0);

    bus.transmit(frame3, 2.0);
    bus.receive(2.0);

    const double actualTotalMs =
        bus.getStatistics().getTotalTransmissionTimeMs();

    constexpr double toleranceMs = 0.000001;

    test.result.expected =
        "Total TX time = " + std::to_string(expectedTotalMs) + " ms";
    test.result.actual =
        "Total TX time = " + std::to_string(actualTotalMs) + " ms";

    if (std::fabs(actualTotalMs - expectedTotalMs) <= toleranceMs)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN total transmission time accumulated correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN total transmission time did not match the sum of individual frame durations.";
    }

    return test;
}

// ==================================================
// TC_CAN_016
// CAN Per-Message Statistics
// ==================================================

TestCase TestRunner::runCanPerMessageStatisticsTest()
{
    TestCase test;
    test.id = "TC_CAN_016";
    test.name = "CAN Per-Message Statistics";
    test.description =
        "Verify that CAN statistics maintain separate statistics for each CAN message identifier.";
    test.requirementId = "CAN-REQ-019";
    test.precondition =
        "Virtual CAN Bus message statistics shall initially contain no frames.";
    test.stimulus =
        "Transmit two POWERTRAIN_STATE frames and one ABS_WHEEL_STATE frame.";
    test.expectedResult =
        "Per-message statistics shall report Powertrain=2 frames and ABS=1 frame.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame absFrame{};
    absFrame.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    bus.transmit(powertrainFrame, 0.0);
    bus.receive(0.0);

    bus.transmit(absFrame, 1.0);
    bus.receive(1.0);

    bus.transmit(powertrainFrame, 2.0);
    bus.receive(2.0);

    const auto& stats =
        bus.getStatistics().getMessageStatistics();

    const auto ptIt =
        stats.find(CanMessageDefinitions::POWERTRAIN_STATE.id);
    const auto absIt =
        stats.find(CanMessageDefinitions::ABS_WHEEL_STATE.id);

    const bool ptFound = ptIt != stats.end();
    const bool absFound = absIt != stats.end();

    std::uint64_t ptFrames = 0;
    std::uint64_t absFrames = 0;

    if (ptFound)
    {
        ptFrames = ptIt->second.frameCount;
    }

    if (absFound)
    {
        absFrames = absIt->second.frameCount;
    }

    test.result.expected = "0x100 frames=2, 0x080 frames=1";
    test.result.actual =
        "0x100 frames=" + std::to_string(ptFrames) +
        ", 0x080 frames=" + std::to_string(absFrames);

    if (ptFound && absFound && ptFrames == 2 && absFrames == 1)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN per-message frame statistics were separated and counted correctly by arbitration ID.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN per-message statistics did not contain the expected frame counts.";
    }

    return test;
}


// ==================================================
// TC_CAN_017
// CAN Pending Frame Behavior
// ==================================================

TestCase TestRunner::runCanPendingFrameBehaviorTest()
{
    TestCase test;

    test.id =
        "TC_CAN_017";

    test.name =
        "CAN Pending Frame Behavior";

    test.description =
        "Verify that a non-winning CAN frame remains pending "
        "after arbitration and can be transmitted afterward.";

    test.requirementId =
        "CAN-REQ-005";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Queue two frames at the same request time with IDs "
        "0x100 and 0x080, then receive twice.";

    test.expectedResult =
        "0x080 shall transmit first and 0x100 shall remain "
        "pending until the second receive operation.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame absFrame{};
    absFrame.arbitrationId =
        CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc =
        CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    constexpr double requestTimeMs =
        0.0;

    bus.transmit(
        powertrainFrame,
        requestTimeMs
    );

    bus.transmit(
        absFrame,
        requestTimeMs
    );

    const bool pendingBeforeReceive =
        bus.hasFrame();

    const CanFrame firstFrame =
        bus.receive(
            0.0
        );

    const bool pendingAfterFirstReceive =
        bus.hasFrame();

    const CanFrame secondFrame =
        bus.receive(
            bus.getBusyUntilMs()
        );

    const bool pendingAfterSecondReceive =
        bus.hasFrame();

    const bool firstIsAbs =
        firstFrame.arbitrationId ==
        CanMessageDefinitions::ABS_WHEEL_STATE.id;

    const bool secondIsPowertrain =
        secondFrame.arbitrationId ==
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    test.result.expected =
        "Before=true, First=0x080, PendingAfterFirst=true, "
        "Second=0x100, PendingAfterSecond=false";

    test.result.actual =
        "Before=" +
        std::string(
            pendingBeforeReceive
            ? "true"
            : "false"
        ) +
        ", First=" +
        std::string(
            firstIsAbs
            ? "0x080"
            : "unexpected"
        ) +
        ", PendingAfterFirst=" +
        std::string(
            pendingAfterFirstReceive
            ? "true"
            : "false"
        ) +
        ", Second=" +
        std::string(
            secondIsPowertrain
            ? "0x100"
            : "unexpected"
        ) +
        ", PendingAfterSecond=" +
        std::string(
            pendingAfterSecondReceive
            ? "true"
            : "false"
        );

    if (
        pendingBeforeReceive &&
        firstIsAbs &&
        pendingAfterFirstReceive &&
        secondIsPowertrain &&
        !pendingAfterSecondReceive
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Non-winning CAN frame remained pending and "
            "was transmitted on the next receive operation.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN pending-frame behavior did not match "
            "the requirement.";
    }

    return test;
}

// ==================================================
// TC_CAN_018
// CAN Waiting-Time Trace Evidence
// ==================================================

TestCase TestRunner::runCanWaitingTimeTraceEvidenceTest()
{
    TestCase test;

    test.id =
        "TC_CAN_018";

    test.name =
        "CAN Waiting-Time Trace Evidence";

    test.description =
        "Verify that CAN trace entries preserve the calculated "
        "waiting-time evidence for transmitted frames.";

    test.requirementId =
        "CAN-REQ-012";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Request a CAN frame at 4 ms and transmit it at 9 ms.";

    test.expectedResult =
        "The CAN trace shall record a waiting time of 5 ms.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};
    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;
    frame.dlc =
        CanMessageDefinitions::STEERING_STATE.dlc;

    constexpr double requestTimeMs =
        4.0;

    constexpr double txStartTimeMs =
        9.0;

    constexpr double expectedWaitingTimeMs =
        5.0;

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
        "Trace waiting time = 5.000 ms";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    constexpr double toleranceMs =
        0.001;

    const bool waitingTimeMatches =
        std::fabs(
            entry.waitingTimeMs -
            expectedWaitingTimeMs
        ) <=
        toleranceMs;

    const bool requestTimeMatches =
        std::fabs(
            entry.requestTimeMs -
            requestTimeMs
        ) <=
        toleranceMs;

    const bool txStartMatches =
        std::fabs(
            entry.txStartTimeMs -
            txStartTimeMs
        ) <=
        toleranceMs;

    test.result.responseTimeMs =
        entry.waitingTimeMs;

    test.result.actual =
        "Request=" +
        std::to_string(
            entry.requestTimeMs
        ) +
        " ms, Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        " ms, Waiting=" +
        std::to_string(
            entry.waitingTimeMs
        ) +
        " ms";

    if (
        waitingTimeMatches &&
        requestTimeMatches &&
        txStartMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace preserved the expected waiting-time "
            "evidence for the transmitted frame.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace waiting-time evidence did not match "
            "the expected request/start relationship.";
    }

    return test;
}


// ==================================================
// TC_CAN_019
// CAN Exclusive Bus Access
// ==================================================

TestCase TestRunner::runCanExclusiveBusAccessTest()
{
    TestCase test;

    test.id =
        "TC_CAN_019";

    test.name =
        "CAN Exclusive Bus Access";

    test.description =
        "Verify that two CAN frames do not occupy the virtual "
        "CAN bus at the same time.";

    test.requirementId =
        "CAN-REQ-004";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Queue two CAN frames at the same request time and "
        "transmit them sequentially.";

    test.expectedResult =
        "The second frame transmission shall start at or after "
        "the first frame transmission has completed.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame absFrame{};
    absFrame.arbitrationId =
        CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc =
        CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    bus.transmit(
        powertrainFrame,
        0.0
    );

    bus.transmit(
        absFrame,
        0.0
    );

    bus.receive(
        0.0
    );

    const double secondTransmissionStartMs =
        bus.getBusyUntilMs();

    bus.receive(
        secondTransmissionStartMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Second TX start >= First TX end";

    if (trace.size() < 2)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Fewer than two trace entries";

        test.result.message =
            "The test could not observe two sequential CAN transmissions.";

        return test;
    }

    const CanTraceEntry& first =
        trace[0];

    const CanTraceEntry& second =
        trace[1];

    constexpr double toleranceMs =
        0.000001;

    const bool firstDurationValid =
        first.txEndTimeMs >
        first.txStartTimeMs;

    const bool secondDurationValid =
        second.txEndTimeMs >
        second.txStartTimeMs;

    const bool noOverlap =
        second.txStartTimeMs +
        toleranceMs >=
        first.txEndTimeMs;

    test.result.actual =
        "First=[" +
        std::to_string(
            first.txStartTimeMs
        ) +
        ", " +
        std::to_string(
            first.txEndTimeMs
        ) +
        "] ms, Second=[" +
        std::to_string(
            second.txStartTimeMs
        ) +
        ", " +
        std::to_string(
            second.txEndTimeMs
        ) +
        "] ms";

    if (
        firstDurationValid &&
        secondDurationValid &&
        noOverlap
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN transmissions occupied the bus exclusively "
            "without temporal overlap.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN transmissions overlapped or contained invalid "
            "transmission timing.";
    }

    return test;
}

// ==================================================
// TC_CAN_020
// CAN Trace Identifier Evidence
// ==================================================

TestCase TestRunner::runCanTraceIdentifierTest()
{
    TestCase test;

    test.id =
        "TC_CAN_020";

    test.name =
        "CAN Trace Identifier Evidence";

    test.description =
        "Verify that each CAN trace entry records the arbitration "
        "identifier of the transmitted frame.";

    test.requirementId =
        "CAN-REQ-010";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Transmit a STEERING_STATE frame using CAN ID 0x120.";

    test.expectedResult =
        "The CAN trace entry shall contain arbitration ID 0x120.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};
    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;
    frame.dlc =
        CanMessageDefinitions::STEERING_STATE.dlc;

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
        "Trace CAN ID = 0x120";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    const bool identifierMatches =
        entry.arbitrationId ==
        CanMessageDefinitions::STEERING_STATE.id;

    test.result.actual =
        identifierMatches
        ? "Trace CAN ID = 0x120"
        : "Trace CAN ID unexpected";

    if (identifierMatches)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace preserved the transmitted arbitration identifier.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace arbitration identifier did not match "
            "the transmitted frame.";
    }

    return test;
}

// ==================================================
// TC_CAN_021
// CAN Trace Payload Evidence
// ==================================================

TestCase TestRunner::runCanTracePayloadTest()
{
    TestCase test;

    test.id =
        "TC_CAN_021";

    test.name =
        "CAN Trace Payload Evidence";

    test.description =
        "Verify that the CAN trace stores the DLC and payload bytes "
        "of the transmitted CAN frame.";

    test.requirementId =
        "CAN-REQ-011";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Transmit a CAN frame with a known four-byte payload.";

    test.expectedResult =
        "The CAN trace shall contain the same DLC and payload bytes.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;

    frame.dlc =
        4;

    frame.data[0] =
        0x12;

    frame.data[1] =
        0x34;

    frame.data[2] =
        0x56;

    frame.data[3] =
        0x78;

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
        "DLC=4, DATA=12 34 56 78";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    const bool dlcMatches =
        entry.dlc ==
        4;

    const bool payloadMatches =
        entry.data[0] == 0x12 &&
        entry.data[1] == 0x34 &&
        entry.data[2] == 0x56 &&
        entry.data[3] == 0x78;

    test.result.actual =
        "DLC=" +
        std::to_string(
            static_cast<int>(
                entry.dlc
                )
        ) +
        ", DATA=" +
        std::to_string(
            static_cast<int>(
                entry.data[0]
                )
        ) +
        " " +
        std::to_string(
            static_cast<int>(
                entry.data[1]
                )
        ) +
        " " +
        std::to_string(
            static_cast<int>(
                entry.data[2]
                )
        ) +
        " " +
        std::to_string(
            static_cast<int>(
                entry.data[3]
                )
        );

    if (
        dlcMatches &&
        payloadMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace preserved the transmitted DLC and payload bytes.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace payload evidence did not match "
            "the transmitted CAN frame.";
    }

    return test;
}


// ==================================================
// TC_DIA_001
// Front-Left Diagnostic Fault Injection
// ==================================================

TestCase TestRunner::runFrontLeftDiagnosticFaultInjectionTest()
{
    TestCase test;

    test.id = "TC_DIA_001";
    test.name = "Front-Left Diagnostic Fault Injection";
    test.description =
        "Verify that a front-left wheel-speed Dropout fault can be "
        "injected while the simulation is running.";
    test.requirementId = "DIA-REQ-001";
    test.precondition =
        "Simulation shall be running with ABS health HEALTHY.";
    test.stimulus =
        "Inject SensorFault::Dropout into the front-left wheel-speed sensor.";
    test.expectedResult =
        "The running simulation shall accept the fault and the ABS ECU "
        "shall transition to DEGRADED.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 50.0);

    const bool healthyBeforeInjection =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 70.0);

    const bool degradedAfterInjection =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Before=HEALTHY, after FL Dropout=DEGRADED";

    test.result.actual =
        "Before=" +
        std::string(
            healthyBeforeInjection ? "HEALTHY" : "NOT HEALTHY"
        ) +
        ", After=" +
        std::string(
            degradedAfterInjection ? "DEGRADED" : "NOT DEGRADED"
        );

    if (healthyBeforeInjection && degradedAfterInjection)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Front-left Dropout fault was injected successfully "
            "during simulation execution.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Front-left diagnostic fault injection did not produce "
            "the expected ABS health response.";
    }

    return test;
}

// ==================================================
// TC_DIA_002
// Front-Right Diagnostic Fault Injection
// ==================================================

TestCase TestRunner::runFrontRightDiagnosticFaultInjectionTest()
{
    TestCase test;

    test.id = "TC_DIA_002";
    test.name = "Front-Right Diagnostic Fault Injection";
    test.description =
        "Verify that a front-right wheel-speed Dropout fault can be "
        "injected while the simulation is running.";
    test.requirementId = "DIA-REQ-002";
    test.precondition =
        "Simulation shall be running with ABS health HEALTHY.";
    test.stimulus =
        "Inject SensorFault::Dropout into the front-right wheel-speed sensor.";
    test.expectedResult =
        "The running simulation shall accept the fault and the ABS ECU "
        "shall transition to DEGRADED.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 50.0);

    const bool healthyBeforeInjection =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    engine.setFrontRightWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 70.0);

    const bool degradedAfterInjection =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Before=HEALTHY, after FR Dropout=DEGRADED";

    test.result.actual =
        "Before=" +
        std::string(
            healthyBeforeInjection ? "HEALTHY" : "NOT HEALTHY"
        ) +
        ", After=" +
        std::string(
            degradedAfterInjection ? "DEGRADED" : "NOT DEGRADED"
        );

    if (healthyBeforeInjection && degradedAfterInjection)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Front-right Dropout fault was injected successfully "
            "during simulation execution.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Front-right diagnostic fault injection did not produce "
            "the expected ABS health response.";
    }

    return test;
}

// ==================================================
// TC_DIA_003
// Front-Left Diagnostic Fault Clearing
// ==================================================

TestCase TestRunner::runFrontLeftDiagnosticFaultClearTest()
{
    TestCase test;

    test.id = "TC_DIA_003";
    test.name = "Front-Left Diagnostic Fault Clearing";
    test.description =
        "Verify that an injected front-left wheel-speed sensor fault "
        "can be cleared.";
    test.requirementId = "DIA-REQ-003";
    test.precondition =
        "Front-left Dropout shall be active and ABS health shall be DEGRADED.";
    test.stimulus =
        "Clear the front-left wheel-speed sensor fault.";
    test.expectedResult =
        "ABS health shall recover to HEALTHY without resetting the simulation.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 50.0);

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 70.0);

    const bool degradedBeforeClear =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    engine.clearFrontLeftWheelSensorFault();

    runUntil(engine, 90.0);

    const bool healthyAfterClear =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Before clear=DEGRADED, after clear=HEALTHY";

    test.result.actual =
        "Before clear=" +
        std::string(
            degradedBeforeClear ? "DEGRADED" : "NOT DEGRADED"
        ) +
        ", After clear=" +
        std::string(
            healthyAfterClear ? "HEALTHY" : "NOT HEALTHY"
        );

    if (degradedBeforeClear && healthyAfterClear)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Front-left sensor fault was cleared and ABS health recovered.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Front-left diagnostic fault clearing did not restore "
            "the expected ABS health state.";
    }

    return test;
}

// ==================================================
// TC_DIA_004
// Front-Right Diagnostic Fault Clearing
// ==================================================

TestCase TestRunner::runFrontRightDiagnosticFaultClearTest()
{
    TestCase test;

    test.id = "TC_DIA_004";
    test.name = "Front-Right Diagnostic Fault Clearing";
    test.description =
        "Verify that an injected front-right wheel-speed sensor fault "
        "can be cleared.";
    test.requirementId = "DIA-REQ-004";
    test.precondition =
        "Front-right Dropout shall be active and ABS health shall be DEGRADED.";
    test.stimulus =
        "Clear the front-right wheel-speed sensor fault.";
    test.expectedResult =
        "ABS health shall recover to HEALTHY without resetting the simulation.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 50.0);

    engine.setFrontRightWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 70.0);

    const bool degradedBeforeClear =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    engine.clearFrontRightWheelSensorFault();

    runUntil(engine, 90.0);

    const bool healthyAfterClear =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Before clear=DEGRADED, after clear=HEALTHY";

    test.result.actual =
        "Before clear=" +
        std::string(
            degradedBeforeClear ? "DEGRADED" : "NOT DEGRADED"
        ) +
        ", After clear=" +
        std::string(
            healthyAfterClear ? "HEALTHY" : "NOT HEALTHY"
        );

    if (degradedBeforeClear && healthyAfterClear)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Front-right sensor fault was cleared and ABS health recovered.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Front-right diagnostic fault clearing did not restore "
            "the expected ABS health state.";
    }

    return test;
}

// ==================================================
// TC_DIA_005
// ECU Health Observability
// ==================================================

TestCase TestRunner::runDiagnosticHealthObservabilityTest()
{
    TestCase test;

    test.id = "TC_DIA_005";
    test.name = "ECU Health Observability";
    test.description =
        "Verify that the effect of a monitored sensor fault is observable "
        "through the ABS ECU health state.";
    test.requirementId = "DIA-REQ-005";
    test.precondition =
        "Simulation shall be running with no wheel-speed sensor fault.";
    test.stimulus =
        "Observe ABS health before and after a front-left Dropout fault.";
    test.expectedResult =
        "ABS health shall be observable as HEALTHY before the fault and "
        "DEGRADED after the fault.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 40.0);

    const AbsHealthStatus healthBefore =
        engine.getAbsState().healthStatus;

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 60.0);

    const AbsHealthStatus healthAfter =
        engine.getAbsState().healthStatus;

    const bool healthyObservable =
        healthBefore ==
        AbsHealthStatus::Healthy;

    const bool degradedObservable =
        healthAfter ==
        AbsHealthStatus::Degraded;

    test.result.expected =
        "Observable health transition: HEALTHY -> DEGRADED";

    test.result.actual =
        std::string(
            healthyObservable ? "HEALTHY" : "NOT HEALTHY"
        ) +
        " -> " +
        std::string(
            degradedObservable ? "DEGRADED" : "NOT DEGRADED"
        );

    if (healthyObservable && degradedObservable)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS ECU health state exposed the effect of the monitored "
            "sensor fault.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "ABS ECU health state did not provide the expected "
            "diagnostic observability.";
    }

    return test;
}

// ==================================================
// TC_DIA_006
// Runtime Fault Injection
// ==================================================

TestCase TestRunner::runRuntimeFaultInjectionTest()
{
    TestCase test;

    test.id = "TC_DIA_006";
    test.name = "Runtime Fault Injection";
    test.description =
        "Verify that a supported sensor fault can be injected after "
        "the simulation has already advanced, without restarting it.";
    test.requirementId = "DIA-REQ-006";
    test.precondition =
        "Simulation shall have advanced beyond its initial time.";
    test.stimulus =
        "Inject a front-left Dropout at runtime and continue the same simulation.";
    test.expectedResult =
        "Simulation time shall continue forward and ABS health shall become DEGRADED.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 100.0);

    const double timeBeforeInjection =
        engine.getCurrentTimeMs();

    const bool runtimeEstablished =
        timeBeforeInjection >
        0.0;

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 120.0);

    const double timeAfterInjection =
        engine.getCurrentTimeMs();

    const bool timeContinued =
        timeAfterInjection >
        timeBeforeInjection;

    const bool degraded =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    test.result.executionTimeMs =
        timeAfterInjection;

    test.result.expected =
        "Time continues beyond 100 ms and ABS becomes DEGRADED";

    test.result.actual =
        "Before=" +
        std::to_string(
            timeBeforeInjection
        ) +
        " ms, After=" +
        std::to_string(
            timeAfterInjection
        ) +
        " ms, ABS=" +
        std::string(
            degraded ? "DEGRADED" : "NOT DEGRADED"
        );

    if (runtimeEstablished && timeContinued && degraded)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Sensor fault was injected during runtime without restarting "
            "the simulation.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Runtime fault injection did not preserve continuous "
            "simulation execution.";
    }

    return test;
}

// ==================================================
// TC_DIA_007
// Runtime Fault Recovery
// ==================================================

TestCase TestRunner::runRuntimeFaultRecoveryTest()
{
    TestCase test;

    test.id = "TC_DIA_007";
    test.name = "Runtime Fault Recovery";
    test.description =
        "Verify that a supported injected sensor fault can be cleared "
        "without restarting the simulation.";
    test.requirementId = "DIA-REQ-007";
    test.precondition =
        "Simulation shall be running with a front-left Dropout fault "
        "and ABS health DEGRADED.";
    test.stimulus =
        "Clear the fault and continue running the same SimulationEngine instance.";
    test.expectedResult =
        "Simulation time shall continue forward and ABS health shall recover to HEALTHY.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 80.0);

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(engine, 100.0);

    const bool degradedBeforeRecovery =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    const double timeBeforeClear =
        engine.getCurrentTimeMs();

    engine.clearFrontLeftWheelSensorFault();

    runUntil(engine, 120.0);

    const double timeAfterClear =
        engine.getCurrentTimeMs();

    const bool timeContinued =
        timeAfterClear >
        timeBeforeClear;

    const bool healthyAfterRecovery =
        engine.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    test.result.executionTimeMs =
        timeAfterClear;

    test.result.expected =
        "Before=DEGRADED, simulation continues, after clear=HEALTHY";

    test.result.actual =
        "Before=" +
        std::string(
            degradedBeforeRecovery ? "DEGRADED" : "NOT DEGRADED"
        ) +
        ", Time=" +
        std::to_string(
            timeBeforeClear
        ) +
        "->" +
        std::to_string(
            timeAfterClear
        ) +
        " ms, After=" +
        std::string(
            healthyAfterRecovery ? "HEALTHY" : "NOT HEALTHY"
        );

    if (
        degradedBeforeRecovery &&
        timeContinued &&
        healthyAfterRecovery
        )
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Injected sensor fault was cleared during runtime and "
            "ABS health recovered without simulation restart.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Runtime fault recovery did not meet the expected behavior.";
    }

    return test;
}


// ==================================================
// TC_SYS_003
// ECU Communication Through Virtual CAN
// ==================================================

TestCase TestRunner::runEcuCommunicationTest()
{
    TestCase test;

    test.id = "TC_SYS_003";
    test.name = "ECU Communication Through Virtual CAN";
    test.description =
        "Verify that multiple simulated ECUs communicate through the "
        "Virtual CAN Bus during normal simulation execution.";
    test.requirementId = "SYS-REQ-003";
    test.precondition =
        "Simulation shall be reset with ABS, Powertrain, and Steering ECUs available.";
    test.stimulus =
        "Run the Acceleration scenario for 40 ms and inspect CAN trace traffic.";
    test.expectedResult =
        "CAN trace shall contain ABS 0x080, Powertrain 0x100, and Steering 0x120 frames.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);

    runUntil(engine, 40.0);

    bool absSeen = false;
    bool powertrainSeen = false;
    bool steeringSeen = false;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::ABS_WHEEL_STATE.id)
        {
            absSeen = true;
        }
        else if (entry.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            powertrainSeen = true;
        }
        else if (entry.arbitrationId == CanMessageDefinitions::STEERING_STATE.id)
        {
            steeringSeen = true;
        }
    }

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "0x080=true, 0x100=true, 0x120=true";
    test.result.actual =
        "0x080=" + std::string(absSeen ? "true" : "false") +
        ", 0x100=" + std::string(powertrainSeen ? "true" : "false") +
        ", 0x120=" + std::string(steeringSeen ? "true" : "false");

    if (absSeen && powertrainSeen && steeringSeen)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS, Powertrain, and Steering ECU traffic was observed on the Virtual CAN Bus.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Expected ECU communication was not fully observed on the Virtual CAN Bus.";
    }

    return test;
}

// ==================================================
// TC_VEH_005
// Vehicle Speed State
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
// TC_VEH_006
// Front-Left Wheel Speed State
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
// TC_VEH_007
// Front-Right Wheel Speed State
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
// TC_VEH_008
// Brake Input State
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
// TC_VEH_009
// Steering Angle State
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
// TC_VEH_010
// Dynamic State Update
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
// TC_VEH_011
// Independent Wheel Speeds
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

// ==================================================
// TC_ABS_005
// ABS Intervention State Observability
// ==================================================

TestCase TestRunner::runAbsInterventionStateObservabilityTest()
{
    TestCase test;

    test.id = "TC_ABS_005";
    test.name = "ABS Intervention State Observability";
    test.description =
        "Verify that the ABS ECU exposes whether intervention is active.";
    test.requirementId = "ABS-REQ-006";
    test.precondition =
        "Vehicle shall accelerate and cruise before Hard Braking.";
    test.stimulus =
        "Enter Hard Braking and observe the exposed absActive state.";
    test.expectedResult =
        "The exposed ABS intervention state shall become ACTIVE when intervention occurs.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 2000.0);
    engine.startScenario(DrivingScenario::Cruise);
    runUntil(engine, 3000.0);

    engine.startScenario(DrivingScenario::HardBraking);

    bool activeObserved = false;

    while (engine.getCurrentTimeMs() < 4500.0)
    {
        engine.update(simulationStepMs);

        if (engine.getAbsState().absActive)
        {
            activeObserved = true;
            break;
        }
    }

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "Exposed ABS intervention state = ACTIVE";
    test.result.actual =
        std::string("Exposed ABS intervention state = ") +
        (activeObserved ? "ACTIVE" : "INACTIVE");

    if (activeObserved)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS intervention state was directly observable through the ABS state interface.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "ABS intervention state did not become observable as ACTIVE.";
    }

    return test;
}

// ==================================================
// TC_ABS_006
// ABS CAN Identifier
// ==================================================

TestCase TestRunner::runAbsCanIdentifierTest()
{
    TestCase test;

    test.id = "TC_ABS_006";
    test.name = "ABS CAN Identifier";
    test.description =
        "Verify that ABS Wheel State traffic uses CAN identifier 0x080.";
    test.requirementId = "ABS-REQ-008";
    test.precondition = "Simulation shall be reset.";
    test.stimulus = "Run the simulation and inspect ABS CAN traffic.";
    test.expectedResult = "ABS_WHEEL_STATE shall be transmitted using CAN ID 0x080.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 20.0);

    bool absIdentifierSeen = false;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::ABS_WHEEL_STATE.id)
        {
            absIdentifierSeen = true;
            break;
        }
    }

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "ABS CAN ID = 0x080";
    test.result.actual =
        absIdentifierSeen ? "ABS CAN ID = 0x080" : "ABS CAN ID not observed";

    if (absIdentifierSeen)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS Wheel State traffic used the required CAN identifier 0x080.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Required ABS CAN identifier 0x080 was not observed.";
    }

    return test;
}

// ==================================================
// TC_CAN_022
// Multiple ECU Transmission Requests
// ==================================================

TestCase TestRunner::runCanMultipleEcuRequestsTest()
{
    TestCase test;

    test.id = "TC_CAN_022";
    test.name = "Multiple ECU Transmission Requests";
    test.description =
        "Verify that the Virtual CAN Bus accepts multiple pending "
        "transmission requests from simulated ECU message sources.";
    test.requirementId = "CAN-REQ-002";
    test.precondition = "Virtual CAN Bus shall be empty.";
    test.stimulus =
        "Queue ABS, Powertrain, and Steering frames at the same request time and transmit all three.";
    test.expectedResult =
        "All three requests shall be accepted and transmitted through the Virtual CAN Bus.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame absFrame{};
    absFrame.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame steeringFrame{};
    steeringFrame.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    steeringFrame.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(powertrainFrame, 0.0);
    bus.transmit(steeringFrame, 0.0);
    bus.transmit(absFrame, 0.0);

    std::vector<std::uint32_t> transmittedIds;

    while (bus.hasFrame())
    {
        const CanFrame frame =
            bus.receive(bus.getBusyUntilMs());

        transmittedIds.push_back(frame.arbitrationId);
    }

    bool absSeen = false;
    bool powertrainSeen = false;
    bool steeringSeen = false;

    for (const std::uint32_t id : transmittedIds)
    {
        if (id == CanMessageDefinitions::ABS_WHEEL_STATE.id)
        {
            absSeen = true;
        }
        else if (id == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            powertrainSeen = true;
        }
        else if (id == CanMessageDefinitions::STEERING_STATE.id)
        {
            steeringSeen = true;
        }
    }

    test.result.expected =
        "3 transmissions: 0x080, 0x100, 0x120";
    test.result.actual =
        "Count=" + std::to_string(transmittedIds.size()) +
        ", 0x080=" + std::string(absSeen ? "true" : "false") +
        ", 0x100=" + std::string(powertrainSeen ? "true" : "false") +
        ", 0x120=" + std::string(steeringSeen ? "true" : "false");

    if (transmittedIds.size() == 3 &&
        absSeen &&
        powertrainSeen &&
        steeringSeen)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Virtual CAN Bus accepted and transmitted multiple ECU requests.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Virtual CAN Bus did not process all expected ECU transmission requests.";
    }

    return test;
}


// ==================================================
// TC_MSG_002
// ABS Message CAN Identifier
// ==================================================

TestCase TestRunner::runAbsMessageIdentifierTest()
{
    TestCase test;

    test.id = "TC_MSG_002";
    test.name = "ABS Message CAN Identifier";
    test.description =
        "Verify that ABS_WHEEL_STATE encoding uses CAN identifier 0x080.";
    test.requirementId = "MSG-ABS-REQ-001";
    test.precondition = "A valid ABS state shall be available.";
    test.stimulus = "Encode an ABS_WHEEL_STATE CAN frame.";
    test.expectedResult = "Encoded frame shall use CAN ID 0x080.";
    test.result.status = TestStatus::Running;

    AbsState state{};
    state.wheelSpeedFrontLeft = 50.0f;
    state.wheelSpeedFrontRight = 50.5f;
    state.absActive = false;
    state.frontLeftSensorValid = true;
    state.frontRightSensorValid = true;
    state.healthStatus = AbsHealthStatus::Healthy;

    const CanFrame frame =
        AbsStateCodec::encode(state);

    test.result.expected = "CAN ID = 0x080";
    test.result.actual =
        frame.arbitrationId == CanMessageDefinitions::ABS_WHEEL_STATE.id
        ? "CAN ID = 0x080"
        : "Unexpected CAN ID";

    if (frame.arbitrationId == CanMessageDefinitions::ABS_WHEEL_STATE.id)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS_WHEEL_STATE encoding used the required CAN identifier.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "ABS_WHEEL_STATE encoding used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================
// TC_MSG_003
// ABS Message State Information
// ==================================================

TestCase TestRunner::runAbsMessageStateInformationTest()
{
    TestCase test;

    test.id = "TC_MSG_003";
    test.name = "ABS Message State Information";
    test.description =
        "Verify that ABS_WHEEL_STATE contains the defined simulated "
        "wheel-speed, intervention, validity, and health information.";
    test.requirementId = "MSG-ABS-REQ-002";
    test.precondition = "ABS state shall contain known test values.";
    test.stimulus = "Encode and decode ABS_WHEEL_STATE and inspect defined state information.";
    test.expectedResult =
        "Decoded ABS state information shall match the values represented in the message.";
    test.result.status = TestStatus::Running;

    AbsState original{};
    original.wheelSpeedFrontLeft = 62.4f;
    original.wheelSpeedFrontRight = 61.8f;
    original.absActive = true;
    original.frontLeftSensorValid = true;
    original.frontRightSensorValid = false;
    original.healthStatus = AbsHealthStatus::Degraded;

    const CanFrame frame =
        AbsStateCodec::encode(original);

    const AbsState decoded =
        AbsStateCodec::decode(frame);

    constexpr double wheelSpeedResolutionKmh = 0.1;

    const bool frontLeftPresent =
        std::fabs(
            decoded.wheelSpeedFrontLeft -
            original.wheelSpeedFrontLeft
        ) <= wheelSpeedResolutionKmh;

    const bool frontRightPresent =
        std::fabs(
            decoded.wheelSpeedFrontRight -
            original.wheelSpeedFrontRight
        ) <= wheelSpeedResolutionKmh;

    const bool activePresent =
        decoded.absActive == original.absActive;

    const bool frontLeftValidityPresent =
        decoded.frontLeftSensorValid == original.frontLeftSensorValid;

    const bool frontRightValidityPresent =
        decoded.frontRightSensorValid == original.frontRightSensorValid;

    const bool healthPresent =
        decoded.healthStatus == original.healthStatus;

    test.result.expected =
        "FL=62.4, FR=61.8, ABS=true, FLValid=true, FRValid=false, Health=DEGRADED";

    test.result.actual =
        "FL=" + std::to_string(decoded.wheelSpeedFrontLeft) +
        ", FR=" + std::to_string(decoded.wheelSpeedFrontRight) +
        ", ABS=" + std::string(decoded.absActive ? "true" : "false") +
        ", FLValid=" + std::string(decoded.frontLeftSensorValid ? "true" : "false") +
        ", FRValid=" + std::string(decoded.frontRightSensorValid ? "true" : "false") +
        ", Health=" +
        std::string(
            decoded.healthStatus == AbsHealthStatus::Degraded
            ? "DEGRADED"
            : "HEALTHY"
        );

    if (
        frontLeftPresent &&
        frontRightPresent &&
        activePresent &&
        frontLeftValidityPresent &&
        frontRightValidityPresent &&
        healthPresent
        )
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS_WHEEL_STATE contained all defined simulated ABS and wheel-state information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "ABS_WHEEL_STATE did not preserve all required state information.";
    }

    return test;
}

// ==================================================
// TC_MSG_004
// Powertrain Message CAN Identifier
// ==================================================

TestCase TestRunner::runPowertrainMessageIdentifierTest()
{
    TestCase test;

    test.id = "TC_MSG_004";
    test.name = "Powertrain Message CAN Identifier";
    test.description =
        "Verify that POWERTRAIN_STATE encoding uses CAN identifier 0x100.";
    test.requirementId = "MSG-PT-REQ-001";
    test.precondition = "A valid vehicle state shall be available.";
    test.stimulus = "Encode a POWERTRAIN_STATE CAN frame.";
    test.expectedResult = "Encoded frame shall use CAN ID 0x100.";
    test.result.status = TestStatus::Running;

    VehicleState state{};
    state.speedKmh = 72.0f;
    state.engineRpm = 2200;
    state.gear = 3;
    state.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(state);

    test.result.expected = "CAN ID = 0x100";
    test.result.actual =
        frame.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id
        ? "CAN ID = 0x100"
        : "Unexpected CAN ID";

    if (frame.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE encoding used the required CAN identifier.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE encoding used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================
// TC_MSG_005
// Powertrain Vehicle-Speed Information
// ==================================================

TestCase TestRunner::runPowertrainMessageVehicleSpeedTest()
{
    TestCase test;

    test.id = "TC_MSG_005";
    test.name = "Powertrain Vehicle-Speed Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains vehicle-speed information.";
    test.requirementId = "MSG-PT-REQ-002";
    test.precondition = "Vehicle state shall contain a known vehicle speed.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect vehicle speed.";
    test.expectedResult =
        "Decoded vehicle speed shall match the represented value within message resolution.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 82.5f;
    original.engineRpm = 2140;
    original.gear = 3;
    original.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    constexpr double speedResolutionKmh = 0.1;

    const bool matches =
        std::fabs(decoded.speedKmh - original.speedKmh) <=
        speedResolutionKmh;

    test.result.expected = "Vehicle speed = 82.5 km/h";
    test.result.actual =
        "Vehicle speed = " + std::to_string(decoded.speedKmh) + " km/h";

    if (matches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required vehicle-speed information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE vehicle-speed information was not preserved.";
    }

    return test;
}

// ==================================================
// TC_MSG_006
// Powertrain Engine-RPM Information
// ==================================================

TestCase TestRunner::runPowertrainMessageEngineRpmTest()
{
    TestCase test;

    test.id = "TC_MSG_006";
    test.name = "Powertrain Engine-RPM Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains engine-RPM information.";
    test.requirementId = "MSG-PT-REQ-003";
    test.precondition = "Vehicle state shall contain a known engine RPM.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect engine RPM.";
    test.expectedResult = "Decoded engine RPM shall equal the represented value.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 82.5f;
    original.engineRpm = 2140;
    original.gear = 3;
    original.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    test.result.expected = "Engine RPM = 2140";
    test.result.actual =
        "Engine RPM = " + std::to_string(decoded.engineRpm);

    if (decoded.engineRpm == original.engineRpm)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required engine-RPM information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE engine-RPM information was not preserved.";
    }

    return test;
}

// ==================================================
// TC_MSG_007
// Powertrain Gear-State Information
// ==================================================

TestCase TestRunner::runPowertrainMessageGearStateTest()
{
    TestCase test;

    test.id = "TC_MSG_007";
    test.name = "Powertrain Gear-State Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains gear-state information.";
    test.requirementId = "MSG-PT-REQ-004";
    test.precondition = "Vehicle state shall contain a known gear.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect gear state.";
    test.expectedResult = "Decoded gear shall equal the represented gear.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 82.5f;
    original.engineRpm = 2140;
    original.gear = 4;
    original.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    test.result.expected = "Gear = 4";
    test.result.actual =
        "Gear = " +
        std::to_string(
            static_cast<int>(decoded.gear)
        );

    if (decoded.gear == original.gear)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required gear-state information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE gear-state information was not preserved.";
    }

    return test;
}

// ==================================================
// TC_MSG_008
// Powertrain Brake-State Information
// ==================================================

TestCase TestRunner::runPowertrainMessageBrakeStateTest()
{
    TestCase test;

    test.id = "TC_MSG_008";
    test.name = "Powertrain Brake-State Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains brake-state information.";
    test.requirementId = "MSG-PT-REQ-005";
    test.precondition = "Vehicle state shall contain brakeApplied=true.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect brake state.";
    test.expectedResult = "Decoded brake state shall remain applied.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 45.0f;
    original.engineRpm = 1600;
    original.gear = 2;
    original.brakeApplied = true;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    test.result.expected = "Brake applied = true";
    test.result.actual =
        std::string("Brake applied = ") +
        (decoded.brakeApplied ? "true" : "false");

    if (decoded.brakeApplied == original.brakeApplied)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required brake-state information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE brake-state information was not preserved.";
    }

    return test;
}

// ==================================================
// TC_MSG_009
// Steering Message CAN Identifier
// ==================================================

TestCase TestRunner::runSteeringMessageIdentifierTest()
{
    TestCase test;

    test.id = "TC_MSG_009";
    test.name = "Steering Message CAN Identifier";
    test.description =
        "Verify that STEERING_STATE encoding uses CAN identifier 0x120.";
    test.requirementId = "MSG-STR-REQ-001";
    test.precondition = "A valid steering state shall be available.";
    test.stimulus = "Encode a STEERING_STATE CAN frame.";
    test.expectedResult = "Encoded frame shall use CAN ID 0x120.";
    test.result.status = TestStatus::Running;

    SteeringState state{};
    state.steeringAngleDeg = 3.2f;
    state.steeringTorqueNm = 1.45f;

    const CanFrame frame =
        SteeringStateCodec::encode(state);

    test.result.expected = "CAN ID = 0x120";
    test.result.actual =
        frame.arbitrationId == CanMessageDefinitions::STEERING_STATE.id
        ? "CAN ID = 0x120"
        : "Unexpected CAN ID";

    if (frame.arbitrationId == CanMessageDefinitions::STEERING_STATE.id)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "STEERING_STATE encoding used the required CAN identifier.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "STEERING_STATE encoding used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================
// TC_MSG_010
// Steering-Angle Information
// ==================================================

TestCase TestRunner::runSteeringMessageAngleTest()
{
    TestCase test;

    test.id = "TC_MSG_010";
    test.name = "Steering-Angle Information";
    test.description =
        "Verify that STEERING_STATE contains steering-angle information.";
    test.requirementId = "MSG-STR-REQ-002";
    test.precondition = "Steering state shall contain a known steering angle.";
    test.stimulus = "Encode and decode STEERING_STATE and inspect steering angle.";
    test.expectedResult =
        "Decoded steering angle shall match the represented value within message resolution.";
    test.result.status = TestStatus::Running;

    SteeringState original{};
    original.steeringAngleDeg = -12.7f;
    original.steeringTorqueNm = 2.25f;

    const CanFrame frame =
        SteeringStateCodec::encode(original);

    const SteeringState decoded =
        SteeringStateCodec::decode(frame);

    constexpr double angleResolutionDeg = 0.1;

    const bool matches =
        std::fabs(
            decoded.steeringAngleDeg -
            original.steeringAngleDeg
        ) <= angleResolutionDeg;

    test.result.expected = "Steering angle = -12.7 deg";
    test.result.actual =
        "Steering angle = " +
        std::to_string(decoded.steeringAngleDeg) +
        " deg";

    if (matches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "STEERING_STATE contained the required steering-angle information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "STEERING_STATE steering-angle information was not preserved.";
    }

    return test;
}


// ==================================================
// TC_ABS_007
// ABS Wheel Slip Processing
// ==================================================

TestCase TestRunner::runAbsWheelSlipProcessingTest()
{
    TestCase test;

    test.id = "TC_ABS_007";
    test.name = "ABS Wheel Slip Processing";
    test.description =
        "Verify that the ABS ECU processes valid wheel-speed information "
        "during a simulated wheel-slip condition and determines that "
        "ABS intervention is required.";
    test.requirementId = "ABS-REQ-001";
    test.precondition =
        "Vehicle shall be moving with valid front wheel-speed sensor inputs.";
    test.stimulus =
        "Enter Hard Braking, which introduces front-left wheel slip, "
        "and observe wheel-speed divergence and ABS state.";
    test.expectedResult =
        "A valid wheel-speed divergence shall be observed and ABS shall "
        "determine an active intervention state.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();

    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 2000.0);

    engine.startScenario(DrivingScenario::Cruise);
    runUntil(engine, 3000.0);

    engine.startScenario(DrivingScenario::HardBraking);

    bool slipObserved = false;
    bool absActiveDuringSlip = false;
    bool validInputsDuringSlip = false;

    double observedVehicleSpeedKmh = 0.0;
    double observedFrontLeftKmh = 0.0;
    double observedFrontRightKmh = 0.0;

    constexpr double minimumObservableWheelDifferenceKmh = 0.5;
    constexpr double observationEndTimeMs = 4500.0;

    while (engine.getCurrentTimeMs() < observationEndTimeMs)
    {
        engine.update(simulationStepMs);

        const double vehicleSpeedKmh =
            engine.getVehicleModel().getVehicleSpeedKmh();

        const AbsState& absState =
            engine.getAbsState();

        const double wheelDifferenceKmh =
            std::fabs(
                static_cast<double>(absState.wheelSpeedFrontLeft) -
                static_cast<double>(absState.wheelSpeedFrontRight)
            );

        const bool inputsValid =
            absState.frontLeftSensorValid &&
            absState.frontRightSensorValid;

        if (
            vehicleSpeedKmh > speedToleranceKmh &&
            inputsValid &&
            wheelDifferenceKmh >= minimumObservableWheelDifferenceKmh
            )
        {
            slipObserved = true;
            validInputsDuringSlip = true;

            observedVehicleSpeedKmh = vehicleSpeedKmh;
            observedFrontLeftKmh = absState.wheelSpeedFrontLeft;
            observedFrontRightKmh = absState.wheelSpeedFrontRight;

            if (absState.absActive)
            {
                absActiveDuringSlip = true;
                break;
            }
        }
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Valid wheel-speed divergence observed and ABS ACTIVE";

    test.result.actual =
        "Vehicle=" +
        std::to_string(observedVehicleSpeedKmh) +
        " km/h, FL=" +
        std::to_string(observedFrontLeftKmh) +
        " km/h, FR=" +
        std::to_string(observedFrontRightKmh) +
        " km/h, ABS=" +
        std::string(absActiveDuringSlip ? "ACTIVE" : "INACTIVE");

    if (
        slipObserved &&
        validInputsDuringSlip &&
        absActiveDuringSlip
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "ABS ECU processed valid wheel-speed information during "
            "the simulated slip condition and determined intervention.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Expected wheel-slip processing evidence was not observed.";
    }

    return test;
}

// ==================================================
// TC_PT_003
// Powertrain State
// ==================================================

TestCase TestRunner::runPowertrainStateTest()
{
    TestCase test;

    test.id = "TC_PT_003";
    test.name = "Powertrain State";
    test.description =
        "Verify that the Powertrain ECU provides the simulated vehicle "
        "powertrain state through POWERTRAIN_STATE.";
    test.requirementId = "PT-REQ-001";
    test.precondition =
        "Simulation shall be running with Powertrain ECU transmission enabled.";
    test.stimulus =
        "Run Acceleration and decode the latest POWERTRAIN_STATE frame.";
    test.expectedResult =
        "A decodable Powertrain state containing speed, RPM, gear, and brake state shall be provided.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const CanTraceEntry* latestPowertrainEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            latestPowertrainEntry = &entry;
        }
    }

    if (latestPowertrainEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "POWERTRAIN_STATE available";
        test.result.actual = "POWERTRAIN_STATE not found";
        test.result.message =
            "Powertrain ECU did not provide a state frame.";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestPowertrainEntry->arbitrationId;
    frame.dlc = latestPowertrainEntry->dlc;
    frame.data = latestPowertrainEntry->data;

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    const VehicleState& expected =
        engine.getVehicleState();

    constexpr double speedResolutionKmh = 0.1;

    const bool speedMatches =
        std::fabs(decoded.speedKmh - expected.speedKmh) <=
        speedResolutionKmh;

    const bool rpmMatches =
        decoded.engineRpm == expected.engineRpm;

    const bool gearMatches =
        decoded.gear == expected.gear;

    const bool brakeMatches =
        decoded.brakeApplied == expected.brakeApplied;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "Powertrain state matches current simulated VehicleState";
    test.result.actual =
        "Speed=" + std::to_string(decoded.speedKmh) +
        ", RPM=" + std::to_string(decoded.engineRpm) +
        ", Gear=" + std::to_string(static_cast<int>(decoded.gear)) +
        ", Brake=" + std::string(decoded.brakeApplied ? "true" : "false");

    if (speedMatches && rpmMatches && gearMatches && brakeMatches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Powertrain ECU provided the simulated vehicle powertrain state.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Provided Powertrain state did not match the current simulated vehicle state.";
    }

    return test;
}

// ==================================================
// TC_PT_004
// Powertrain Vehicle Speed
// ==================================================

TestCase TestRunner::runPowertrainVehicleSpeedTest()
{
    TestCase test;

    test.id = "TC_PT_004";
    test.name = "Powertrain Vehicle Speed";
    test.description =
        "Verify that Powertrain state includes current simulated vehicle speed.";
    test.requirementId = "PT-REQ-002";
    test.precondition = "Vehicle shall be moving.";
    test.stimulus =
        "Run Acceleration, decode POWERTRAIN_STATE, and compare vehicle speed.";
    test.expectedResult =
        "Powertrain vehicle speed shall match simulated vehicle speed within message resolution.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const CanTraceEntry* latestEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            latestEntry = &entry;
        }
    }

    if (latestEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "Powertrain speed available";
        test.result.actual = "POWERTRAIN_STATE not found";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestEntry->arbitrationId;
    frame.dlc = latestEntry->dlc;
    frame.data = latestEntry->data;

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    const double expectedSpeedKmh =
        engine.getVehicleState().speedKmh;

    constexpr double speedResolutionKmh = 0.1;

    const bool matches =
        std::fabs(decoded.speedKmh - expectedSpeedKmh) <=
        speedResolutionKmh;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "Speed=" + std::to_string(expectedSpeedKmh) + " km/h";
    test.result.actual =
        "Speed=" + std::to_string(decoded.speedKmh) + " km/h";

    if (matches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Powertrain state included current simulated vehicle speed.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Powertrain vehicle-speed state did not match the simulation.";
    }

    return test;
}

// ==================================================
// TC_PT_005
// Powertrain Engine Speed
// ==================================================

TestCase TestRunner::runPowertrainEngineSpeedTest()
{
    TestCase test;

    test.id = "TC_PT_005";
    test.name = "Powertrain Engine Speed";
    test.description =
        "Verify that Powertrain state includes engine RPM information.";
    test.requirementId = "PT-REQ-003";
    test.precondition =
        "Simulation shall be running with a valid Powertrain state.";
    test.stimulus =
        "Decode the latest POWERTRAIN_STATE and compare engine RPM.";
    test.expectedResult =
        "Powertrain engine RPM shall match the simulated Powertrain state.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const CanTraceEntry* latestEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            latestEntry = &entry;
        }
    }

    if (latestEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "Powertrain RPM available";
        test.result.actual = "POWERTRAIN_STATE not found";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestEntry->arbitrationId;
    frame.dlc = latestEntry->dlc;
    frame.data = latestEntry->data;

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    const std::uint16_t expectedRpm =
        engine.getVehicleState().engineRpm;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "RPM=" + std::to_string(expectedRpm);
    test.result.actual =
        "RPM=" + std::to_string(decoded.engineRpm);

    if (decoded.engineRpm == expectedRpm)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Powertrain state included current simulated engine RPM.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Powertrain engine RPM did not match the simulated Powertrain state.";
    }

    return test;
}

// ==================================================
// TC_PT_006
// Powertrain Gear State
// ==================================================

TestCase TestRunner::runPowertrainGearStateTest()
{
    TestCase test;

    test.id = "TC_PT_006";
    test.name = "Powertrain Gear State";
    test.description =
        "Verify that Powertrain state includes current simulated gear information.";
    test.requirementId = "PT-REQ-004";
    test.precondition =
        "Simulation shall be running with a valid Powertrain state.";
    test.stimulus =
        "Decode the latest POWERTRAIN_STATE and compare gear.";
    test.expectedResult =
        "Powertrain gear shall match the simulated Powertrain state.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 1000.0);

    const CanTraceEntry* latestEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            latestEntry = &entry;
        }
    }

    if (latestEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "Powertrain gear available";
        test.result.actual = "POWERTRAIN_STATE not found";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestEntry->arbitrationId;
    frame.dlc = latestEntry->dlc;
    frame.data = latestEntry->data;

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    const auto expectedGear =
        engine.getVehicleState().gear;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "Gear=" + std::to_string(static_cast<int>(expectedGear));
    test.result.actual =
        "Gear=" + std::to_string(static_cast<int>(decoded.gear));

    if (decoded.gear == expectedGear)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Powertrain state included current simulated gear information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Powertrain gear did not match the simulated Powertrain state.";
    }

    return test;
}

// ==================================================
// TC_PT_007
// Powertrain Brake State
// ==================================================

TestCase TestRunner::runPowertrainBrakeStateTest()
{
    TestCase test;

    test.id = "TC_PT_007";
    test.name = "Powertrain Brake State";
    test.description =
        "Verify that Powertrain state includes simulated vehicle brake state.";
    test.requirementId = "PT-REQ-005";
    test.precondition =
        "Vehicle shall be in the Hard Braking scenario.";
    test.stimulus =
        "Run Hard Braking, decode POWERTRAIN_STATE, and inspect brake state.";
    test.expectedResult =
        "Powertrain brake state shall report brake applied.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();

    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 2000.0);

    engine.startScenario(DrivingScenario::Cruise);
    runUntil(engine, 3000.0);

    engine.startScenario(DrivingScenario::HardBraking);
    runUntil(engine, 3200.0);

    const CanTraceEntry* latestEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            latestEntry = &entry;
        }
    }

    if (latestEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "Brake applied=true";
        test.result.actual = "POWERTRAIN_STATE not found";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestEntry->arbitrationId;
    frame.dlc = latestEntry->dlc;
    frame.data = latestEntry->data;

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    const bool sourceBrakeApplied =
        engine.getVehicleState().brakeApplied;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected = "Brake applied=true";
    test.result.actual =
        "Source=" + std::string(sourceBrakeApplied ? "true" : "false") +
        ", Powertrain=" + std::string(decoded.brakeApplied ? "true" : "false");

    if (sourceBrakeApplied && decoded.brakeApplied)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Powertrain state included the simulated applied-brake state.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Powertrain brake state did not represent the current simulated brake condition.";
    }

    return test;
}

// ==================================================
// TC_STR_003
// Steering State
// ==================================================

TestCase TestRunner::runSteeringStateTest()
{
    TestCase test;

    test.id = "TC_STR_003";
    test.name = "Steering State";
    test.description =
        "Verify that the Steering ECU provides the simulated steering state.";
    test.requirementId = "STR-REQ-001";
    test.precondition =
        "Simulation shall be running with Steering ECU transmission enabled.";
    test.stimulus =
        "Decode the latest STEERING_STATE frame and compare it with simulation steering state.";
    test.expectedResult =
        "Steering angle and torque shall match the current simulated steering state.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 100.0);

    const CanTraceEntry* latestEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::STEERING_STATE.id)
        {
            latestEntry = &entry;
        }
    }

    if (latestEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "STEERING_STATE available";
        test.result.actual = "STEERING_STATE not found";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestEntry->arbitrationId;
    frame.dlc = latestEntry->dlc;
    frame.data = latestEntry->data;

    const SteeringState decoded =
        SteeringStateCodec::decode(frame);

    const SteeringState& expected =
        engine.getSteeringState();

    constexpr double angleResolutionDeg = 0.1;
    constexpr double torqueResolutionNm = 0.01;

    const bool angleMatches =
        std::fabs(
            decoded.steeringAngleDeg -
            expected.steeringAngleDeg
        ) <= angleResolutionDeg;

    const bool torqueMatches =
        std::fabs(
            decoded.steeringTorqueNm -
            expected.steeringTorqueNm
        ) <= torqueResolutionNm;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "Steering state matches current simulation state";
    test.result.actual =
        "Angle=" + std::to_string(decoded.steeringAngleDeg) +
        " deg, Torque=" + std::to_string(decoded.steeringTorqueNm) + " Nm";

    if (angleMatches && torqueMatches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Steering ECU provided the simulated steering state.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Provided Steering state did not match the current simulation state.";
    }

    return test;
}

// ==================================================
// TC_STR_004
// Steering Angle
// ==================================================

TestCase TestRunner::runSteeringAngleTest()
{
    TestCase test;

    test.id = "TC_STR_004";
    test.name = "Steering Angle";
    test.description =
        "Verify that Steering state includes simulated steering-angle information.";
    test.requirementId = "STR-REQ-002";
    test.precondition =
        "Simulation shall be initialized with its defined steering angle.";
    test.stimulus =
        "Decode STEERING_STATE and compare steering angle with the simulation.";
    test.expectedResult =
        "Decoded steering angle shall match the simulated steering angle within message resolution.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(DrivingScenario::Acceleration);
    runUntil(engine, 100.0);

    const CanTraceEntry* latestEntry = nullptr;

    for (const CanTraceEntry& entry : engine.getCanTrace())
    {
        if (entry.arbitrationId == CanMessageDefinitions::STEERING_STATE.id)
        {
            latestEntry = &entry;
        }
    }

    if (latestEntry == nullptr)
    {
        test.result.status = TestStatus::Failed;
        test.result.expected = "Steering angle available";
        test.result.actual = "STEERING_STATE not found";
        return test;
    }

    CanFrame frame{};
    frame.arbitrationId = latestEntry->arbitrationId;
    frame.dlc = latestEntry->dlc;
    frame.data = latestEntry->data;

    const SteeringState decoded =
        SteeringStateCodec::decode(frame);

    const double expectedAngleDeg =
        engine.getSteeringState().steeringAngleDeg;

    constexpr double angleResolutionDeg = 0.1;

    const bool matches =
        std::fabs(
            decoded.steeringAngleDeg -
            expectedAngleDeg
        ) <= angleResolutionDeg;

    test.result.executionTimeMs = engine.getCurrentTimeMs();
    test.result.expected =
        "Angle=" + std::to_string(expectedAngleDeg) + " deg";
    test.result.actual =
        "Angle=" + std::to_string(decoded.steeringAngleDeg) + " deg";

    if (matches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Steering state included the simulated steering angle.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Steering angle did not match the current simulated steering state.";
    }

    return test;
}


// ==================================================
// TC_SYS_004
// Automated Validation Isolation
// ==================================================

TestCase TestRunner::runValidationIsolationTest()
{
    TestCase test;

    test.id = "TC_SYS_004";
    test.name = "Automated Validation Isolation";
    test.description =
        "Verify that an automated validation simulation instance does not "
        "modify the state of a separate interactive-style simulation instance.";
    test.requirementId = "SYS-REQ-007";
    test.precondition =
        "Two independent SimulationEngine instances shall be created.";
    test.stimulus =
        "Advance and fault-inject only the validation instance.";
    test.expectedResult =
        "The second simulation instance shall retain its original time, "
        "health state, and CAN trace.";
    test.result.status = TestStatus::Running;

    SimulationEngine interactiveEngine(LogLevel::Quiet);
    SimulationEngine validationEngine(LogLevel::Quiet);

    interactiveEngine.reset();
    validationEngine.reset();

    const double interactiveInitialTime =
        interactiveEngine.getCurrentTimeMs();

    const AbsHealthStatus interactiveInitialHealth =
        interactiveEngine.getAbsState().healthStatus;

    const std::size_t interactiveInitialTraceSize =
        interactiveEngine.getCanTrace().size();

    validationEngine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        validationEngine,
        100.0
    );

    validationEngine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        validationEngine,
        120.0
    );

    const bool interactiveTimeUnchanged =
        std::fabs(
            interactiveEngine.getCurrentTimeMs() -
            interactiveInitialTime
        ) <= 0.001;

    const bool interactiveHealthUnchanged =
        interactiveEngine.getAbsState().healthStatus ==
        interactiveInitialHealth;

    const bool interactiveTraceUnchanged =
        interactiveEngine.getCanTrace().size() ==
        interactiveInitialTraceSize;

    const bool validationActuallyChanged =
        validationEngine.getCurrentTimeMs() > 0.0 &&
        !validationEngine.getCanTrace().empty();

    test.result.executionTimeMs =
        validationEngine.getCurrentTimeMs();

    test.result.expected =
        "Interactive instance unchanged; validation instance changes";

    test.result.actual =
        "InteractiveTime=" +
        std::to_string(interactiveEngine.getCurrentTimeMs()) +
        " ms, InteractiveTrace=" +
        std::to_string(interactiveEngine.getCanTrace().size()) +
        ", ValidationTime=" +
        std::to_string(validationEngine.getCurrentTimeMs()) +
        " ms";

    if (
        interactiveTimeUnchanged &&
        interactiveHealthUnchanged &&
        interactiveTraceUnchanged &&
        validationActuallyChanged
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Validation activity remained isolated from the separate simulation instance.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Validation activity modified or failed to remain isolated from the separate instance.";
    }

    return test;
}

// ==================================================
// TC_SNS_004
// Front-Left Wheel-Speed Measurement
// ==================================================

TestCase TestRunner::runFrontLeftWheelSpeedMeasurementTest()
{
    TestCase test;

    test.id = "TC_SNS_004";
    test.name = "Front-Left Wheel-Speed Measurement";
    test.description =
        "Verify that the simulated front-left wheel-speed sensor provides "
        "the front-left wheel speed to the ABS subsystem.";
    test.requirementId = "SNS-REQ-001";
    test.precondition =
        "Vehicle shall be moving with no front-left sensor fault.";
    test.stimulus =
        "Run Acceleration and compare VehicleModel front-left wheel speed "
        "with the value observed by the ABS state.";
    test.expectedResult =
        "ABS front-left wheel speed shall match the simulated front-left wheel speed.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        1000.0
    );

    const double expectedWheelSpeed =
        engine.getVehicleModel()
        .getFrontLeftWheelSpeedKmh();

    const AbsState& absState =
        engine.getAbsState();

    const double observedWheelSpeed =
        absState.wheelSpeedFrontLeft;

    constexpr double toleranceKmh = 0.1;

    const bool valid =
        absState.frontLeftSensorValid;

    const bool matches =
        std::fabs(
            observedWheelSpeed -
            expectedWheelSpeed
        ) <= toleranceKmh;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS FL speed=" +
        std::to_string(expectedWheelSpeed) +
        " km/h, valid=true";

    test.result.actual =
        "ABS FL speed=" +
        std::to_string(observedWheelSpeed) +
        " km/h, valid=" +
        std::string(valid ? "true" : "false");

    if (valid && matches)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-left wheel-speed measurement was provided to the ABS subsystem.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-left wheel-speed measurement did not match the simulated wheel speed.";
    }

    return test;
}

// ==================================================
// TC_SNS_005
// Front-Right Wheel-Speed Measurement
// ==================================================

TestCase TestRunner::runFrontRightWheelSpeedMeasurementTest()
{
    TestCase test;

    test.id = "TC_SNS_005";
    test.name = "Front-Right Wheel-Speed Measurement";
    test.description =
        "Verify that the simulated front-right wheel-speed sensor provides "
        "the front-right wheel speed to the ABS subsystem.";
    test.requirementId = "SNS-REQ-002";
    test.precondition =
        "Vehicle shall be moving with no front-right sensor fault.";
    test.stimulus =
        "Run Acceleration and compare VehicleModel front-right wheel speed "
        "with the value observed by the ABS state.";
    test.expectedResult =
        "ABS front-right wheel speed shall match the simulated front-right wheel speed.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        1000.0
    );

    const double expectedWheelSpeed =
        engine.getVehicleModel()
        .getFrontRightWheelSpeedKmh();

    const AbsState& absState =
        engine.getAbsState();

    const double observedWheelSpeed =
        absState.wheelSpeedFrontRight;

    constexpr double toleranceKmh = 0.1;

    const bool valid =
        absState.frontRightSensorValid;

    const bool matches =
        std::fabs(
            observedWheelSpeed -
            expectedWheelSpeed
        ) <= toleranceKmh;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS FR speed=" +
        std::to_string(expectedWheelSpeed) +
        " km/h, valid=true";

    test.result.actual =
        "ABS FR speed=" +
        std::to_string(observedWheelSpeed) +
        " km/h, valid=" +
        std::string(valid ? "true" : "false");

    if (valid && matches)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-right wheel-speed measurement was provided to the ABS subsystem.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-right wheel-speed measurement did not match the simulated wheel speed.";
    }

    return test;
}

// ==================================================
// TC_SNS_006
// Front-Left Dropout Injection
// ==================================================

TestCase TestRunner::runFrontLeftDropoutInjectionTest()
{
    TestCase test;

    test.id = "TC_SNS_006";
    test.name = "Front-Left Dropout Injection";
    test.description =
        "Verify direct support for injecting a Dropout fault into the "
        "front-left wheel-speed sensor.";
    test.requirementId = "SNS-REQ-003";
    test.precondition =
        "Front-left wheel-speed sensor shall initially provide a valid measurement.";
    test.stimulus =
        "Inject SensorFault::Dropout into the front-left wheel-speed sensor during execution.";
    test.expectedResult =
        "The consuming ABS subsystem shall observe the front-left sensor as invalid.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    const bool validBefore =
        engine.getAbsState()
        .frontLeftSensorValid;

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool validAfter =
        engine.getAbsState()
        .frontLeftSensorValid;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Valid before=true, valid after Dropout=false";

    test.result.actual =
        "Valid before=" +
        std::string(validBefore ? "true" : "false") +
        ", valid after=" +
        std::string(validAfter ? "true" : "false");

    if (validBefore && !validAfter)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-left Dropout fault injection was supported and observed by ABS.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-left Dropout fault injection did not produce the expected invalid input.";
    }

    return test;
}

// ==================================================
// TC_VAL_001
// Individual Test Execution
// ==================================================

TestCase TestRunner::runIndividualTestExecutionVerification()
{
    TestCase test;

    test.id = "TC_VAL_001";
    test.name = "Individual Test Execution";
    test.description =
        "Verify that the validation framework can execute one selected automated test case.";
    test.requirementId = "VAL-REQ-001";
    test.precondition =
        "TC_SYS_001 shall be registered in the TestRunner dispatcher.";
    test.stimulus =
        "Execute only TC_SYS_001 through runTest().";
    test.expectedResult =
        "Exactly the requested TC_SYS_001 result shall be returned as executed.";
    test.result.status = TestStatus::Running;

    const TestCase selected =
        runTest(
            "TC_SYS_001"
        );

    const bool correctId =
        selected.id == "TC_SYS_001";

    const bool executed =
        selected.result.status !=
        TestStatus::NotRun &&
        selected.result.status !=
        TestStatus::Running;

    test.result.executionTimeMs =
        selected.result.executionTimeMs;

    test.result.expected =
        "Returned ID=TC_SYS_001 and final execution status";

    test.result.actual =
        "Returned ID=" +
        selected.id +
        ", executed=" +
        std::string(executed ? "true" : "false");

    if (correctId && executed)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Validation framework executed the individually selected test case.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Validation framework did not correctly execute the selected individual test.";
    }

    return test;
}

// ==================================================
// TC_VAL_002
// Validation Test Isolation
// ==================================================

TestCase TestRunner::runValidationTestIsolationVerification()
{
    TestCase test;

    test.id = "TC_VAL_002";
    test.name = "Validation Test Isolation";
    test.description =
        "Verify that independently created automated simulation test instances "
        "do not share simulation state.";
    test.requirementId = "VAL-REQ-003";
    test.precondition =
        "Two clean SimulationEngine instances shall exist.";
    test.stimulus =
        "Advance only the first instance and inject a sensor fault into it.";
    test.expectedResult =
        "The second instance shall remain at its initial time and healthy ABS state.";
    test.result.status = TestStatus::Running;

    SimulationEngine first(LogLevel::Quiet);
    SimulationEngine second(LogLevel::Quiet);

    first.reset();
    second.reset();

    first.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        first,
        100.0
    );

    first.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        first,
        120.0
    );

    const bool firstChanged =
        first.getCurrentTimeMs() > 0.0 &&
        first.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    const bool secondTimeClean =
        std::fabs(
            second.getCurrentTimeMs()
        ) <= 0.001;

    const bool secondHealthClean =
        second.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    const bool secondTraceClean =
        second.getCanTrace().empty();

    test.result.executionTimeMs =
        first.getCurrentTimeMs();

    test.result.expected =
        "First instance changed; second instance remains clean";

    test.result.actual =
        "FirstTime=" +
        std::to_string(first.getCurrentTimeMs()) +
        ", SecondTime=" +
        std::to_string(second.getCurrentTimeMs()) +
        ", SecondTrace=" +
        std::to_string(second.getCanTrace().size());

    if (
        firstChanged &&
        secondTimeClean &&
        secondHealthClean &&
        secondTraceClean
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Automated simulation instances remained isolated.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Automated simulation instance isolation was not maintained.";
    }

    return test;
}

// ==================================================
// TC_VAL_003
// Test Status
// ==================================================

TestCase TestRunner::runTestStatusVerification()
{
    TestCase test;

    test.id = "TC_VAL_003";
    test.name = "Test Status";
    test.description =
        "Verify that an executed automated test produces a final PASS or FAIL status.";
    test.requirementId = "VAL-REQ-008";
    test.precondition =
        "A registered automated test shall be available.";
    test.stimulus =
        "Execute TC_CAN_002 through the validation dispatcher.";
    test.expectedResult =
        "The executed result status shall be either Passed or Failed.";
    test.result.status = TestStatus::Running;

    const TestCase executed =
        runTest(
            "TC_CAN_002"
        );

    const bool finalStatus =
        executed.result.status ==
        TestStatus::Passed ||
        executed.result.status ==
        TestStatus::Failed;

    test.result.executionTimeMs =
        executed.result.executionTimeMs;

    test.result.expected =
        "Status = PASS or FAIL";

    test.result.actual =
        finalStatus
        ? (
            executed.result.status == TestStatus::Passed
            ? "Status = PASS"
            : "Status = FAIL"
            )
        : "Status is not final";

    if (finalStatus)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Executed automated test produced a final PASS/FAIL result.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Executed automated test did not produce a final PASS/FAIL result.";
    }

    return test;
}

// ==================================================
// TC_VAL_004
// Execution-Time Evidence
// ==================================================

TestCase TestRunner::runExecutionTimeVerification()
{
    TestCase test;

    test.id = "TC_VAL_004";
    test.name = "Execution-Time Evidence";
    test.description =
        "Verify that the validation framework records execution time for an executed test.";
    test.requirementId = "VAL-REQ-009";
    test.precondition =
        "A simulation-based automated test shall be available.";
    test.stimulus =
        "Execute TC_SYS_001 and inspect its executionTimeMs evidence.";
    test.expectedResult =
        "Execution time shall be recorded as a positive value.";
    test.result.status = TestStatus::Running;

    const TestCase executed =
        runTest(
            "TC_SYS_001"
        );

    const bool recorded =
        executed.result.executionTimeMs > 0.0;

    test.result.executionTimeMs =
        executed.result.executionTimeMs;

    test.result.expected =
        "executionTimeMs > 0";

    test.result.actual =
        "executionTimeMs=" +
        std::to_string(
            executed.result.executionTimeMs
        );

    if (recorded)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Execution-time evidence was recorded for the executed automated test.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Execution-time evidence was not recorded.";
    }

    return test;
}

// ==================================================
// TC_VAL_005
// Response-Time Evidence
// ==================================================

TestCase TestRunner::runResponseTimeEvidenceVerification()
{
    TestCase test;

    test.id = "TC_VAL_005";
    test.name = "Response-Time Evidence";
    test.description =
        "Verify that a test associated with a maximum response-time requirement "
        "records measured response time.";
    test.requirementId = "VAL-REQ-010";
    test.precondition =
        "TC_ABS_002 shall verify the 20 ms front-left sensor-fault response requirement.";
    test.stimulus =
        "Execute TC_ABS_002 and inspect responseTimeMs.";
    test.expectedResult =
        "Measured response time shall be recorded and the timing requirement shall be identified.";
    test.result.status = TestStatus::Running;

    const TestCase executed =
        runTest(
            "TC_ABS_002"
        );

    const bool timingRequirementPresent =
        executed.result.maximumAllowedResponseTimeMs > 0.0;

    const bool responseRecorded =
        executed.result.responseTimeMs > 0.0;

    test.result.executionTimeMs =
        executed.result.executionTimeMs;

    test.result.responseTimeMs =
        executed.result.responseTimeMs;

    test.result.maximumAllowedResponseTimeMs =
        executed.result.maximumAllowedResponseTimeMs;

    test.result.expected =
        "responseTimeMs > 0 and maximumAllowedResponseTimeMs = 20";

    test.result.actual =
        "responseTimeMs=" +
        std::to_string(executed.result.responseTimeMs) +
        ", maximumAllowedResponseTimeMs=" +
        std::to_string(
            executed.result.maximumAllowedResponseTimeMs
        );

    if (
        timingRequirementPresent &&
        responseRecorded
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Measured response-time evidence was recorded for the timed requirement.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Required response-time evidence was not recorded.";
    }

    return test;
}

// ==================================================
// TC_VAL_006
// Timing Pass Criteria
// ==================================================

TestCase TestRunner::runTimingPassCriteriaVerification()
{
    TestCase test;

    test.id = "TC_VAL_006";
    test.name = "Timing Pass Criteria";
    test.description =
        "Verify validation timing pass criteria reject a response time "
        "that exceeds its maximum allowed limit.";
    test.requirementId = "VAL-REQ-012";
    test.precondition =
        "A TestResult shall define a 20 ms maximum response time.";
    test.stimulus =
        "Evaluate a synthetic measured response time of 21 ms against a 20 ms maximum.";
    test.expectedResult =
        "responseTimeWithinLimit() shall return false.";
    test.result.status = TestStatus::Running;

    TestResult timingResult{};

    timingResult.responseTimeMs =
        21.0;

    timingResult.maximumAllowedResponseTimeMs =
        20.0;

    const bool withinLimit =
        timingResult.responseTimeWithinLimit();

    test.result.expected =
        "21 ms <= 20 ms = false";

    test.result.actual =
        std::string("responseTimeWithinLimit=") +
        (withinLimit ? "true" : "false");

    if (!withinLimit)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Validation timing criteria correctly rejected an over-limit response time.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Validation timing criteria incorrectly accepted an over-limit response time.";
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
        84
    );

    // ==================================================
    // System
    // ==================================================

    results.push_back(
        runSimulationTimeProgressionTest()
    );

    results.push_back(
        runSimulationResetTest()
    );

    results.push_back(
        runEcuCommunicationTest()
    );

    results.push_back(
        runValidationIsolationTest()
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

    results.push_back(
        runVehicleSpeedStateTest()
    );

    results.push_back(
        runFrontLeftWheelSpeedStateTest()
    );

    results.push_back(
        runFrontRightWheelSpeedStateTest()
    );

    results.push_back(
        runBrakeInputStateTest()
    );

    results.push_back(
        runSteeringAngleStateTest()
    );

    results.push_back(
        runVehicleDynamicStateUpdateTest()
    );

    results.push_back(
        runIndependentWheelSpeedsTest()
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

    results.push_back(
        runAbsHealthyStateTest()
    );

    results.push_back(
        runAbsInterventionStateObservabilityTest()
    );

    results.push_back(
        runAbsCanIdentifierTest()
    );

    results.push_back(
        runAbsWheelSlipProcessingTest()
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

    results.push_back(
        runFrontLeftWheelSpeedMeasurementTest()
    );

    results.push_back(
        runFrontRightWheelSpeedMeasurementTest()
    );

    results.push_back(
        runFrontLeftDropoutInjectionTest()
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

    results.push_back(
        runPowertrainStateTest()
    );

    results.push_back(
        runPowertrainVehicleSpeedTest()
    );

    results.push_back(
        runPowertrainEngineSpeedTest()
    );

    results.push_back(
        runPowertrainGearStateTest()
    );

    results.push_back(
        runPowertrainBrakeStateTest()
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

    results.push_back(
        runSteeringStateTest()
    );

    results.push_back(
        runSteeringAngleTest()
    );

    // ==================================================
    // Diagnostics
    // ==================================================

    results.push_back(
        runFrontLeftDiagnosticFaultInjectionTest()
    );

    results.push_back(
        runFrontRightDiagnosticFaultInjectionTest()
    );

    results.push_back(
        runFrontLeftDiagnosticFaultClearTest()
    );

    results.push_back(
        runFrontRightDiagnosticFaultClearTest()
    );

    results.push_back(
        runDiagnosticHealthObservabilityTest()
    );

    results.push_back(
        runRuntimeFaultInjectionTest()
    );

    results.push_back(
        runRuntimeFaultRecoveryTest()
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

    results.push_back(
        runCanTransmissionDurationTest()
    );

    results.push_back(
        runCanWaitingTimeTest()
    );

    results.push_back(
        runCanFrameCountTest()
    );

    results.push_back(
        runCanAverageWaitingTimeTest()
    );

    results.push_back(
        runCanMaximumWaitingTimeTest()
    );

    results.push_back(
        runCanTotalTransmissionTimeTest()
    );

    results.push_back(
        runCanPerMessageStatisticsTest()
    );

    results.push_back(
        runCanPendingFrameBehaviorTest()
    );

    results.push_back(
        runCanWaitingTimeTraceEvidenceTest()
    );

    results.push_back(
        runCanExclusiveBusAccessTest()
    );

    results.push_back(
        runCanTraceIdentifierTest()
    );

    results.push_back(
        runCanTracePayloadTest()
    );

    results.push_back(
        runCanMultipleEcuRequestsTest()
    );

    // ==================================================
    // Messages
    // ==================================================

    results.push_back(
        runAbsCodecRoundtripTest()
    );

    results.push_back(
        runAbsMessageIdentifierTest()
    );

    results.push_back(
        runAbsMessageStateInformationTest()
    );

    results.push_back(
        runPowertrainMessageIdentifierTest()
    );

    results.push_back(
        runPowertrainMessageVehicleSpeedTest()
    );

    results.push_back(
        runPowertrainMessageEngineRpmTest()
    );

    results.push_back(
        runPowertrainMessageGearStateTest()
    );

    results.push_back(
        runPowertrainMessageBrakeStateTest()
    );

    results.push_back(
        runSteeringMessageIdentifierTest()
    );

    results.push_back(
        runSteeringMessageAngleTest()
    );

    // ==================================================
    // Validation Framework
    // ==================================================

    results.push_back(
        runIndividualTestExecutionVerification()
    );

    results.push_back(
        runValidationTestIsolationVerification()
    );

    results.push_back(
        runTestStatusVerification()
    );

    results.push_back(
        runExecutionTimeVerification()
    );

    results.push_back(
        runResponseTimeEvidenceVerification()
    );

    results.push_back(
        runTimingPassCriteriaVerification()
    );

    return results;
}