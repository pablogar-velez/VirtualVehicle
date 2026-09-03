# VirtualVehicle Requirements Traceability Matrix

**Document ID:** VV-RTM-001  
**Baseline:** 1.2  
**Status:** Current Verification Record  
**Latest Automated Suite Result:** 84 Passed / 0 Failed

---

## 1. Purpose

This document provides project-level traceability between VirtualVehicle
requirements, implementation areas, automated verification test cases, and
verification results.

A requirement is marked **VERIFIED / PASS** only when the available evidence is
sufficient for direct verification. Incidental or indirect exercise is recorded
as supporting evidence and does not by itself change formal verification status.

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
| VAL-REQ-001 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_001 | Operational individual-test execution | VERIFIED / PASS |
| VAL-REQ-002 | TestRunner / TestCase / TestResult / TestRunnerWidget | Suite execution record: 84 PASS / 0 FAIL | 84-test suite execution | VERIFIED / PASS |
| VAL-REQ-003 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_002 | Current isolated test architecture | VERIFIED / PASS |
| VAL-REQ-004 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-003 | 46 unique IDs observed | VERIFIED / PASS |
| VAL-REQ-005 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-004 | requirementId populated | VERIFIED / PASS |
| VAL-REQ-006 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-005 | Expected evidence populated | VERIFIED / PASS |
| VAL-REQ-007 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-006 | Actual evidence populated | VERIFIED / PASS |
| VAL-REQ-008 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_003 | 46 PASS results observed | VERIFIED / PASS |
| VAL-REQ-009 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_004 | Execution-time evidence displayed | VERIFIED / PASS |
| VAL-REQ-010 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_005 | TC_ABS_002, TC_ABS_003 | VERIFIED / PASS |
| VAL-REQ-011 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-007 | Timing limit displayed | VERIFIED / PASS |
| VAL-REQ-012 | TestRunner / TestCase / TestResult / TestRunnerWidget | TC_VAL_006 | Timing pass criteria implemented | VERIFIED / PASS |
| VAL-REQ-013 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-008 | Summary displayed | VERIFIED / PASS |
| VAL-REQ-014 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-009 | Detail evidence displayed | VERIFIED / PASS |
| VAL-REQ-015 | TestRunner / TestCase / TestResult / TestRunnerWidget | Inspection IVR-010 | Requirement ID displayed | VERIFIED / PASS |

---

## 4. Verification Summary

**Automated tests:** 84  
**Passed:** 84  
**Failed:** 0  
**Baseline requirements:** 94  
**Verified requirements:** 94  
**Unverified baseline requirements:** 0

Automated tests provide direct requirements-based evidence for the functional,
CAN, diagnostics, message, sensor, ECU, and validation-framework behavior.
Requirements whose defined verification method is Inspection are closed through
the inspection records in `InspectionVerification.md`.

SYS-REQ-002 uses combined scenario evidence across TC_VEH_001 through
TC_VEH_004. VAL-REQ-002 is closed by successful execution of the complete
registered 84-test suite.

---

## 5. Baseline Verification Closure

Baseline v1.2 records full verification closure for the requirements currently
defined by the VirtualVehicle project.

The following features remain explicitly outside this baseline and are not
claimed as verified requirements:

- UDS diagnostic services
- Diagnostic Trouble Codes (DTCs)
- Diagnostic sessions and security access
- ECU reprogramming / flashing
- AUTOSAR runtime behavior
- Physical CAN transceiver or hardware-bus validation

Future requirements for those capabilities shall be introduced in a new
baseline before implementation is claimed as requirements-verified.
