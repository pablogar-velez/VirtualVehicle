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

