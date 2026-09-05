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
    // UDS
    // ==================================================

    if (testId == "TC_UDS_001")
        return runUdsRequestRepresentationTest();

    if (testId == "TC_UDS_002")
        return runUdsResponseRepresentationTest();

    if (testId == "TC_UDS_003")
        return runUdsServerProcessingTest();

    if (testId == "TC_UDS_004")
        return runUdsPositiveResponseSidTest();

    if (testId == "TC_UDS_005")
        return runUdsNegativeResponseFormatTest();

    if (testId == "TC_UDS_006")
        return runUdsUnsupportedServiceTest();

    if (testId == "TC_UDS_007")
        return runUdsDiagnosticSessionControlSupportTest();

    if (testId == "TC_UDS_008")
        return runUdsDefaultSessionTest();

    if (testId == "TC_UDS_009")
        return runUdsExtendedSessionTest();

    if (testId == "TC_UDS_010")
        return runUdsSessionStateUpdateTest();

    if (testId == "TC_UDS_011")
        return runUdsUnsupportedSessionTest();

    if (testId == "TC_UDS_012")
        return runUdsReadDtcInformationSupportTest();

    if (testId == "TC_UDS_013")
        return runUdsReadActiveDtcsTest();

    if (testId == "TC_UDS_014")
        return runUdsFrontLeftDtcReportingTest();

    if (testId == "TC_UDS_015")
        return runUdsFrontRightDtcReportingTest();

    if (testId == "TC_UDS_016")
        return runUdsNoActiveDtcResponseTest();

    if (testId == "TC_UDS_017")
        return runUdsClearDiagnosticInformationSupportTest();

    if (testId == "TC_UDS_018")
        return runUdsClearAllDtcsTest();

    if (testId == "TC_UDS_019")
        return runUdsClearDiagnosticResponseTest();

    if (testId == "TC_UDS_020")
        return runUdsReadDataByIdentifierSupportTest();

    if (testId == "TC_UDS_021")
        return runUdsVehicleSpeedDidTest();

    if (testId == "TC_UDS_022")
        return runUdsVehicleSpeedReadoutTest();

    if (testId == "TC_UDS_023")
        return runUdsEngineSpeedDidTest();

    if (testId == "TC_UDS_024")
        return runUdsEngineSpeedReadoutTest();

    if (testId == "TC_UDS_025")
        return runUdsSteeringAngleDidTest();

    if (testId == "TC_UDS_026")
        return runUdsSteeringAngleReadoutTest();

    if (testId == "TC_UDS_027")
        return runUdsUnsupportedDidTest();

    if (testId == "TC_UDS_028")
        return runUdsDtcManagerAccessTest();

    if (testId == "TC_UDS_029")
        return runUdsDtcClearIntegrationTest();

    if (testId == "TC_UDS_030")
        return runUdsRuntimeDiagnosticProcessingTest();

    if (testId == "TC_UDS_031")
        return runUdsCurrentVehicleDataTest();

    if (testId == "TC_UDS_032")
        return runUdsResetStateTest();

    if (testId == "TC_UDS_033")
        return runUdsDeterminismTest();

    // ==================================================
    // ISO-TP
    // ==================================================

    if (testId == "TC_ISOTP_001")
        return runIsoTpTransportComponentTest();

    if (testId == "TC_ISOTP_002")
        return runIsoTpClassicCanFrameSizeTest();

    if (testId == "TC_ISOTP_003")
        return runIsoTpStandardCanIdentifierTest();

    if (testId == "TC_ISOTP_004")
        return runIsoTpMaximumPayloadTest();

    if (testId == "TC_ISOTP_005")
        return runIsoTpEmptyPayloadRejectionTest();

    if (testId == "TC_ISOTP_006")
        return runIsoTpSingleFrameSupportTest();

    if (testId == "TC_ISOTP_007")
        return runIsoTpSingleFramePayloadCapacityTest();

    if (testId == "TC_ISOTP_008")
        return runIsoTpSingleFramePciTest();

    if (testId == "TC_ISOTP_009")
        return runIsoTpFirstFrameSupportTest();

    if (testId == "TC_ISOTP_010")
        return runIsoTpFirstFramePayloadLengthTest();

    if (testId == "TC_ISOTP_011")
        return runIsoTpFirstFrameDataCapacityTest();

    if (testId == "TC_ISOTP_012")
        return runIsoTpConsecutiveFrameSupportTest();

    if (testId == "TC_ISOTP_013")
        return runIsoTpConsecutiveFrameDataCapacityTest();

    if (testId == "TC_ISOTP_014")
        return runIsoTpInitialSequenceNumberTest();

    if (testId == "TC_ISOTP_015")
        return runIsoTpSequenceNumberIncrementTest();

    if (testId == "TC_ISOTP_016")
        return runIsoTpSequenceNumberRolloverTest();

    if (testId == "TC_ISOTP_017")
        return runIsoTpFlowControlFrameSupportTest();

    if (testId == "TC_ISOTP_018")
        return runIsoTpFlowStatusTest();

    if (testId == "TC_ISOTP_019")
        return runIsoTpFlowControlParametersTest();

    if (testId == "TC_ISOTP_020")
        return runIsoTpSingleFrameReassemblyTest();

    if (testId == "TC_ISOTP_021")
        return runIsoTpMultiFrameReassemblyTest();

    if (testId == "TC_ISOTP_022")
        return runIsoTpSequenceValidationTest();

    if (testId == "TC_ISOTP_023")
        return runIsoTpCanIdentifierValidationTest();

    if (testId == "TC_ISOTP_024")
        return runIsoTpIncompleteMessageDetectionTest();

    if (testId == "TC_ISOTP_025")
        return runIsoTpDeterministicSegmentationTest();

    if (testId == "TC_ISOTP_026")
        return runIsoTpDeterministicReassemblyTest();

    // ==================================================
    // UDS Transport
    // ==================================================

    if (testId == "TC_UDSTP_001")
        return runUdsTransportRequestTest();

    if (testId == "TC_UDSTP_002")
        return runUdsTransportResponseTest();

    if (testId == "TC_UDSTP_003")
        return runUdsTransportRequestCanIdTest();

    if (testId == "TC_UDSTP_004")
        return runUdsTransportResponseCanIdTest();

    if (testId == "TC_UDSTP_005")
        return runUdsTransportRequestSerializationTest();

    if (testId == "TC_UDSTP_006")
        return runUdsTransportResponseSerializationTest();

    if (testId == "TC_UDSTP_007")
        return runUdsTransportPositiveResponseTest();

    if (testId == "TC_UDSTP_008")
        return runUdsTransportNegativeResponseTest();

    if (testId == "TC_UDSTP_009")
        return runUdsTransportRequestIdentifierValidationTest();

    if (testId == "TC_UDSTP_010")
        return runUdsTransportResponseIdentifierValidationTest();

    if (testId == "TC_UDSTP_011")
        return runUdsTransportEndToEndTest();

    if (testId == "TC_UDSTP_012")
        return runUdsTransportDeterminismTest();

    // ==================================================
    // UDS CAN Transport
    // ==================================================

    if (testId == "TC_UDSCAN_001")
        return runUdsCanTransportComponentTest();

    if (testId == "TC_UDSCAN_002")
        return runUdsCanRequestTransmissionTest();

    if (testId == "TC_UDSCAN_003")
        return runUdsCanResponseTransmissionTest();

    if (testId == "TC_UDSCAN_004")
        return runUdsCanRequestIdentifierTest();

    if (testId == "TC_UDSCAN_005")
        return runUdsCanResponseIdentifierTest();

    if (testId == "TC_UDSCAN_006")
        return runUdsCanRequestReassemblyTest();

    if (testId == "TC_UDSCAN_007")
        return runUdsCanResponseReassemblyTest();

    if (testId == "TC_UDSCAN_008")
        return runUdsCanEndToEndTransactionTest();

    if (testId == "TC_UDSCAN_009")
        return runUdsCanTraceEvidenceTest();

    if (testId == "TC_UDSCAN_010")
        return runUdsCanTimingTest();

    if (testId == "TC_UDSCAN_011")
        return runUdsCanTransactionEvidenceTest();

    if (testId == "TC_UDSCAN_012")
        return runUdsCanPositiveResponseTest();

    if (testId == "TC_UDSCAN_013")
        return runUdsCanNegativeResponseTest();

    if (testId == "TC_UDSCAN_014")
        return runUdsCanMultiFrameTransportTest();

    if (testId == "TC_UDSCAN_015")
        return runUdsCanDeterminismTest();

    // ==================================================
    // UDS Runtime Integration
    // ==================================================

    if (testId == "TC_UDSRT_001")
        return runUdsRuntimeRequestSubmissionTest();

    if (testId == "TC_UDSRT_002")
        return runUdsRuntimeRequestIdentifierTest();

    if (testId == "TC_UDSRT_003")
        return runUdsRuntimeRequestSegmentationTest();

    if (testId == "TC_UDSRT_004")
        return runUdsRuntimeSharedBusRequestTest();

    if (testId == "TC_UDSRT_005")
        return runUdsRuntimeSharedArbitrationTest();

    if (testId == "TC_UDSRT_006")
        return runUdsRuntimePriorityPreservationTest();

    if (testId == "TC_UDSRT_007")
        return runUdsRuntimeNoTrafficLossTest();

    if (testId == "TC_UDSRT_008")
        return runUdsRuntimeCentralRoutingTest();

    if (testId == "TC_UDSRT_009")
        return runUdsRuntimeVehicleRoutingTest();

    if (testId == "TC_UDSRT_010")
        return runUdsRuntimeRequestRoutingTest();

    if (testId == "TC_UDSRT_011")
        return runUdsRuntimeResponseRoutingTest();

    if (testId == "TC_UDSRT_012")
        return runUdsRuntimeReassemblyProcessingTest();

    if (testId == "TC_UDSRT_013")
        return runUdsRuntimeCurrentVehicleDataTest();

    if (testId == "TC_UDSRT_014")
        return runUdsRuntimeResponseGenerationTest();

    if (testId == "TC_UDSRT_015")
        return runUdsRuntimeResponseIdentifierTest();

    if (testId == "TC_UDSRT_016")
        return runUdsRuntimeSharedBusResponseTest();

    if (testId == "TC_UDSRT_017")
        return runUdsRuntimeResponseReassemblyTest();

    if (testId == "TC_UDSRT_018")
        return runUdsRuntimeTraceTest();

    if (testId == "TC_UDSRT_019")
        return runUdsRuntimeStatisticsTest();

    if (testId == "TC_UDSRT_020")
        return runUdsRuntimeTimingTest();

    if (testId == "TC_UDSRT_021")
        return runUdsRuntimePendingStateTest();

    if (testId == "TC_UDSRT_022")
        return runUdsRuntimeCompletionTest();

    if (testId == "TC_UDSRT_023")
        return runUdsRuntimeResultAccessTest();

    if (testId == "TC_UDSRT_024")
        return runUdsRuntimeResetStateTest();

    if (testId == "TC_UDSRT_025")
        return runUdsRuntimeResetCanIsolationTest();

    if (testId == "TC_UDSRT_026")
        return runUdsRuntimeEcuRegressionTest();

    if (testId == "TC_UDSRT_027")
        return runUdsRuntimeDiagnosticRegressionTest();

    if (testId == "TC_UDSRT_028")
        return runUdsRuntimeBaselineRegressionTest();

    if (testId == "TC_UDSRT_029")
        return runUdsRuntimeDeterminismTest();

    // ==================================================
    // Automotive Ethernet
    // ==================================================

    if (testId == "TC_ETH_001")
        return runEthernetFrameRepresentationTest();
    if (testId == "TC_ETH_002")
        return runEthernetSourceMacAddressTest();
    if (testId == "TC_ETH_003")
        return runEthernetDestinationMacAddressTest();
    if (testId == "TC_ETH_004")
        return runEthernetEtherTypeTest();
    if (testId == "TC_ETH_005")
        return runEthernetPayloadTest();
    if (testId == "TC_ETH_006")
        return runEthernetPayloadPreservationTest();
    if (testId == "TC_ETH_007")
        return runEthernetMacAddressRepresentationTest();
    if (testId == "TC_ETH_008")
        return runEthernetMacAddressEqualityTest();
    if (testId == "TC_ETH_009")
        return runEthernetDeterministicNodeAddressingTest();
    if (testId == "TC_ETH_010")
        return runVirtualEthernetBusComponentTest();
    if (testId == "TC_ETH_011")
        return runEthernetFrameTransmissionRequestTest();
    if (testId == "TC_ETH_012")
        return runEthernetFrameDeliveryTest();
    if (testId == "TC_ETH_013")
        return runEthernetUnknownDestinationTest();
    if (testId == "TC_ETH_014")
        return runEthernetMultipleNodesTest();
    if (testId == "TC_ETH_015")
        return runEthernetLinkRateTest();
    if (testId == "TC_ETH_016")
        return runEthernetTransmissionDurationTest();
    if (testId == "TC_ETH_017")
        return runEthernetBusTimeProgressionTest();
    if (testId == "TC_ETH_018")
        return runEthernetBusyStateTest();
    if (testId == "TC_ETH_019")
        return runEthernetTraceRecordingTest();
    if (testId == "TC_ETH_020")
        return runEthernetTraceAddressEvidenceTest();
    if (testId == "TC_ETH_021")
        return runEthernetTraceEtherTypeEvidenceTest();
    if (testId == "TC_ETH_022")
        return runEthernetTracePayloadEvidenceTest();
    if (testId == "TC_ETH_023")
        return runEthernetTraceTimingEvidenceTest();
    if (testId == "TC_ETH_024")
        return runEthernetFrameCountTest();
    if (testId == "TC_ETH_025")
        return runEthernetByteCountTest();
    if (testId == "TC_ETH_026")
        return runEthernetPerNodeStatisticsTest();
    if (testId == "TC_ETH_027")
        return runVirtualEthernetNodeTest();
    if (testId == "TC_ETH_028")
        return runEthernetNodeReceiveFilteringTest();
    if (testId == "TC_ETH_029")
        return runEthernetNodeTransmissionTest();
    if (testId == "TC_ETH_030")
        return runSimulationEthernetOwnershipTest();
    if (testId == "TC_ETH_031")
        return runCanEthernetCoexistenceTest();
    if (testId == "TC_ETH_032")
        return runEthernetRuntimeProcessingTest();
    if (testId == "TC_ETH_033")
        return runEthernetRuntimeResetTest();
    if (testId == "TC_ETH_034")
        return runEthernetCanRegressionTest();
    if (testId == "TC_ETH_035")
        return runEthernetDiagnosticRegressionTest();
    if (testId == "TC_ETH_036")
        return runEthernetBaselineRegressionTest();
    if (testId == "TC_ETH_037")
        return runEthernetDeterminismTest();

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
    // Unknown Test
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
        254
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
    // UDS
    // ==================================================

    results.push_back(runUdsRequestRepresentationTest());
    results.push_back(runUdsResponseRepresentationTest());
    results.push_back(runUdsServerProcessingTest());
    results.push_back(runUdsPositiveResponseSidTest());
    results.push_back(runUdsNegativeResponseFormatTest());
    results.push_back(runUdsUnsupportedServiceTest());

    results.push_back(runUdsDiagnosticSessionControlSupportTest());
    results.push_back(runUdsDefaultSessionTest());
    results.push_back(runUdsExtendedSessionTest());
    results.push_back(runUdsSessionStateUpdateTest());
    results.push_back(runUdsUnsupportedSessionTest());

    results.push_back(runUdsReadDtcInformationSupportTest());
    results.push_back(runUdsReadActiveDtcsTest());
    results.push_back(runUdsFrontLeftDtcReportingTest());
    results.push_back(runUdsFrontRightDtcReportingTest());
    results.push_back(runUdsNoActiveDtcResponseTest());

    results.push_back(runUdsClearDiagnosticInformationSupportTest());
    results.push_back(runUdsClearAllDtcsTest());
    results.push_back(runUdsClearDiagnosticResponseTest());

    results.push_back(runUdsReadDataByIdentifierSupportTest());
    results.push_back(runUdsVehicleSpeedDidTest());
    results.push_back(runUdsVehicleSpeedReadoutTest());
    results.push_back(runUdsEngineSpeedDidTest());
    results.push_back(runUdsEngineSpeedReadoutTest());
    results.push_back(runUdsSteeringAngleDidTest());
    results.push_back(runUdsSteeringAngleReadoutTest());
    results.push_back(runUdsUnsupportedDidTest());

    results.push_back(runUdsDtcManagerAccessTest());
    results.push_back(runUdsDtcClearIntegrationTest());

    results.push_back(runUdsRuntimeDiagnosticProcessingTest());
    results.push_back(runUdsCurrentVehicleDataTest());

    results.push_back(runUdsResetStateTest());
    results.push_back(runUdsDeterminismTest());

    // ==================================================
    // ISO-TP
    // ==================================================

    results.push_back(runIsoTpTransportComponentTest());
    results.push_back(runIsoTpClassicCanFrameSizeTest());
    results.push_back(runIsoTpStandardCanIdentifierTest());
    results.push_back(runIsoTpMaximumPayloadTest());
    results.push_back(runIsoTpEmptyPayloadRejectionTest());

    results.push_back(runIsoTpSingleFrameSupportTest());
    results.push_back(runIsoTpSingleFramePayloadCapacityTest());
    results.push_back(runIsoTpSingleFramePciTest());

    results.push_back(runIsoTpFirstFrameSupportTest());
    results.push_back(runIsoTpFirstFramePayloadLengthTest());
    results.push_back(runIsoTpFirstFrameDataCapacityTest());

    results.push_back(runIsoTpConsecutiveFrameSupportTest());
    results.push_back(runIsoTpConsecutiveFrameDataCapacityTest());
    results.push_back(runIsoTpInitialSequenceNumberTest());
    results.push_back(runIsoTpSequenceNumberIncrementTest());
    results.push_back(runIsoTpSequenceNumberRolloverTest());

    results.push_back(runIsoTpFlowControlFrameSupportTest());
    results.push_back(runIsoTpFlowStatusTest());
    results.push_back(runIsoTpFlowControlParametersTest());

    results.push_back(runIsoTpSingleFrameReassemblyTest());
    results.push_back(runIsoTpMultiFrameReassemblyTest());
    results.push_back(runIsoTpSequenceValidationTest());
    results.push_back(runIsoTpCanIdentifierValidationTest());
    results.push_back(runIsoTpIncompleteMessageDetectionTest());

    results.push_back(runIsoTpDeterministicSegmentationTest());
    results.push_back(runIsoTpDeterministicReassemblyTest());

    // ==================================================
    // UDS Transport
    // ==================================================

    results.push_back(runUdsTransportRequestTest());
    results.push_back(runUdsTransportResponseTest());
    results.push_back(runUdsTransportRequestCanIdTest());
    results.push_back(runUdsTransportResponseCanIdTest());
    results.push_back(runUdsTransportRequestSerializationTest());
    results.push_back(runUdsTransportResponseSerializationTest());
    results.push_back(runUdsTransportPositiveResponseTest());
    results.push_back(runUdsTransportNegativeResponseTest());
    results.push_back(runUdsTransportRequestIdentifierValidationTest());
    results.push_back(runUdsTransportResponseIdentifierValidationTest());
    results.push_back(runUdsTransportEndToEndTest());
    results.push_back(runUdsTransportDeterminismTest());

    // ==================================================
    // UDS CAN Transport
    // ==================================================

    results.push_back(runUdsCanTransportComponentTest());
    results.push_back(runUdsCanRequestTransmissionTest());
    results.push_back(runUdsCanResponseTransmissionTest());
    results.push_back(runUdsCanRequestIdentifierTest());
    results.push_back(runUdsCanResponseIdentifierTest());
    results.push_back(runUdsCanRequestReassemblyTest());
    results.push_back(runUdsCanResponseReassemblyTest());
    results.push_back(runUdsCanEndToEndTransactionTest());
    results.push_back(runUdsCanTraceEvidenceTest());
    results.push_back(runUdsCanTimingTest());
    results.push_back(runUdsCanTransactionEvidenceTest());
    results.push_back(runUdsCanPositiveResponseTest());
    results.push_back(runUdsCanNegativeResponseTest());
    results.push_back(runUdsCanMultiFrameTransportTest());
    results.push_back(runUdsCanDeterminismTest());

    // ==================================================
    // UDS Runtime Integration
    // ==================================================

    results.push_back(runUdsRuntimeRequestSubmissionTest());
    results.push_back(runUdsRuntimeRequestIdentifierTest());
    results.push_back(runUdsRuntimeRequestSegmentationTest());
    results.push_back(runUdsRuntimeSharedBusRequestTest());
    results.push_back(runUdsRuntimeSharedArbitrationTest());
    results.push_back(runUdsRuntimePriorityPreservationTest());
    results.push_back(runUdsRuntimeNoTrafficLossTest());
    results.push_back(runUdsRuntimeCentralRoutingTest());
    results.push_back(runUdsRuntimeVehicleRoutingTest());
    results.push_back(runUdsRuntimeRequestRoutingTest());
    results.push_back(runUdsRuntimeResponseRoutingTest());
    results.push_back(runUdsRuntimeReassemblyProcessingTest());
    results.push_back(runUdsRuntimeCurrentVehicleDataTest());
    results.push_back(runUdsRuntimeResponseGenerationTest());
    results.push_back(runUdsRuntimeResponseIdentifierTest());
    results.push_back(runUdsRuntimeSharedBusResponseTest());
    results.push_back(runUdsRuntimeResponseReassemblyTest());
    results.push_back(runUdsRuntimeTraceTest());
    results.push_back(runUdsRuntimeStatisticsTest());
    results.push_back(runUdsRuntimeTimingTest());
    results.push_back(runUdsRuntimePendingStateTest());
    results.push_back(runUdsRuntimeCompletionTest());
    results.push_back(runUdsRuntimeResultAccessTest());
    results.push_back(runUdsRuntimeResetStateTest());
    results.push_back(runUdsRuntimeResetCanIsolationTest());
    results.push_back(runUdsRuntimeEcuRegressionTest());
    results.push_back(runUdsRuntimeDiagnosticRegressionTest());
    results.push_back(runUdsRuntimeBaselineRegressionTest());
    results.push_back(runUdsRuntimeDeterminismTest());

    // ==================================================
    // Automotive Ethernet
    // ==================================================

    results.push_back(runEthernetFrameRepresentationTest());
    results.push_back(runEthernetSourceMacAddressTest());
    results.push_back(runEthernetDestinationMacAddressTest());
    results.push_back(runEthernetEtherTypeTest());
    results.push_back(runEthernetPayloadTest());
    results.push_back(runEthernetPayloadPreservationTest());
    results.push_back(runEthernetMacAddressRepresentationTest());
    results.push_back(runEthernetMacAddressEqualityTest());
    results.push_back(runEthernetDeterministicNodeAddressingTest());
    results.push_back(runVirtualEthernetBusComponentTest());
    results.push_back(runEthernetFrameTransmissionRequestTest());
    results.push_back(runEthernetFrameDeliveryTest());
    results.push_back(runEthernetUnknownDestinationTest());
    results.push_back(runEthernetMultipleNodesTest());
    results.push_back(runEthernetLinkRateTest());
    results.push_back(runEthernetTransmissionDurationTest());
    results.push_back(runEthernetBusTimeProgressionTest());
    results.push_back(runEthernetBusyStateTest());
    results.push_back(runEthernetTraceRecordingTest());
    results.push_back(runEthernetTraceAddressEvidenceTest());
    results.push_back(runEthernetTraceEtherTypeEvidenceTest());
    results.push_back(runEthernetTracePayloadEvidenceTest());
    results.push_back(runEthernetTraceTimingEvidenceTest());
    results.push_back(runEthernetFrameCountTest());
    results.push_back(runEthernetByteCountTest());
    results.push_back(runEthernetPerNodeStatisticsTest());
    results.push_back(runVirtualEthernetNodeTest());
    results.push_back(runEthernetNodeReceiveFilteringTest());
    results.push_back(runEthernetNodeTransmissionTest());
    results.push_back(runSimulationEthernetOwnershipTest());
    results.push_back(runCanEthernetCoexistenceTest());
    results.push_back(runEthernetRuntimeProcessingTest());
    results.push_back(runEthernetRuntimeResetTest());
    results.push_back(runEthernetCanRegressionTest());
    results.push_back(runEthernetDiagnosticRegressionTest());
    results.push_back(runEthernetBaselineRegressionTest());
    results.push_back(runEthernetDeterminismTest());

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