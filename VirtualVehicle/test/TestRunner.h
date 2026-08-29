#pragma once

#include <string>
#include <vector>

#include "TestCase.h"

class SimulationEngine;

class TestRunner
{
public:
    TestRunner();

    // ==================================================
    // System Tests
    // ==================================================

    TestCase runSimulationTimeProgressionTest();

    // ==================================================
    // Vehicle / Scenario Tests
    // ==================================================

    TestCase runAccelerationScenarioTest();
    TestCase runCruiseScenarioTest();
    TestCase runHardBrakingScenarioTest();
    TestCase runRecoveryScenarioTest();

    // ==================================================
    // ABS Tests
    // ==================================================

    TestCase runAbsEmergencyBrakingTest();
    TestCase runFrontLeftWheelSensorDropoutTest();
    TestCase runFrontRightWheelSensorDropoutTest();

    // ==================================================
    // Sensor Tests
    // ==================================================

    TestCase runFrontLeftSensorDropoutTest();
    TestCase runFrontRightSensorDropoutTest();
    TestCase runSensorFaultRecoveryTest();

    // ==================================================
    // Powertrain Tests
    // ==================================================

    TestCase runPowertrainCanIdentifierTest();
    TestCase runPowertrainCodecRoundtripTest();

    // ==================================================
    // Steering Tests
    // ==================================================

    TestCase runSteeringCanIdentifierTest();
    TestCase runSteeringCodecRoundtripTest();

    // ==================================================
    // CAN Tests
    // ==================================================

    TestCase runCanArbitrationPriorityTest();
    TestCase runCanBitrateTest();
    TestCase runCanTraceRecordingTest();
    TestCase runCanArbitrationStatisticsTest();

    TestCase runAbsMessagePeriodicityTest();
    TestCase runPowertrainMessagePeriodicityTest();
    TestCase runSteeringMessagePeriodicityTest();
    TestCase runCanBusUtilizationTest();
    TestCase runCanTraceTimingEvidenceTest();

    // ==================================================
    // Message Tests
    // ==================================================

    TestCase runAbsCodecRoundtripTest();

    // ==================================================
    // Dispatcher / Suite
    // ==================================================

    TestCase runTest(
        const std::string& testId
    );

    std::vector<TestCase> runAll();

private:
    void runUntil(
        SimulationEngine& engine,
        double targetTimeMs,
        double stepMs = 10.0
    );
};
