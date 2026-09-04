# VirtualVehicle Requirements Traceability Matrix

**Document ID:** VV-RTM-001  
**Baseline:** 1.5  
**Status:** Current Verification Record / Closed  
**Latest Automated Suite Result:** 188 Passed / 0 Failed

---

## 1. Purpose

This document provides project-level traceability between VirtualVehicle
requirements, implementation areas, automated verification test cases, and
verification results.

A requirement is marked **VERIFIED / PASS** only when the available evidence is
sufficient for direct verification.

Incidental or indirect exercise is recorded as supporting evidence and does not
by itself change formal verification status.

---

## 2. Automated Test Inventory

| Test Case | Primary Requirement | Verification Area | Result |
|---|---|---|---|
| TC_SYS_001 | SYS-REQ-001 | Simulation Time Progression | PASS |
| TC_SYS_002 | SYS-REQ-005 | Simulation Reset | PASS |
| TC_SYS_003 | SYS-REQ-003 | ECU Communication Through Virtual CAN | PASS |
| TC_SYS_004 | SYS-REQ-007 | Automated Validation Isolation | PASS |
| TC_VEH_001 | VEH-REQ-007 | Acceleration Scenario | PASS |
| TC_VEH_002 | SYS-REQ-002 | Cruise Scenario | PASS |
| TC_VEH_003 | VEH-REQ-008 | Hard Braking Scenario | PASS |
| TC_VEH_004 | SYS-REQ-002 | Recovery Scenario | PASS |
| TC_VEH_005 | VEH-REQ-001 | Vehicle Speed State | PASS |
| TC_VEH_006 | VEH-REQ-002 | Front-Left Wheel Speed State | PASS |
| TC_VEH_007 | VEH-REQ-003 | Front-Right Wheel Speed State | PASS |
| TC_VEH_008 | VEH-REQ-004 | Brake Input State | PASS |
| TC_VEH_009 | VEH-REQ-005 | Steering Angle State | PASS |
| TC_VEH_010 | VEH-REQ-006 | Vehicle Dynamic State Update | PASS |
| TC_VEH_011 | VEH-REQ-009 | Independent Wheel Speeds | PASS |
| TC_ABS_001 | ABS-REQ-002 | ABS Emergency Braking | PASS |
| TC_ABS_002 | ABS-REQ-004 | Front Left Wheel Sensor Dropout | PASS |
| TC_ABS_003 | ABS-REQ-005 | Front Right Wheel Sensor Dropout | PASS |
| TC_ABS_004 | ABS-REQ-003 | ABS Healthy State | PASS |
| TC_ABS_005 | ABS-REQ-006 | ABS Intervention State Observability | PASS |
| TC_ABS_006 | ABS-REQ-008 | ABS CAN Identifier | PASS |
| TC_ABS_007 | ABS-REQ-001 | ABS Wheel Slip Processing | PASS |
| TC_SNS_001 | SNS-REQ-005 | Front Left Sensor Dropout | PASS |
| TC_SNS_002 | SNS-REQ-004 | Front Right Sensor Dropout | PASS |
| TC_SNS_003 | SNS-REQ-006 | Sensor Fault Recovery | PASS |
| TC_SNS_004 | SNS-REQ-001 | Front-Left Wheel-Speed Measurement | PASS |
| TC_SNS_005 | SNS-REQ-002 | Front-Right Wheel-Speed Measurement | PASS |
| TC_SNS_006 | SNS-REQ-003 | Front-Left Dropout Injection | PASS |
| TC_PT_001 | PT-REQ-007 | Powertrain CAN Identifier | PASS |
| TC_PT_002 | MSG-PT-REQ-006 | Powertrain Codec Roundtrip | PASS |
| TC_PT_003 | PT-REQ-001 | Powertrain State | PASS |
| TC_PT_004 | PT-REQ-002 | Powertrain Vehicle Speed | PASS |
| TC_PT_005 | PT-REQ-003 | Powertrain Engine Speed | PASS |
| TC_PT_006 | PT-REQ-004 | Powertrain Gear State | PASS |
| TC_PT_007 | PT-REQ-005 | Powertrain Brake State | PASS |
| TC_STR_001 | STR-REQ-004 | Steering CAN Identifier | PASS |
| TC_STR_002 | MSG-STR-REQ-003 | Steering Codec Roundtrip | PASS |
| TC_STR_003 | STR-REQ-001 | Steering State | PASS |
| TC_STR_004 | STR-REQ-002 | Steering Angle | PASS |
| TC_DIA_001 | DIA-REQ-001 | Front-Left Diagnostic Fault Injection | PASS |
| TC_DIA_002 | DIA-REQ-002 | Front-Right Diagnostic Fault Injection | PASS |
| TC_DIA_003 | DIA-REQ-003 | Front-Left Diagnostic Fault Clearing | PASS |
| TC_DIA_004 | DIA-REQ-004 | Front-Right Diagnostic Fault Clearing | PASS |
| TC_DIA_005 | DIA-REQ-005 | ECU Health Observability | PASS |
| TC_DIA_006 | DIA-REQ-006 | Runtime Fault Injection | PASS |
| TC_DIA_007 | DIA-REQ-007 | Runtime Fault Recovery | PASS |
| TC_DTC_001 | DTC-REQ-001 | Front-Left DTC Definition | PASS |
| TC_DTC_002 | DTC-REQ-002 | Front-Right DTC Definition | PASS |
| TC_DTC_003 | DTC-REQ-003 | Front-Left DTC Activation | PASS |
| TC_DTC_004 | DTC-REQ-004 | Front-Right DTC Activation | PASS |
| TC_DTC_005 | DTC-REQ-005 | Front-Left DTC Recovery | PASS |
| TC_DTC_006 | DTC-REQ-006 | Front-Right DTC Recovery | PASS |
| TC_DTC_007 | DTC-REQ-007 | Individual DTC Clear | PASS |
| TC_DTC_008 | DTC-REQ-008 | Clear All DTCs | PASS |
| TC_DTC_009 | DTC-REQ-009 | First Detection Timestamp | PASS |
| TC_DTC_010 | DTC-REQ-010 | Last Detection Timestamp | PASS |
| TC_DTC_011 | DTC-REQ-011 | DTC Occurrence Count | PASS |
| TC_DTC_012 | DTC-REQ-012 | DTC Lookup | PASS |
| TC_DTC_013 | DTC-REQ-013 | DTC Collection Access | PASS |
| TC_DTC_014 | DTC-REQ-014 | Active DTC Query | PASS |
| TC_DTC_015 | DTC-REQ-015 | Runtime DTC Detection | PASS |
| TC_DTC_016 | DTC-REQ-016 | Runtime DTC Recovery | PASS |
| TC_DTC_017 | DTC-REQ-017 | Diagnostic Reset | PASS |
| TC_DTC_018 | DTC-REQ-018 | Deterministic Diagnostic Behavior | PASS |
| TC_UDS_001 | UDS-REQ-001 | UDS Request Representation | PASS |
| TC_UDS_002 | UDS-REQ-002 | UDS Response Representation | PASS |
| TC_UDS_003 | UDS-REQ-003 | UDS Server Request Processing | PASS |
| TC_UDS_004 | UDS-REQ-004 | Positive Response SID | PASS |
| TC_UDS_005 | UDS-REQ-005 | Negative Response Format | PASS |
| TC_UDS_006 | UDS-REQ-006 | Unsupported Service | PASS |
| TC_UDS_007 | UDS-REQ-007 | Diagnostic Session Control Support | PASS |
| TC_UDS_008 | UDS-REQ-008 | Default Diagnostic Session | PASS |
| TC_UDS_009 | UDS-REQ-009 | Extended Diagnostic Session | PASS |
| TC_UDS_010 | UDS-REQ-010 | Diagnostic Session State Update | PASS |
| TC_UDS_011 | UDS-REQ-011 | Unsupported Diagnostic Session | PASS |
| TC_UDS_012 | UDS-REQ-012 | Read DTC Information Support | PASS |
| TC_UDS_013 | UDS-REQ-013 | Read Active DTCs | PASS |
| TC_UDS_014 | UDS-REQ-014 | Front-Left DTC Reporting | PASS |
| TC_UDS_015 | UDS-REQ-015 | Front-Right DTC Reporting | PASS |
| TC_UDS_016 | UDS-REQ-016 | No Active DTC Response | PASS |
| TC_UDS_017 | UDS-REQ-017 | Clear Diagnostic Information Support | PASS |
| TC_UDS_018 | UDS-REQ-018 | Clear All DTCs Through UDS | PASS |
| TC_UDS_019 | UDS-REQ-019 | Clear Diagnostic Positive Response | PASS |
| TC_UDS_020 | UDS-REQ-020 | Read Data By Identifier Support | PASS |
| TC_UDS_021 | UDS-REQ-021 | Vehicle Speed DID Definition | PASS |
| TC_UDS_022 | UDS-REQ-022 | Vehicle Speed DID Readout | PASS |
| TC_UDS_023 | UDS-REQ-023 | Engine RPM DID Definition | PASS |
| TC_UDS_024 | UDS-REQ-024 | Engine RPM DID Readout | PASS |
| TC_UDS_025 | UDS-REQ-025 | Steering Angle DID Definition | PASS |
| TC_UDS_026 | UDS-REQ-026 | Steering Angle DID Readout | PASS |
| TC_UDS_027 | UDS-REQ-027 | Unsupported DID | PASS |
| TC_UDS_028 | UDS-REQ-028 | DTC Manager Access | PASS |
| TC_UDS_029 | UDS-REQ-029 | DTC Clear Integration | PASS |
| TC_UDS_030 | UDS-REQ-030 | Runtime Diagnostic Processing | PASS |
| TC_UDS_031 | UDS-REQ-031 | Current Vehicle Data | PASS |
| TC_UDS_032 | UDS-REQ-032 | UDS Reset State | PASS |
| TC_UDS_033 | UDS-REQ-033 | Deterministic Diagnostic Response | PASS |
| TC_ISOTP_001 | ISOTP-REQ-001 | ISO-TP Transport Component | PASS |
| TC_ISOTP_002 | ISOTP-REQ-002 | Classic CAN Frame Size | PASS |
| TC_ISOTP_003 | ISOTP-REQ-003 | Standard CAN Identifier | PASS |
| TC_ISOTP_004 | ISOTP-REQ-004 | Maximum ISO-TP Payload | PASS |
| TC_ISOTP_005 | ISOTP-REQ-005 | Empty Payload Rejection | PASS |
| TC_ISOTP_006 | ISOTP-REQ-006 | Single Frame Support | PASS |
| TC_ISOTP_007 | ISOTP-REQ-007 | Single Frame Payload Capacity | PASS |
| TC_ISOTP_008 | ISOTP-REQ-008 | Single Frame PCI | PASS |
| TC_ISOTP_009 | ISOTP-REQ-009 | First Frame Support | PASS |
| TC_ISOTP_010 | ISOTP-REQ-010 | First Frame Payload Length | PASS |
| TC_ISOTP_011 | ISOTP-REQ-011 | First Frame Data Capacity | PASS |
| TC_ISOTP_012 | ISOTP-REQ-012 | Consecutive Frame Support | PASS |
| TC_ISOTP_013 | ISOTP-REQ-013 | Consecutive Frame Data Capacity | PASS |
| TC_ISOTP_014 | ISOTP-REQ-014 | Initial Sequence Number | PASS |
| TC_ISOTP_015 | ISOTP-REQ-015 | Sequence Number Increment | PASS |
| TC_ISOTP_016 | ISOTP-REQ-016 | Sequence Number Rollover | PASS |
| TC_ISOTP_017 | ISOTP-REQ-017 | Flow Control Frame Support | PASS |
| TC_ISOTP_018 | ISOTP-REQ-018 | Flow Status | PASS |
| TC_ISOTP_019 | ISOTP-REQ-019 | Flow Control Parameters | PASS |
| TC_ISOTP_020 | ISOTP-REQ-020 | Single Frame Reassembly | PASS |
| TC_ISOTP_021 | ISOTP-REQ-021 | Multi-Frame Reassembly | PASS |
| TC_ISOTP_022 | ISOTP-REQ-022 | Sequence Validation | PASS |
| TC_ISOTP_023 | ISOTP-REQ-023 | CAN Identifier Validation | PASS |
| TC_ISOTP_024 | ISOTP-REQ-024 | Incomplete Message Detection | PASS |
| TC_ISOTP_025 | ISOTP-REQ-025 | Deterministic Segmentation | PASS |
| TC_ISOTP_026 | ISOTP-REQ-026 | Deterministic Reassembly | PASS |
| TC_UDSTP_001 | UDSTP-REQ-001 | UDS Request Transport | PASS |
| TC_UDSTP_002 | UDSTP-REQ-002 | UDS Response Transport | PASS |
| TC_UDSTP_003 | UDSTP-REQ-003 | Request CAN Identifier | PASS |
| TC_UDSTP_004 | UDSTP-REQ-004 | Response CAN Identifier | PASS |
| TC_UDSTP_005 | UDSTP-REQ-005 | Request Serialization | PASS |
| TC_UDSTP_006 | UDSTP-REQ-006 | Response Serialization | PASS |
| TC_UDSTP_007 | UDSTP-REQ-007 | Positive Response Preservation | PASS |
| TC_UDSTP_008 | UDSTP-REQ-008 | Negative Response Preservation | PASS |
| TC_UDSTP_009 | UDSTP-REQ-009 | Request Identifier Validation | PASS |
| TC_UDSTP_010 | UDSTP-REQ-010 | Response Identifier Validation | PASS |
| TC_UDSTP_011 | UDSTP-REQ-011 | End-to-End UDS Processing | PASS |
| TC_UDSTP_012 | UDSTP-REQ-012 | Deterministic Transport | PASS |
| TC_UDSCAN_001 | UDSCAN-REQ-001 | CAN Transport Component | PASS |
| TC_UDSCAN_002 | UDSCAN-REQ-002 | Request Transmission | PASS |
| TC_UDSCAN_003 | UDSCAN-REQ-003 | Response Transmission | PASS |
| TC_UDSCAN_004 | UDSCAN-REQ-004 | Request Identifier | PASS |
| TC_UDSCAN_005 | UDSCAN-REQ-005 | Response Identifier | PASS |
| TC_UDSCAN_006 | UDSCAN-REQ-006 | Request Reassembly | PASS |
| TC_UDSCAN_007 | UDSCAN-REQ-007 | Response Reassembly | PASS |
| TC_UDSCAN_008 | UDSCAN-REQ-008 | End-to-End Diagnostic Transaction | PASS |
| TC_UDSCAN_009 | UDSCAN-REQ-009 | Diagnostic CAN Trace | PASS |
| TC_UDSCAN_010 | UDSCAN-REQ-010 | Diagnostic CAN Timing | PASS |
| TC_UDSCAN_011 | UDSCAN-REQ-011 | Transaction Evidence | PASS |
| TC_UDSCAN_012 | UDSCAN-REQ-012 | Positive Response Preservation | PASS |
| TC_UDSCAN_013 | UDSCAN-REQ-013 | Negative Response Preservation | PASS |
| TC_UDSCAN_014 | UDSCAN-REQ-014 | Multi-Frame Diagnostic Transport | PASS |
| TC_UDSCAN_015 | UDSCAN-REQ-015 | Deterministic CAN Diagnostic Transaction | PASS |
| TC_CAN_001 | CAN-REQ-003 | CAN Arbitration Priority | PASS |
| TC_CAN_002 | CAN-REQ-001 | CAN Bitrate | PASS |
| TC_CAN_003 | CAN-REQ-008 | CAN Trace Recording | PASS |
| TC_CAN_004 | CAN-REQ-014 | CAN Arbitration Statistics | PASS |
| TC_CAN_005 | ABS-REQ-007 | ABS Message Periodicity | PASS |
| TC_CAN_006 | PT-REQ-006 | Powertrain Message Periodicity | PASS |
| TC_CAN_007 | STR-REQ-003 | Steering Message Periodicity | PASS |
| TC_CAN_008 | CAN-REQ-018 | CAN Bus Utilization | PASS |
| TC_CAN_009 | CAN-REQ-009 | CAN Trace Timing Evidence | PASS |
| TC_CAN_010 | CAN-REQ-006 | CAN Transmission Duration | PASS |
| TC_CAN_011 | CAN-REQ-007 | CAN Waiting Time | PASS |
| TC_CAN_012 | CAN-REQ-013 | CAN Frame Count | PASS |
| TC_CAN_013 | CAN-REQ-015 | Average Waiting Time | PASS |
| TC_CAN_014 | CAN-REQ-016 | Maximum Waiting Time | PASS |
| TC_CAN_015 | CAN-REQ-017 | Total Transmission Time | PASS |
| TC_CAN_016 | CAN-REQ-019 | Per-Message Statistics | PASS |
| TC_CAN_017 | CAN-REQ-005 | CAN Pending Frame Behavior | PASS |
| TC_CAN_018 | CAN-REQ-012 | CAN Waiting-Time Trace Evidence | PASS |
| TC_CAN_019 | CAN-REQ-004 | CAN Exclusive Bus Access | PASS |
| TC_CAN_020 | CAN-REQ-010 | CAN Trace Identifier Evidence | PASS |
| TC_CAN_021 | CAN-REQ-011 | CAN Trace Payload Evidence | PASS |
| TC_CAN_022 | CAN-REQ-002 | Multiple ECU Transmission Requests | PASS |
| TC_MSG_001 | MSG-ABS-REQ-003 | ABS Codec Roundtrip | PASS |
| TC_MSG_002 | MSG-ABS-REQ-001 | ABS Message CAN Identifier | PASS |
| TC_MSG_003 | MSG-ABS-REQ-002 | ABS Message State Information | PASS |
| TC_MSG_004 | MSG-PT-REQ-001 | Powertrain Message CAN Identifier | PASS |
| TC_MSG_005 | MSG-PT-REQ-002 | Powertrain Vehicle-Speed Information | PASS |
| TC_MSG_006 | MSG-PT-REQ-003 | Powertrain Engine-RPM Information | PASS |
| TC_MSG_007 | MSG-PT-REQ-004 | Powertrain Gear-State Information | PASS |
| TC_MSG_008 | MSG-PT-REQ-005 | Powertrain Brake-State Information | PASS |
| TC_MSG_009 | MSG-STR-REQ-001 | Steering Message CAN Identifier | PASS |
| TC_MSG_010 | MSG-STR-REQ-002 | Steering-Angle Information | PASS |
| TC_VAL_001 | VAL-REQ-001 | Individual Test Execution | PASS |
| TC_VAL_002 | VAL-REQ-003 | Validation Test Isolation | PASS |
| TC_VAL_003 | VAL-REQ-008 | Test Status | PASS |
| TC_VAL_004 | VAL-REQ-009 | Execution-Time Evidence | PASS |
| TC_VAL_005 | VAL-REQ-010 | Response-Time Evidence | PASS |
| TC_VAL_006 | VAL-REQ-012 | Timing Pass Criteria | PASS |

---

## 3. Requirement Traceability

### 3.1 Existing Baseline Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| SYS-REQ-001 | SimulationEngine / ScenarioController / GUI | TC_SYS_001 | — | VERIFIED / PASS |
| SYS-REQ-002 | SimulationEngine / ScenarioController / GUI | TC_VEH_001, TC_VEH_002, TC_VEH_003, TC_VEH_004 | — | VERIFIED / PASS |
| SYS-REQ-003 | SimulationEngine / ScenarioController / GUI | TC_SYS_003 | ECU/CAN test group | VERIFIED / PASS |
| SYS-REQ-004 | SimulationEngine / ScenarioController / GUI | Inspection IVR-001 | GUI observability inspection | VERIFIED / PASS |
| SYS-REQ-005 | SimulationEngine / ScenarioController / GUI | TC_SYS_002 | — | VERIFIED / PASS |
| SYS-REQ-006 | SimulationEngine / ScenarioController / GUI | Inspection IVR-002 | — | VERIFIED / PASS |
| SYS-REQ-007 | SimulationEngine / ScenarioController / GUI | TC_SYS_004 | Isolated SimulationEngine instances in tests | VERIFIED / PASS |
| VEH-REQ-001 | VehicleModel / ScenarioController | TC_VEH_005 | TC_VEH_001, TC_VEH_003 | VERIFIED / PASS |
| VEH-REQ-002 | VehicleModel / ScenarioController | TC_VEH_006 | ABS/sensor tests | VERIFIED / PASS |
| VEH-REQ-003 | VehicleModel / ScenarioController | TC_VEH_007 | ABS/sensor tests | VERIFIED / PASS |
| VEH-REQ-004 | VehicleModel / ScenarioController | TC_VEH_008 | TC_VEH_003 | VERIFIED / PASS |
| VEH-REQ-005 | VehicleModel / ScenarioController | TC_VEH_009 | Steering tests | VERIFIED / PASS |
| VEH-REQ-006 | VehicleModel / ScenarioController | TC_VEH_010 | Scenario tests | VERIFIED / PASS |
| VEH-REQ-007 | VehicleModel / ScenarioController | TC_VEH_001 | — | VERIFIED / PASS |
| VEH-REQ-008 | VehicleModel / ScenarioController | TC_VEH_003 | — | VERIFIED / PASS |
| VEH-REQ-009 | VehicleModel / ScenarioController | TC_VEH_011 | ABS tests | VERIFIED / PASS |
| ABS-REQ-001 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_007 | TC_ABS_001 | VERIFIED / PASS |
| ABS-REQ-002 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_001 | — | VERIFIED / PASS |
| ABS-REQ-003 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_004 | — | VERIFIED / PASS |
| ABS-REQ-004 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_002 | — | VERIFIED / PASS |
| ABS-REQ-005 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_003 | — | VERIFIED / PASS |
| ABS-REQ-006 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_005 | TC_ABS_001 | VERIFIED / PASS |
| ABS-REQ-007 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_CAN_005 | — | VERIFIED / PASS |
| ABS-REQ-008 | AbsEcu / WheelSpeedSensor / SimulationEngine | TC_ABS_006 | TC_MSG_001 | VERIFIED / PASS |
| SNS-REQ-001 | WheelSpeedSensor / SimulationEngine | TC_SNS_004 | — | VERIFIED / PASS |
| SNS-REQ-002 | WheelSpeedSensor / SimulationEngine | TC_SNS_005 | — | VERIFIED / PASS |
| SNS-REQ-003 | WheelSpeedSensor / SimulationEngine | TC_SNS_006 | TC_SNS_001, TC_ABS_002 | VERIFIED / PASS |
| SNS-REQ-004 | WheelSpeedSensor / SimulationEngine | TC_SNS_002 | — | VERIFIED / PASS |
| SNS-REQ-005 | WheelSpeedSensor / SimulationEngine | TC_SNS_001 | — | VERIFIED / PASS |
| SNS-REQ-006 | WheelSpeedSensor / SimulationEngine | TC_SNS_003 | — | VERIFIED / PASS |
| PT-REQ-001 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_PT_003 | TC_PT_002 | VERIFIED / PASS |
| PT-REQ-002 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_PT_004 | TC_PT_002 | VERIFIED / PASS |
| PT-REQ-003 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_PT_005 | TC_PT_002 | VERIFIED / PASS |
| PT-REQ-004 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_PT_006 | TC_PT_002 | VERIFIED / PASS |
| PT-REQ-005 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_PT_007 | TC_PT_002 | VERIFIED / PASS |
| PT-REQ-006 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_CAN_006 | — | VERIFIED / PASS |
| PT-REQ-007 | PowertrainEcu / VehicleStateCodec / SimulationEngine | TC_PT_001 | — | VERIFIED / PASS |
| STR-REQ-001 | SteeringEcu / SteeringStateCodec / SimulationEngine | TC_STR_003 | TC_STR_002 | VERIFIED / PASS |
| STR-REQ-002 | SteeringEcu / SteeringStateCodec / SimulationEngine | TC_STR_004 | TC_STR_002 | VERIFIED / PASS |
| STR-REQ-003 | SteeringEcu / SteeringStateCodec / SimulationEngine | TC_CAN_007 | — | VERIFIED / PASS |
| STR-REQ-004 | SteeringEcu / SteeringStateCodec / SimulationEngine | TC_STR_001 | — | VERIFIED / PASS |
| CAN-REQ-001 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_002 | — | VERIFIED / PASS |
| CAN-REQ-002 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_022 | TC_CAN_001 | VERIFIED / PASS |
| CAN-REQ-003 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_001 | — | VERIFIED / PASS |
| CAN-REQ-004 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_019 | TC_CAN_001 | VERIFIED / PASS |
| CAN-REQ-005 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_017 | TC_CAN_001 | VERIFIED / PASS |
| CAN-REQ-006 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_010 | TC_CAN_009 | VERIFIED / PASS |
| CAN-REQ-007 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_011 | TC_CAN_009 | VERIFIED / PASS |
| CAN-REQ-008 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_003 | — | VERIFIED / PASS |
| CAN-REQ-009 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_009 | — | VERIFIED / PASS |
| CAN-REQ-010 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_020 | TC_CAN_003 | VERIFIED / PASS |
| CAN-REQ-011 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_021 | TC_CAN_003 | VERIFIED / PASS |
| CAN-REQ-012 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_018 | TC_CAN_009 | VERIFIED / PASS |
| CAN-REQ-013 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_012 | — | VERIFIED / PASS |
| CAN-REQ-014 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_004 | — | VERIFIED / PASS |
| CAN-REQ-015 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_013 | — | VERIFIED / PASS |
| CAN-REQ-016 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_014 | — | VERIFIED / PASS |
| CAN-REQ-017 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_015 | TC_CAN_008 | VERIFIED / PASS |
| CAN-REQ-018 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_008 | — | VERIFIED / PASS |
| CAN-REQ-019 | VirtualCanBus / CanArbitration / CanTiming / CanStatistics / CanTraceEntry | TC_CAN_016 | — | VERIFIED / PASS |
| MSG-ABS-REQ-001 | AbsStateCodec | TC_MSG_002 | TC_MSG_001 | VERIFIED / PASS |
| MSG-ABS-REQ-002 | AbsStateCodec | TC_MSG_003 | TC_MSG_001 | VERIFIED / PASS |
| MSG-ABS-REQ-003 | AbsStateCodec | TC_MSG_001 | — | VERIFIED / PASS |
| MSG-PT-REQ-001 | VehicleStateCodec | TC_MSG_004 | TC_PT_001 | VERIFIED / PASS |
| MSG-PT-REQ-002 | VehicleStateCodec | TC_MSG_005 | TC_PT_002 | VERIFIED / PASS |
| MSG-PT-REQ-003 | VehicleStateCodec | TC_MSG_006 | TC_PT_002 | VERIFIED / PASS |
| MSG-PT-REQ-004 | VehicleStateCodec | TC_MSG_007 | TC_PT_002 | VERIFIED / PASS |
| MSG-PT-REQ-005 | VehicleStateCodec | TC_MSG_008 | TC_PT_002 | VERIFIED / PASS |
| MSG-PT-REQ-006 | VehicleStateCodec | TC_PT_002 | — | VERIFIED / PASS |
| MSG-STR-REQ-001 | SteeringStateCodec | TC_MSG_009 | TC_STR_001 | VERIFIED / PASS |
| MSG-STR-REQ-002 | SteeringStateCodec | TC_MSG_010 | TC_STR_002 | VERIFIED / PASS |
| MSG-STR-REQ-003 | SteeringStateCodec | TC_STR_002 | — | VERIFIED / PASS |
| DIA-REQ-001 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_001 | TC_ABS_002, TC_SNS_001 | VERIFIED / PASS |
| DIA-REQ-002 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_002 | TC_ABS_003, TC_SNS_002 | VERIFIED / PASS |
| DIA-REQ-003 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_003 | TC_SNS_003 | VERIFIED / PASS |
| DIA-REQ-004 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_004 | TC_SNS_003 | VERIFIED / PASS |
| DIA-REQ-005 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_005 | TC_ABS_002, TC_ABS_003 | VERIFIED / PASS |
| DIA-REQ-006 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_006 | — | VERIFIED / PASS |
| DIA-REQ-007 | SimulationEngine / WheelSpeedSensor / AbsEcu | TC_DIA_007 | TC_SNS_003 | VERIFIED / PASS |

### 3.2 DTC Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| DTC-REQ-001 | DtcDefinitions / DtcManager | TC_DTC_001 | — | VERIFIED / PASS |
| DTC-REQ-002 | DtcDefinitions / DtcManager | TC_DTC_002 | — | VERIFIED / PASS |
| DTC-REQ-003 | WheelSpeedSensor / SimulationEngine / DtcManager | TC_DTC_003 | TC_DIA_001 | VERIFIED / PASS |
| DTC-REQ-004 | WheelSpeedSensor / SimulationEngine / DtcManager | TC_DTC_004 | TC_DIA_002 | VERIFIED / PASS |
| DTC-REQ-005 | WheelSpeedSensor / SimulationEngine / DtcManager | TC_DTC_005 | TC_DIA_003 | VERIFIED / PASS |
| DTC-REQ-006 | WheelSpeedSensor / SimulationEngine / DtcManager | TC_DTC_006 | TC_DIA_004 | VERIFIED / PASS |
| DTC-REQ-007 | DtcManager | TC_DTC_007 | — | VERIFIED / PASS |
| DTC-REQ-008 | DtcManager | TC_DTC_008 | — | VERIFIED / PASS |
| DTC-REQ-009 | Dtc / DtcManager | TC_DTC_009 | — | VERIFIED / PASS |
| DTC-REQ-010 | Dtc / DtcManager | TC_DTC_010 | — | VERIFIED / PASS |
| DTC-REQ-011 | Dtc / DtcManager | TC_DTC_011 | — | VERIFIED / PASS |
| DTC-REQ-012 | DtcManager | TC_DTC_012 | — | VERIFIED / PASS |
| DTC-REQ-013 | DtcManager | TC_DTC_013 | — | VERIFIED / PASS |
| DTC-REQ-014 | DtcManager | TC_DTC_014 | — | VERIFIED / PASS |
| DTC-REQ-015 | WheelSpeedSensor / SimulationEngine / DtcManager | TC_DTC_015 | TC_DIA_006 | VERIFIED / PASS |
| DTC-REQ-016 | WheelSpeedSensor / SimulationEngine / DtcManager | TC_DTC_016 | TC_DIA_007 | VERIFIED / PASS |
| DTC-REQ-017 | SimulationEngine / DtcManager | TC_DTC_017 | TC_SYS_002 | VERIFIED / PASS |
| DTC-REQ-018 | SimulationEngine / DtcManager | TC_DTC_018 | TC_SYS_001 | VERIFIED / PASS |

### 3.3 UDS Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| UDS-REQ-001 | UdsRequest | TC_UDS_001 | — | VERIFIED / PASS |
| UDS-REQ-002 | UdsResponse | TC_UDS_002 | — | VERIFIED / PASS |
| UDS-REQ-003 | UdsServer | TC_UDS_003 | — | VERIFIED / PASS |
| UDS-REQ-004 | UdsServer / UdsService | TC_UDS_004 | — | VERIFIED / PASS |
| UDS-REQ-005 | UdsServer / UdsResponse / UdsService | TC_UDS_005 | — | VERIFIED / PASS |
| UDS-REQ-006 | UdsServer / UdsService | TC_UDS_006 | — | VERIFIED / PASS |
| UDS-REQ-007 | UdsServer / UdsService | TC_UDS_007 | — | VERIFIED / PASS |
| UDS-REQ-008 | UdsServer / UdsService | TC_UDS_008 | — | VERIFIED / PASS |
| UDS-REQ-009 | UdsServer / UdsService | TC_UDS_009 | — | VERIFIED / PASS |
| UDS-REQ-010 | UdsServer | TC_UDS_010 | TC_UDS_009 | VERIFIED / PASS |
| UDS-REQ-011 | UdsServer / UdsService | TC_UDS_011 | — | VERIFIED / PASS |
| UDS-REQ-012 | UdsServer / UdsService / DtcManager | TC_UDS_012 | — | VERIFIED / PASS |
| UDS-REQ-013 | UdsServer / DtcManager | TC_UDS_013 | — | VERIFIED / PASS |
| UDS-REQ-014 | UdsServer / DtcManager / DtcDefinitions | TC_UDS_014 | TC_DTC_003 | VERIFIED / PASS |
| UDS-REQ-015 | UdsServer / DtcManager / DtcDefinitions | TC_UDS_015 | TC_DTC_004 | VERIFIED / PASS |
| UDS-REQ-016 | UdsServer / DtcManager | TC_UDS_016 | — | VERIFIED / PASS |
| UDS-REQ-017 | UdsServer / UdsService | TC_UDS_017 | — | VERIFIED / PASS |
| UDS-REQ-018 | UdsServer / DtcManager | TC_UDS_018 | TC_DTC_008 | VERIFIED / PASS |
| UDS-REQ-019 | UdsServer | TC_UDS_019 | TC_UDS_017 | VERIFIED / PASS |
| UDS-REQ-020 | UdsServer / UdsService | TC_UDS_020 | — | VERIFIED / PASS |
| UDS-REQ-021 | UdsService | TC_UDS_021 | — | VERIFIED / PASS |
| UDS-REQ-022 | UdsServer / SimulationEngine / VehicleModel | TC_UDS_022 | TC_UDS_031 | VERIFIED / PASS |
| UDS-REQ-023 | UdsService | TC_UDS_023 | — | VERIFIED / PASS |
| UDS-REQ-024 | UdsServer / SimulationEngine / VehicleState | TC_UDS_024 | TC_UDS_031 | VERIFIED / PASS |
| UDS-REQ-025 | UdsService | TC_UDS_025 | — | VERIFIED / PASS |
| UDS-REQ-026 | UdsServer / SimulationEngine / VehicleModel | TC_UDS_026 | TC_UDS_031 | VERIFIED / PASS |
| UDS-REQ-027 | UdsServer / UdsService | TC_UDS_027 | — | VERIFIED / PASS |
| UDS-REQ-028 | UdsServer / DtcManager | TC_UDS_028 | TC_UDS_013 | VERIFIED / PASS |
| UDS-REQ-029 | UdsServer / DtcManager | TC_UDS_029 | TC_UDS_018 | VERIFIED / PASS |
| UDS-REQ-030 | SimulationEngine / UdsServer | TC_UDS_030 | — | VERIFIED / PASS |
| UDS-REQ-031 | SimulationEngine / UdsServer / VehicleModel | TC_UDS_031 | TC_UDS_022, TC_UDS_024, TC_UDS_026 | VERIFIED / PASS |
| UDS-REQ-032 | SimulationEngine / UdsServer | TC_UDS_032 | TC_SYS_002 | VERIFIED / PASS |
| UDS-REQ-033 | UdsServer / DtcManager / SimulationEngine | TC_UDS_033 | — | VERIFIED / PASS |

### 3.4 ISO-TP Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| ISOTP-REQ-001 | IsoTpProtocol / IsoTpFrame | TC_ISOTP_001 | — | VERIFIED / PASS |
| ISOTP-REQ-002 | IsoTpProtocol / CanFrame | TC_ISOTP_002 | — | VERIFIED / PASS |
| ISOTP-REQ-003 | IsoTpProtocol / CanFrame | TC_ISOTP_003 | — | VERIFIED / PASS |
| ISOTP-REQ-004 | IsoTpProtocol | TC_ISOTP_004 | — | VERIFIED / PASS |
| ISOTP-REQ-005 | IsoTpProtocol | TC_ISOTP_005 | — | VERIFIED / PASS |
| ISOTP-REQ-006 | IsoTpProtocol | TC_ISOTP_006 | — | VERIFIED / PASS |
| ISOTP-REQ-007 | IsoTpProtocol | TC_ISOTP_007 | — | VERIFIED / PASS |
| ISOTP-REQ-008 | IsoTpProtocol / IsoTpFrame | TC_ISOTP_008 | — | VERIFIED / PASS |
| ISOTP-REQ-009 | IsoTpProtocol | TC_ISOTP_009 | — | VERIFIED / PASS |
| ISOTP-REQ-010 | IsoTpProtocol / IsoTpFrame | TC_ISOTP_010 | — | VERIFIED / PASS |
| ISOTP-REQ-011 | IsoTpProtocol | TC_ISOTP_011 | — | VERIFIED / PASS |
| ISOTP-REQ-012 | IsoTpProtocol | TC_ISOTP_012 | — | VERIFIED / PASS |
| ISOTP-REQ-013 | IsoTpProtocol | TC_ISOTP_013 | — | VERIFIED / PASS |
| ISOTP-REQ-014 | IsoTpProtocol | TC_ISOTP_014 | — | VERIFIED / PASS |
| ISOTP-REQ-015 | IsoTpProtocol | TC_ISOTP_015 | — | VERIFIED / PASS |
| ISOTP-REQ-016 | IsoTpProtocol | TC_ISOTP_016 | — | VERIFIED / PASS |
| ISOTP-REQ-017 | IsoTpProtocol / IsoTpFrame | TC_ISOTP_017 | — | VERIFIED / PASS |
| ISOTP-REQ-018 | IsoTpProtocol / IsoTpFrame | TC_ISOTP_018 | — | VERIFIED / PASS |
| ISOTP-REQ-019 | IsoTpProtocol / IsoTpFrame | TC_ISOTP_019 | — | VERIFIED / PASS |
| ISOTP-REQ-020 | IsoTpProtocol | TC_ISOTP_020 | — | VERIFIED / PASS |
| ISOTP-REQ-021 | IsoTpProtocol | TC_ISOTP_021 | — | VERIFIED / PASS |
| ISOTP-REQ-022 | IsoTpProtocol | TC_ISOTP_022 | — | VERIFIED / PASS |
| ISOTP-REQ-023 | IsoTpProtocol / CanFrame | TC_ISOTP_023 | — | VERIFIED / PASS |
| ISOTP-REQ-024 | IsoTpProtocol | TC_ISOTP_024 | — | VERIFIED / PASS |
| ISOTP-REQ-025 | IsoTpProtocol | TC_ISOTP_025 | — | VERIFIED / PASS |
| ISOTP-REQ-026 | IsoTpProtocol | TC_ISOTP_026 | — | VERIFIED / PASS |

### 3.5 UDS Transport Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| UDSTP-REQ-001 | UdsTransport / IsoTpProtocol | TC_UDSTP_001 | TC_ISOTP_001 | VERIFIED / PASS |
| UDSTP-REQ-002 | UdsTransport / IsoTpProtocol | TC_UDSTP_002 | TC_ISOTP_001 | VERIFIED / PASS |
| UDSTP-REQ-003 | UdsTransport | TC_UDSTP_003 | — | VERIFIED / PASS |
| UDSTP-REQ-004 | UdsTransport | TC_UDSTP_004 | — | VERIFIED / PASS |
| UDSTP-REQ-005 | UdsTransport | TC_UDSTP_005 | — | VERIFIED / PASS |
| UDSTP-REQ-006 | UdsTransport | TC_UDSTP_006 | — | VERIFIED / PASS |
| UDSTP-REQ-007 | UdsTransport / UdsResponse | TC_UDSTP_007 | TC_UDS_004 | VERIFIED / PASS |
| UDSTP-REQ-008 | UdsTransport / UdsResponse | TC_UDSTP_008 | TC_UDS_005 | VERIFIED / PASS |
| UDSTP-REQ-009 | UdsTransport | TC_UDSTP_009 | TC_ISOTP_023 | VERIFIED / PASS |
| UDSTP-REQ-010 | UdsTransport | TC_UDSTP_010 | TC_ISOTP_023 | VERIFIED / PASS |
| UDSTP-REQ-011 | UdsTransport / UdsServer / IsoTpProtocol | TC_UDSTP_011 | UDS and ISO-TP test groups | VERIFIED / PASS |
| UDSTP-REQ-012 | UdsTransport / UdsServer | TC_UDSTP_012 | TC_UDS_033 | VERIFIED / PASS |

### 3.6 UDS CAN Transport Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| UDSCAN-REQ-001 | UdsCanTransport / UdsTransport / VirtualCanBus | TC_UDSCAN_001 | UDSTP test group | VERIFIED / PASS |
| UDSCAN-REQ-002 | UdsCanTransport / VirtualCanBus | TC_UDSCAN_002 | TC_UDSTP_001 | VERIFIED / PASS |
| UDSCAN-REQ-003 | UdsCanTransport / VirtualCanBus | TC_UDSCAN_003 | TC_UDSTP_002 | VERIFIED / PASS |
| UDSCAN-REQ-004 | UdsCanTransport / UdsTransport | TC_UDSCAN_004 | TC_UDSTP_003 | VERIFIED / PASS |
| UDSCAN-REQ-005 | UdsCanTransport / UdsTransport | TC_UDSCAN_005 | TC_UDSTP_004 | VERIFIED / PASS |
| UDSCAN-REQ-006 | UdsCanTransport / UdsTransport / IsoTpProtocol | TC_UDSCAN_006 | TC_ISOTP_021 | VERIFIED / PASS |
| UDSCAN-REQ-007 | UdsCanTransport / UdsTransport / IsoTpProtocol | TC_UDSCAN_007 | TC_ISOTP_021 | VERIFIED / PASS |
| UDSCAN-REQ-008 | UdsCanTransport / UdsTransport / VirtualCanBus / UdsServer | TC_UDSCAN_008 | TC_UDSTP_011 | VERIFIED / PASS |
| UDSCAN-REQ-009 | UdsCanTransport / VirtualCanBus / CanTraceEntry | TC_UDSCAN_009 | TC_CAN_003 | VERIFIED / PASS |
| UDSCAN-REQ-010 | UdsCanTransport / VirtualCanBus | TC_UDSCAN_010 | TC_CAN_010 | VERIFIED / PASS |
| UDSCAN-REQ-011 | UdsCanTransport / UdsCanTransactionResult | TC_UDSCAN_011 | — | VERIFIED / PASS |
| UDSCAN-REQ-012 | UdsCanTransport / UdsResponse | TC_UDSCAN_012 | TC_UDSTP_007 | VERIFIED / PASS |
| UDSCAN-REQ-013 | UdsCanTransport / UdsResponse | TC_UDSCAN_013 | TC_UDSTP_008 | VERIFIED / PASS |
| UDSCAN-REQ-014 | UdsCanTransport / UdsTransport / IsoTpProtocol / VirtualCanBus | TC_UDSCAN_014 | TC_ISOTP_021 | VERIFIED / PASS |
| UDSCAN-REQ-015 | UdsCanTransport / VirtualCanBus / UdsServer | TC_UDSCAN_015 | TC_UDSTP_012 | VERIFIED / PASS |

### 3.7 Validation Requirements

| Requirement | Implementation Area | Direct Verification | Supporting Evidence | Status |
|---|---|---|---|---|
| VAL-REQ-001 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_001 | Operational individual-test execution | VERIFIED / PASS |
| VAL-REQ-002 | TestRunner / TestCase / TestResult / TestRunnerWidget | Suite execution record: 188 PASS / 0 FAIL | 188-test suite execution | VERIFIED / PASS |
| VAL-REQ-003 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_002 | Current isolated test architecture | VERIFIED / PASS |
| VAL-REQ-004 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-003 | 188 unique IDs observed | VERIFIED / PASS |
| VAL-REQ-005 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-004 | requirementId populated | VERIFIED / PASS |
| VAL-REQ-006 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-005 | Expected evidence populated | VERIFIED / PASS |
| VAL-REQ-007 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-006 | Actual evidence populated | VERIFIED / PASS |
| VAL-REQ-008 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_003 | 188 PASS results observed | VERIFIED / PASS |
| VAL-REQ-009 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_004 | Execution-time evidence displayed | VERIFIED / PASS |
| VAL-REQ-010 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_005 | TC_ABS_002, TC_ABS_003 | VERIFIED / PASS |
| VAL-REQ-011 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-007 | Timing limit displayed | VERIFIED / PASS |
| VAL-REQ-012 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_006 | Timing pass criteria implemented | VERIFIED / PASS |
| VAL-REQ-013 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-008 | Summary displayed | VERIFIED / PASS |
| VAL-REQ-014 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-009 | Detail evidence displayed | VERIFIED / PASS |
| VAL-REQ-015 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-010 | Requirement ID displayed | VERIFIED / PASS |

---

## 4. Verification Summary

**Automated tests:** 188  
**Passed:** 188  
**Failed:** 0  
**Baseline requirements:** 198  
**Verified requirements:** 198  
**Unverified baseline requirements:** 0

Automated tests provide direct requirements-based evidence for the functional,
CAN, diagnostics, DTC, UDS, ISO-TP, UDS Transport, UDS CAN Transport, message,
sensor, ECU, and validation-framework behavior.

Requirements whose defined verification method is Inspection are closed through
the inspection records in `InspectionVerification.md`.

SYS-REQ-002 uses combined scenario evidence across TC_VEH_001 through
TC_VEH_004.

VAL-REQ-002 is closed by successful execution of the complete registered
188-test suite.

DTC-REQ-001 through DTC-REQ-018 are closed through TC_DTC_001 through
TC_DTC_018.

UDS-REQ-001 through UDS-REQ-033 are closed through TC_UDS_001 through
TC_UDS_033.

ISOTP-REQ-001 through ISOTP-REQ-026 are closed through TC_ISOTP_001 through
TC_ISOTP_026.

UDSTP-REQ-001 through UDSTP-REQ-012 are closed through TC_UDSTP_001 through
TC_UDSTP_012.

UDSCAN-REQ-001 through UDSCAN-REQ-015 are closed through TC_UDSCAN_001 through
TC_UDSCAN_015.

---

## 5. Baseline Verification Closure

Baseline v1.5 records full verification closure for the requirements currently
defined by the VirtualVehicle project.

Baseline v1.5 extends Baseline v1.4 with:

- 26 ISO-TP requirements and 26 corresponding automated verification tests
- 12 UDS Transport requirements and 12 corresponding automated verification tests
- 15 UDS CAN Transport requirements and 15 corresponding automated verification tests

### Closure Record

- Previous baseline requirements: 145
- New ISO-TP requirements: 26
- New UDS Transport requirements: 12
- New UDS CAN Transport requirements: 15
- Total baseline requirements: 198
- Previous automated tests: 135
- New ISO-TP automated tests: 26
- New UDS Transport automated tests: 12
- New UDS CAN Transport automated tests: 15
- Total automated tests: 188
- Passed: 188
- Failed: 0
- Verified requirements: 198
- Unverified requirements: 0

---

## 6. Baseline Boundary

The following features remain explicitly outside Baseline v1.5 and are not
claimed as verified requirements:

- Full ISO 14229 compliance
- Full ISO 15765-2 compliance
- Runtime UDS CAN integration into the SimulationEngine shared ECU bus
- Concurrent periodic ECU and diagnostic traffic verification
- Parallel diagnostic clients
- ISO-TP Flow Control runtime scheduling
- ISO-TP Block Size enforcement
- ISO-TP STmin timing enforcement
- CAN FD ISO-TP
- Extended ISO-TP addressing
- Functional diagnostic addressing
- Security Access
- ECU Reset service
- Communication Control
- Routine Control
- Request Download
- Transfer Data
- ECU reprogramming / flashing
- AUTOSAR runtime behavior
- Automotive Ethernet
- Physical diagnostic hardware
- Physical CAN transceiver or hardware-bus validation
- Hardware-in-the-loop CAN validation

Future requirements for these capabilities shall be introduced in a new
baseline before implementation is claimed as requirements-verified.

---

## 7. Baseline Status

```text
VirtualVehicle Baseline v1.5

Requirements:      198
Verified:          198
Unverified:          0

Automated Tests:   188
Passed:            188
Failed:              0

Status:
VERIFICATION CLOSED
```