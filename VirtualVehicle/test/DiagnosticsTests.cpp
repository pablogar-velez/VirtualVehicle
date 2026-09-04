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

