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

