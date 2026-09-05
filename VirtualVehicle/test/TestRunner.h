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
    // ISO-TP Tests
    // ==================================================

    TestCase runIsoTpTransportComponentTest();
    TestCase runIsoTpClassicCanFrameSizeTest();
    TestCase runIsoTpStandardCanIdentifierTest();
    TestCase runIsoTpMaximumPayloadTest();
    TestCase runIsoTpEmptyPayloadRejectionTest();
    TestCase runIsoTpSingleFrameSupportTest();
    TestCase runIsoTpSingleFramePayloadCapacityTest();
    TestCase runIsoTpSingleFramePciTest();
    TestCase runIsoTpFirstFrameSupportTest();
    TestCase runIsoTpFirstFramePayloadLengthTest();
    TestCase runIsoTpFirstFrameDataCapacityTest();
    TestCase runIsoTpConsecutiveFrameSupportTest();
    TestCase runIsoTpConsecutiveFrameDataCapacityTest();
    TestCase runIsoTpInitialSequenceNumberTest();
    TestCase runIsoTpSequenceNumberIncrementTest();
    TestCase runIsoTpSequenceNumberRolloverTest();
    TestCase runIsoTpFlowControlFrameSupportTest();
    TestCase runIsoTpFlowStatusTest();
    TestCase runIsoTpFlowControlParametersTest();
    TestCase runIsoTpSingleFrameReassemblyTest();
    TestCase runIsoTpMultiFrameReassemblyTest();
    TestCase runIsoTpSequenceValidationTest();
    TestCase runIsoTpCanIdentifierValidationTest();
    TestCase runIsoTpIncompleteMessageDetectionTest();
    TestCase runIsoTpDeterministicSegmentationTest();
    TestCase runIsoTpDeterministicReassemblyTest();

    // ==================================================
    // UDS Transport Tests
    // ==================================================

    TestCase runUdsTransportRequestTest();
    TestCase runUdsTransportResponseTest();
    TestCase runUdsTransportRequestCanIdTest();
    TestCase runUdsTransportResponseCanIdTest();
    TestCase runUdsTransportRequestSerializationTest();
    TestCase runUdsTransportResponseSerializationTest();
    TestCase runUdsTransportPositiveResponseTest();
    TestCase runUdsTransportNegativeResponseTest();
    TestCase runUdsTransportRequestIdentifierValidationTest();
    TestCase runUdsTransportResponseIdentifierValidationTest();
    TestCase runUdsTransportEndToEndTest();
    TestCase runUdsTransportDeterminismTest();

    // ==================================================
    // UDS CAN Transport Tests
    // ==================================================

    TestCase runUdsCanTransportComponentTest();
    TestCase runUdsCanRequestTransmissionTest();
    TestCase runUdsCanResponseTransmissionTest();
    TestCase runUdsCanRequestIdentifierTest();
    TestCase runUdsCanResponseIdentifierTest();
    TestCase runUdsCanRequestReassemblyTest();
    TestCase runUdsCanResponseReassemblyTest();
    TestCase runUdsCanEndToEndTransactionTest();
    TestCase runUdsCanTraceEvidenceTest();
    TestCase runUdsCanTimingTest();
    TestCase runUdsCanTransactionEvidenceTest();
    TestCase runUdsCanPositiveResponseTest();
    TestCase runUdsCanNegativeResponseTest();
    TestCase runUdsCanMultiFrameTransportTest();
    TestCase runUdsCanDeterminismTest();

    // ==================================================
    // UDS Runtime Integration Tests
    // ==================================================

    TestCase runUdsRuntimeRequestSubmissionTest();
    TestCase runUdsRuntimeRequestIdentifierTest();
    TestCase runUdsRuntimeRequestSegmentationTest();
    TestCase runUdsRuntimeSharedBusRequestTest();
    TestCase runUdsRuntimeSharedArbitrationTest();
    TestCase runUdsRuntimePriorityPreservationTest();
    TestCase runUdsRuntimeNoTrafficLossTest();
    TestCase runUdsRuntimeCentralRoutingTest();
    TestCase runUdsRuntimeVehicleRoutingTest();
    TestCase runUdsRuntimeRequestRoutingTest();
    TestCase runUdsRuntimeResponseRoutingTest();
    TestCase runUdsRuntimeReassemblyProcessingTest();
    TestCase runUdsRuntimeCurrentVehicleDataTest();
    TestCase runUdsRuntimeResponseGenerationTest();
    TestCase runUdsRuntimeResponseIdentifierTest();
    TestCase runUdsRuntimeSharedBusResponseTest();
    TestCase runUdsRuntimeResponseReassemblyTest();
    TestCase runUdsRuntimeTraceTest();
    TestCase runUdsRuntimeStatisticsTest();
    TestCase runUdsRuntimeTimingTest();
    TestCase runUdsRuntimePendingStateTest();
    TestCase runUdsRuntimeCompletionTest();
    TestCase runUdsRuntimeResultAccessTest();
    TestCase runUdsRuntimeResetStateTest();
    TestCase runUdsRuntimeResetCanIsolationTest();
    TestCase runUdsRuntimeEcuRegressionTest();
    TestCase runUdsRuntimeDiagnosticRegressionTest();
    TestCase runUdsRuntimeBaselineRegressionTest();
    TestCase runUdsRuntimeDeterminismTest();

    // ==================================================
    // Automotive Ethernet Tests
    // ==================================================

    TestCase runEthernetFrameRepresentationTest();
    TestCase runEthernetSourceMacAddressTest();
    TestCase runEthernetDestinationMacAddressTest();
    TestCase runEthernetEtherTypeTest();
    TestCase runEthernetPayloadTest();
    TestCase runEthernetPayloadPreservationTest();
    TestCase runEthernetMacAddressRepresentationTest();
    TestCase runEthernetMacAddressEqualityTest();
    TestCase runEthernetDeterministicNodeAddressingTest();
    TestCase runVirtualEthernetBusComponentTest();
    TestCase runEthernetFrameTransmissionRequestTest();
    TestCase runEthernetFrameDeliveryTest();
    TestCase runEthernetUnknownDestinationTest();
    TestCase runEthernetMultipleNodesTest();
    TestCase runEthernetLinkRateTest();
    TestCase runEthernetTransmissionDurationTest();
    TestCase runEthernetBusTimeProgressionTest();
    TestCase runEthernetBusyStateTest();
    TestCase runEthernetTraceRecordingTest();
    TestCase runEthernetTraceAddressEvidenceTest();
    TestCase runEthernetTraceEtherTypeEvidenceTest();
    TestCase runEthernetTracePayloadEvidenceTest();
    TestCase runEthernetTraceTimingEvidenceTest();
    TestCase runEthernetFrameCountTest();
    TestCase runEthernetByteCountTest();
    TestCase runEthernetPerNodeStatisticsTest();
    TestCase runVirtualEthernetNodeTest();
    TestCase runEthernetNodeReceiveFilteringTest();
    TestCase runEthernetNodeTransmissionTest();
    TestCase runSimulationEthernetOwnershipTest();
    TestCase runCanEthernetCoexistenceTest();
    TestCase runEthernetRuntimeProcessingTest();
    TestCase runEthernetRuntimeResetTest();
    TestCase runEthernetCanRegressionTest();
    TestCase runEthernetDiagnosticRegressionTest();
    TestCase runEthernetBaselineRegressionTest();
    TestCase runEthernetDeterminismTest();

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