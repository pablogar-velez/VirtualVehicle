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

