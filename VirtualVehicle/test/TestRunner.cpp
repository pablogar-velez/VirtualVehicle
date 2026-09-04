#include "TestRunner.h"

#include "../simulation/SimulationEngine.h"

// ==================================================
// Constructor
// ==================================================

TestRunner::TestRunner()
{
}

// ==================================================
// Helper
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
// Run test by ID
// ==================================================

TestCase TestRunner::runTest(
    const std::string& testId)
{
    // ==================================================
    // System
    // ==================================================

    if (testId == "TC_SYS_001")
        return runSimulationTimeProgressionTest();

    if (testId == "TC_SYS_002")
        return runSimulationResetTest();

    if (testId == "TC_SYS_003")
        return runEcuCommunicationTest();

    if (testId == "TC_SYS_004")
        return runValidationIsolationTest();

    // ==================================================
    // Vehicle
    // ==================================================

    if (testId == "TC_VEH_001")
        return runAccelerationScenarioTest();

    if (testId == "TC_VEH_002")
        return runCruiseScenarioTest();

    if (testId == "TC_VEH_003")
        return runHardBrakingScenarioTest();

    if (testId == "TC_VEH_004")
        return runRecoveryScenarioTest();

    if (testId == "TC_VEH_005")
        return runVehicleSpeedStateTest();

    if (testId == "TC_VEH_006")
        return runFrontLeftWheelSpeedStateTest();

    if (testId == "TC_VEH_007")
        return runFrontRightWheelSpeedStateTest();

    if (testId == "TC_VEH_008")
        return runBrakeInputStateTest();

    if (testId == "TC_VEH_009")
        return runSteeringAngleStateTest();

    if (testId == "TC_VEH_010")
        return runVehicleDynamicStateUpdateTest();

    if (testId == "TC_VEH_011")
        return runIndependentWheelSpeedsTest();

    // ==================================================
    // ABS
    // ==================================================

    if (testId == "TC_ABS_001")
        return runAbsEmergencyBrakingTest();

    if (testId == "TC_ABS_002")
        return runFrontLeftWheelSensorDropoutTest();

    if (testId == "TC_ABS_003")
        return runFrontRightWheelSensorDropoutTest();

    if (testId == "TC_ABS_004")
        return runAbsHealthyStateTest();

    if (testId == "TC_ABS_005")
        return runAbsInterventionStateObservabilityTest();

    if (testId == "TC_ABS_006")
        return runAbsCanIdentifierTest();

    if (testId == "TC_ABS_007")
        return runAbsWheelSlipProcessingTest();

    // ==================================================
    // Sensors
    // ==================================================

    if (testId == "TC_SNS_001")
        return runFrontLeftSensorDropoutTest();

    if (testId == "TC_SNS_002")
        return runFrontRightSensorDropoutTest();

    if (testId == "TC_SNS_003")
        return runSensorFaultRecoveryTest();

    if (testId == "TC_SNS_004")
        return runFrontLeftWheelSpeedMeasurementTest();

    if (testId == "TC_SNS_005")
        return runFrontRightWheelSpeedMeasurementTest();

    if (testId == "TC_SNS_006")
        return runFrontLeftDropoutInjectionTest();

    // ==================================================
    // Powertrain
    // ==================================================

    if (testId == "TC_PT_001")
        return runPowertrainCanIdentifierTest();

    if (testId == "TC_PT_002")
        return runPowertrainCodecRoundtripTest();

    if (testId == "TC_PT_003")
        return runPowertrainStateTest();

    if (testId == "TC_PT_004")
        return runPowertrainVehicleSpeedTest();

    if (testId == "TC_PT_005")
        return runPowertrainEngineSpeedTest();

    if (testId == "TC_PT_006")
        return runPowertrainGearStateTest();

    if (testId == "TC_PT_007")
        return runPowertrainBrakeStateTest();

    // ==================================================
    // Steering
    // ==================================================

    if (testId == "TC_STR_001")
        return runSteeringCanIdentifierTest();

    if (testId == "TC_STR_002")
        return runSteeringCodecRoundtripTest();

    if (testId == "TC_STR_003")
        return runSteeringStateTest();

    if (testId == "TC_STR_004")
        return runSteeringAngleTest();

    // ==================================================
    // Diagnostics
    // ==================================================

    if (testId == "TC_DIA_001")
        return runFrontLeftDiagnosticFaultInjectionTest();

    if (testId == "TC_DIA_002")
        return runFrontRightDiagnosticFaultInjectionTest();

    if (testId == "TC_DIA_003")
        return runFrontLeftDiagnosticFaultClearTest();

    if (testId == "TC_DIA_004")
        return runFrontRightDiagnosticFaultClearTest();

    if (testId == "TC_DIA_005")
        return runDiagnosticHealthObservabilityTest();

    if (testId == "TC_DIA_006")
        return runRuntimeFaultInjectionTest();

    if (testId == "TC_DIA_007")
        return runRuntimeFaultRecoveryTest();

    // ==================================================
    // DTC
    // ==================================================

    if (testId == "TC_DTC_001")
        return runFrontLeftDtcDefinitionTest();

    if (testId == "TC_DTC_002")
        return runFrontRightDtcDefinitionTest();

    if (testId == "TC_DTC_003")
        return runFrontLeftDtcActivationTest();

    if (testId == "TC_DTC_004")
        return runFrontRightDtcActivationTest();

    if (testId == "TC_DTC_005")
        return runFrontLeftDtcRecoveryTest();

    if (testId == "TC_DTC_006")
        return runFrontRightDtcRecoveryTest();

    if (testId == "TC_DTC_007")
        return runIndividualDtcClearTest();

    if (testId == "TC_DTC_008")
        return runClearAllDtcsTest();

    if (testId == "TC_DTC_009")
        return runDtcFirstDetectionTimestampTest();

    if (testId == "TC_DTC_010")
        return runDtcLastDetectionTimestampTest();

    if (testId == "TC_DTC_011")
        return runDtcOccurrenceCountTest();

    if (testId == "TC_DTC_012")
        return runDtcLookupTest();

    if (testId == "TC_DTC_013")
        return runDtcCollectionAccessTest();

    if (testId == "TC_DTC_014")
        return runActiveDtcQueryTest();

    if (testId == "TC_DTC_015")
        return runRuntimeDtcDetectionTest();

    if (testId == "TC_DTC_016")
        return runRuntimeDtcRecoveryTest();

    if (testId == "TC_DTC_017")
        return runDtcResetTest();

    if (testId == "TC_DTC_018")
        return runDtcDeterminismTest();

    // ==================================================
    // CAN
    // ==================================================

    if (testId == "TC_CAN_001")
        return runCanArbitrationPriorityTest();

    if (testId == "TC_CAN_002")
        return runCanBitrateTest();

    if (testId == "TC_CAN_003")
        return runCanTraceRecordingTest();

    if (testId == "TC_CAN_004")
        return runCanArbitrationStatisticsTest();

    if (testId == "TC_CAN_005")
        return runAbsMessagePeriodicityTest();

    if (testId == "TC_CAN_006")
        return runPowertrainMessagePeriodicityTest();

    if (testId == "TC_CAN_007")
        return runSteeringMessagePeriodicityTest();

    if (testId == "TC_CAN_008")
        return runCanBusUtilizationTest();

    if (testId == "TC_CAN_009")
        return runCanTraceTimingEvidenceTest();

    if (testId == "TC_CAN_010")
        return runCanTransmissionDurationTest();

    if (testId == "TC_CAN_011")
        return runCanWaitingTimeTest();

    if (testId == "TC_CAN_012")
        return runCanFrameCountTest();

    if (testId == "TC_CAN_013")
        return runCanAverageWaitingTimeTest();

    if (testId == "TC_CAN_014")
        return runCanMaximumWaitingTimeTest();

    if (testId == "TC_CAN_015")
        return runCanTotalTransmissionTimeTest();

    if (testId == "TC_CAN_016")
        return runCanPerMessageStatisticsTest();

    if (testId == "TC_CAN_017")
        return runCanPendingFrameBehaviorTest();

    if (testId == "TC_CAN_018")
        return runCanWaitingTimeTraceEvidenceTest();

    if (testId == "TC_CAN_019")
        return runCanExclusiveBusAccessTest();

    if (testId == "TC_CAN_020")
        return runCanTraceIdentifierTest();

    if (testId == "TC_CAN_021")
        return runCanTracePayloadTest();

    if (testId == "TC_CAN_022")
        return runCanMultipleEcuRequestsTest();

    // ==================================================
    // Messages
    // ==================================================

    if (testId == "TC_MSG_001")
        return runAbsCodecRoundtripTest();

    if (testId == "TC_MSG_002")
        return runAbsMessageIdentifierTest();

    if (testId == "TC_MSG_003")
        return runAbsMessageStateInformationTest();

    if (testId == "TC_MSG_004")
        return runPowertrainMessageIdentifierTest();

    if (testId == "TC_MSG_005")
        return runPowertrainMessageVehicleSpeedTest();

    if (testId == "TC_MSG_006")
        return runPowertrainMessageEngineRpmTest();

    if (testId == "TC_MSG_007")
        return runPowertrainMessageGearStateTest();

    if (testId == "TC_MSG_008")
        return runPowertrainMessageBrakeStateTest();

    if (testId == "TC_MSG_009")
        return runSteeringMessageIdentifierTest();

    if (testId == "TC_MSG_010")
        return runSteeringMessageAngleTest();

    // ==================================================
    // Validation
    // ==================================================

    if (testId == "TC_VAL_001")
        return runIndividualTestExecutionVerification();

    if (testId == "TC_VAL_002")
        return runValidationTestIsolationVerification();

    if (testId == "TC_VAL_003")
        return runTestStatusVerification();

    if (testId == "TC_VAL_004")
        return runExecutionTimeVerification();

    if (testId == "TC_VAL_005")
        return runResponseTimeEvidenceVerification();

    if (testId == "TC_VAL_006")
        return runTimingPassCriteriaVerification();

    // ==================================================
    // Unknown
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
// Run complete test suite
// ==================================================

std::vector<TestCase>
TestRunner::runAll()
{
    std::vector<TestCase> results;

    results.reserve(
        102
    );

    // ==================================================
    // System
    // ==================================================

    results.push_back(runSimulationTimeProgressionTest());
    results.push_back(runSimulationResetTest());
    results.push_back(runEcuCommunicationTest());
    results.push_back(runValidationIsolationTest());

    // ==================================================
    // Vehicle
    // ==================================================

    results.push_back(runAccelerationScenarioTest());
    results.push_back(runCruiseScenarioTest());
    results.push_back(runHardBrakingScenarioTest());
    results.push_back(runRecoveryScenarioTest());
    results.push_back(runVehicleSpeedStateTest());
    results.push_back(runFrontLeftWheelSpeedStateTest());
    results.push_back(runFrontRightWheelSpeedStateTest());
    results.push_back(runBrakeInputStateTest());
    results.push_back(runSteeringAngleStateTest());
    results.push_back(runVehicleDynamicStateUpdateTest());
    results.push_back(runIndependentWheelSpeedsTest());

    // ==================================================
    // ABS
    // ==================================================

    results.push_back(runAbsEmergencyBrakingTest());
    results.push_back(runFrontLeftWheelSensorDropoutTest());
    results.push_back(runFrontRightWheelSensorDropoutTest());
    results.push_back(runAbsHealthyStateTest());
    results.push_back(runAbsInterventionStateObservabilityTest());
    results.push_back(runAbsCanIdentifierTest());
    results.push_back(runAbsWheelSlipProcessingTest());

    // ==================================================
    // Sensors
    // ==================================================

    results.push_back(runFrontLeftSensorDropoutTest());
    results.push_back(runFrontRightSensorDropoutTest());
    results.push_back(runSensorFaultRecoveryTest());
    results.push_back(runFrontLeftWheelSpeedMeasurementTest());
    results.push_back(runFrontRightWheelSpeedMeasurementTest());
    results.push_back(runFrontLeftDropoutInjectionTest());

    // ==================================================
    // Powertrain
    // ==================================================

    results.push_back(runPowertrainCanIdentifierTest());
    results.push_back(runPowertrainCodecRoundtripTest());
    results.push_back(runPowertrainStateTest());
    results.push_back(runPowertrainVehicleSpeedTest());
    results.push_back(runPowertrainEngineSpeedTest());
    results.push_back(runPowertrainGearStateTest());
    results.push_back(runPowertrainBrakeStateTest());

    // ==================================================
    // Steering
    // ==================================================

    results.push_back(runSteeringCanIdentifierTest());
    results.push_back(runSteeringCodecRoundtripTest());
    results.push_back(runSteeringStateTest());
    results.push_back(runSteeringAngleTest());

    // ==================================================
    // Diagnostics
    // ==================================================

    results.push_back(runFrontLeftDiagnosticFaultInjectionTest());
    results.push_back(runFrontRightDiagnosticFaultInjectionTest());
    results.push_back(runFrontLeftDiagnosticFaultClearTest());
    results.push_back(runFrontRightDiagnosticFaultClearTest());
    results.push_back(runDiagnosticHealthObservabilityTest());
    results.push_back(runRuntimeFaultInjectionTest());
    results.push_back(runRuntimeFaultRecoveryTest());

    // ==================================================
    // DTC
    // ==================================================

    results.push_back(runFrontLeftDtcDefinitionTest());
    results.push_back(runFrontRightDtcDefinitionTest());
    results.push_back(runFrontLeftDtcActivationTest());
    results.push_back(runFrontRightDtcActivationTest());
    results.push_back(runFrontLeftDtcRecoveryTest());
    results.push_back(runFrontRightDtcRecoveryTest());
    results.push_back(runIndividualDtcClearTest());
    results.push_back(runClearAllDtcsTest());
    results.push_back(runDtcFirstDetectionTimestampTest());
    results.push_back(runDtcLastDetectionTimestampTest());
    results.push_back(runDtcOccurrenceCountTest());
    results.push_back(runDtcLookupTest());
    results.push_back(runDtcCollectionAccessTest());
    results.push_back(runActiveDtcQueryTest());
    results.push_back(runRuntimeDtcDetectionTest());
    results.push_back(runRuntimeDtcRecoveryTest());
    results.push_back(runDtcResetTest());
    results.push_back(runDtcDeterminismTest());

    // ==================================================
    // CAN
    // ==================================================

    results.push_back(runCanArbitrationPriorityTest());
    results.push_back(runCanBitrateTest());
    results.push_back(runCanTraceRecordingTest());
    results.push_back(runCanArbitrationStatisticsTest());
    results.push_back(runAbsMessagePeriodicityTest());
    results.push_back(runPowertrainMessagePeriodicityTest());
    results.push_back(runSteeringMessagePeriodicityTest());
    results.push_back(runCanBusUtilizationTest());
    results.push_back(runCanTraceTimingEvidenceTest());
    results.push_back(runCanTransmissionDurationTest());
    results.push_back(runCanWaitingTimeTest());
    results.push_back(runCanFrameCountTest());
    results.push_back(runCanAverageWaitingTimeTest());
    results.push_back(runCanMaximumWaitingTimeTest());
    results.push_back(runCanTotalTransmissionTimeTest());
    results.push_back(runCanPerMessageStatisticsTest());
    results.push_back(runCanPendingFrameBehaviorTest());
    results.push_back(runCanWaitingTimeTraceEvidenceTest());
    results.push_back(runCanExclusiveBusAccessTest());
    results.push_back(runCanTraceIdentifierTest());
    results.push_back(runCanTracePayloadTest());
    results.push_back(runCanMultipleEcuRequestsTest());

    // ==================================================
    // Messages
    // ==================================================

    results.push_back(runAbsCodecRoundtripTest());
    results.push_back(runAbsMessageIdentifierTest());
    results.push_back(runAbsMessageStateInformationTest());
    results.push_back(runPowertrainMessageIdentifierTest());
    results.push_back(runPowertrainMessageVehicleSpeedTest());
    results.push_back(runPowertrainMessageEngineRpmTest());
    results.push_back(runPowertrainMessageGearStateTest());
    results.push_back(runPowertrainMessageBrakeStateTest());
    results.push_back(runSteeringMessageIdentifierTest());
    results.push_back(runSteeringMessageAngleTest());

    // ==================================================
    // Validation
    // ==================================================

    results.push_back(runIndividualTestExecutionVerification());
    results.push_back(runValidationTestIsolationVerification());
    results.push_back(runTestStatusVerification());
    results.push_back(runExecutionTimeVerification());
    results.push_back(runResponseTimeEvidenceVerification());
    results.push_back(runTimingPassCriteriaVerification());

    return results;
}