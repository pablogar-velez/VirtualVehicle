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

