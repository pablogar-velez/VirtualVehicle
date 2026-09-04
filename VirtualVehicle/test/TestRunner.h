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
    TestCase runSimulationResetTest();
    TestCase runEcuCommunicationTest();
    TestCase runValidationIsolationTest();

    // ==================================================
    // Vehicle / Scenario Tests
    // ==================================================

    TestCase runAccelerationScenarioTest();
    TestCase runCruiseScenarioTest();
    TestCase runHardBrakingScenarioTest();
    TestCase runRecoveryScenarioTest();
    TestCase runVehicleSpeedStateTest();
    TestCase runFrontLeftWheelSpeedStateTest();
    TestCase runFrontRightWheelSpeedStateTest();
    TestCase runBrakeInputStateTest();
    TestCase runSteeringAngleStateTest();
    TestCase runVehicleDynamicStateUpdateTest();
    TestCase runIndependentWheelSpeedsTest();

    // ==================================================
    // ABS Tests
    // ==================================================

    TestCase runAbsEmergencyBrakingTest();
    TestCase runFrontLeftWheelSensorDropoutTest();
    TestCase runFrontRightWheelSensorDropoutTest();
    TestCase runAbsHealthyStateTest();
    TestCase runAbsInterventionStateObservabilityTest();
    TestCase runAbsCanIdentifierTest();
    TestCase runAbsWheelSlipProcessingTest();

    // ==================================================
    // Sensor Tests
    // ==================================================

    TestCase runFrontLeftSensorDropoutTest();
    TestCase runFrontRightSensorDropoutTest();
    TestCase runSensorFaultRecoveryTest();
    TestCase runFrontLeftWheelSpeedMeasurementTest();
    TestCase runFrontRightWheelSpeedMeasurementTest();
    TestCase runFrontLeftDropoutInjectionTest();

    // ==================================================
    // Powertrain Tests
    // ==================================================

    TestCase runPowertrainCanIdentifierTest();
    TestCase runPowertrainCodecRoundtripTest();
    TestCase runPowertrainStateTest();
    TestCase runPowertrainVehicleSpeedTest();
    TestCase runPowertrainEngineSpeedTest();
    TestCase runPowertrainGearStateTest();
    TestCase runPowertrainBrakeStateTest();

    // ==================================================
    // Steering Tests
    // ==================================================

    TestCase runSteeringCanIdentifierTest();
    TestCase runSteeringCodecRoundtripTest();
    TestCase runSteeringStateTest();
    TestCase runSteeringAngleTest();

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

    TestCase runCanTransmissionDurationTest();
    TestCase runCanWaitingTimeTest();
    TestCase runCanFrameCountTest();
    TestCase runCanAverageWaitingTimeTest();
    TestCase runCanMaximumWaitingTimeTest();
    TestCase runCanTotalTransmissionTimeTest();
    TestCase runCanPerMessageStatisticsTest();
    TestCase runCanPendingFrameBehaviorTest();
    TestCase runCanWaitingTimeTraceEvidenceTest();
    TestCase runCanExclusiveBusAccessTest();
    TestCase runCanTraceIdentifierTest();
    TestCase runCanTracePayloadTest();
    TestCase runCanMultipleEcuRequestsTest();

    // ==================================================
    // Diagnostics Tests
    // ==================================================

    TestCase runFrontLeftDiagnosticFaultInjectionTest();
    TestCase runFrontRightDiagnosticFaultInjectionTest();
    TestCase runFrontLeftDiagnosticFaultClearTest();
    TestCase runFrontRightDiagnosticFaultClearTest();
    TestCase runDiagnosticHealthObservabilityTest();
    TestCase runRuntimeFaultInjectionTest();
    TestCase runRuntimeFaultRecoveryTest();

    // ==================================================
    // DTC Tests
    // ==================================================

    TestCase runFrontLeftDtcDefinitionTest();
    TestCase runFrontRightDtcDefinitionTest();
    TestCase runFrontLeftDtcActivationTest();
    TestCase runFrontRightDtcActivationTest();
    TestCase runFrontLeftDtcRecoveryTest();
    TestCase runFrontRightDtcRecoveryTest();
    TestCase runIndividualDtcClearTest();
    TestCase runClearAllDtcsTest();
    TestCase runDtcFirstDetectionTimestampTest();
    TestCase runDtcLastDetectionTimestampTest();
    TestCase runDtcOccurrenceCountTest();
    TestCase runDtcLookupTest();
    TestCase runDtcCollectionAccessTest();
    TestCase runActiveDtcQueryTest();
    TestCase runRuntimeDtcDetectionTest();
    TestCase runRuntimeDtcRecoveryTest();
    TestCase runDtcResetTest();
    TestCase runDtcDeterminismTest();

    // ==================================================
    // UDS Tests
    // ==================================================

    TestCase runUdsRequestRepresentationTest();
    TestCase runUdsResponseRepresentationTest();
    TestCase runUdsServerProcessingTest();
    TestCase runUdsPositiveResponseSidTest();
    TestCase runUdsNegativeResponseFormatTest();
    TestCase runUdsUnsupportedServiceTest();

    TestCase runUdsDiagnosticSessionControlSupportTest();
    TestCase runUdsDefaultSessionTest();
    TestCase runUdsExtendedSessionTest();
    TestCase runUdsSessionStateUpdateTest();
    TestCase runUdsUnsupportedSessionTest();

    TestCase runUdsReadDtcInformationSupportTest();
    TestCase runUdsReadActiveDtcsTest();
    TestCase runUdsFrontLeftDtcReportingTest();
    TestCase runUdsFrontRightDtcReportingTest();
    TestCase runUdsNoActiveDtcResponseTest();

    TestCase runUdsClearDiagnosticInformationSupportTest();
    TestCase runUdsClearAllDtcsTest();
    TestCase runUdsClearDiagnosticResponseTest();

    TestCase runUdsReadDataByIdentifierSupportTest();
    TestCase runUdsVehicleSpeedDidTest();
    TestCase runUdsVehicleSpeedReadoutTest();
    TestCase runUdsEngineSpeedDidTest();
    TestCase runUdsEngineSpeedReadoutTest();
    TestCase runUdsSteeringAngleDidTest();
    TestCase runUdsSteeringAngleReadoutTest();
    TestCase runUdsUnsupportedDidTest();

    TestCase runUdsDtcManagerAccessTest();
    TestCase runUdsDtcClearIntegrationTest();

    TestCase runUdsRuntimeDiagnosticProcessingTest();
    TestCase runUdsCurrentVehicleDataTest();

    TestCase runUdsResetStateTest();
    TestCase runUdsDeterminismTest();

    // ==================================================
    // Message Tests
    // ==================================================

    TestCase runAbsCodecRoundtripTest();
    TestCase runAbsMessageIdentifierTest();
    TestCase runAbsMessageStateInformationTest();
    TestCase runPowertrainMessageIdentifierTest();
    TestCase runPowertrainMessageVehicleSpeedTest();
    TestCase runPowertrainMessageEngineRpmTest();
    TestCase runPowertrainMessageGearStateTest();
    TestCase runPowertrainMessageBrakeStateTest();
    TestCase runSteeringMessageIdentifierTest();
    TestCase runSteeringMessageAngleTest();

    // ==================================================
    // Validation Framework Tests
    // ==================================================

    TestCase runIndividualTestExecutionVerification();
    TestCase runValidationTestIsolationVerification();
    TestCase runTestStatusVerification();
    TestCase runExecutionTimeVerification();
    TestCase runResponseTimeEvidenceVerification();
    TestCase runTimingPassCriteriaVerification();

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