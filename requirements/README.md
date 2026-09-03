# VirtualVehicle Requirements Specification

**Document ID:** VV-SRS-001  
**Version:** 1.2  
**Status:** Baseline  
**System:** VirtualVehicle Platform  

---

## 1. Verification Workflow

Requirement → Implementation → Test Case → Execution → Evidence → PASS / FAIL

Implementation alone does not constitute verification.

Verification states:

- NOT IMPLEMENTED
- IMPLEMENTED / UNVERIFIED
- VERIFIED / PASS
- VERIFICATION FAILED

---

## 2. Requirement Categories

| Prefix | Domain | Specification |
|---|---|---|
| SYS | System | SystemRequirements.md |
| VEH | Vehicle Model | VehicleRequirements.md |
| ABS | ABS ECU | AbsRequirements.md |
| SNS | Sensors | SensorRequirements.md |
| PT | Powertrain ECU | PowertrainRequirements.md |
| STR | Steering ECU | SteeringRequirements.md |
| CAN | CAN Bus | CanRequirements.md |
| MSG | CAN Messages | MessageRequirements.md |
| DIA | Diagnostics / Fault Injection | DiagnosticsRequirements.md |
| VAL | Automated Validation | ValidationRequirements.md |

---

## 3. Current Automated Test Coverage

**Suite result: 84 passed / 0 Failed**

| Test Case | Primary Requirement | Description |
|---|---|---|
| TC_SYS_001 | SYS-REQ-001 | Simulation Time Progression |
| TC_SYS_002 | SYS-REQ-005 | Simulation Reset |
| TC_VEH_001 | VEH-REQ-007 | Acceleration Scenario |
| TC_VEH_002 | SYS-REQ-002 | Cruise Scenario |
| TC_VEH_003 | VEH-REQ-008 | Hard Braking Scenario |
| TC_VEH_004 | SYS-REQ-002 | Recovery Scenario |
| TC_ABS_001 | ABS-REQ-002 | ABS Emergency Braking |
| TC_ABS_002 | ABS-REQ-004 | Front Left Wheel Sensor Dropout |
| TC_ABS_003 | ABS-REQ-005 | Front Right Wheel Sensor Dropout |
| TC_ABS_004 | ABS-REQ-003 | ABS Healthy State |
| TC_SNS_001 | SNS-REQ-005 | Front Left Sensor Dropout |
| TC_SNS_002 | SNS-REQ-004 | Front Right Sensor Dropout |
| TC_SNS_003 | SNS-REQ-006 | Sensor Fault Recovery |
| TC_PT_001 | PT-REQ-007 | Powertrain CAN Identifier |
| TC_PT_002 | MSG-PT-REQ-006 | Powertrain Codec Roundtrip |
| TC_STR_001 | STR-REQ-004 | Steering CAN Identifier |
| TC_STR_002 | MSG-STR-REQ-003 | Steering Codec Roundtrip |
| TC_DIA_001 | DIA-REQ-001 | Front-Left Diagnostic Fault Injection |
| TC_DIA_002 | DIA-REQ-002 | Front-Right Diagnostic Fault Injection |
| TC_DIA_003 | DIA-REQ-003 | Front-Left Diagnostic Fault Clearing |
| TC_DIA_004 | DIA-REQ-004 | Front-Right Diagnostic Fault Clearing |
| TC_DIA_005 | DIA-REQ-005 | ECU Health Observability |
| TC_DIA_006 | DIA-REQ-006 | Runtime Fault Injection |
| TC_DIA_007 | DIA-REQ-007 | Runtime Fault Recovery |
| TC_CAN_001 | CAN-REQ-003 | CAN Arbitration Priority |
| TC_CAN_002 | CAN-REQ-001 | CAN Bitrate |
| TC_CAN_003 | CAN-REQ-008 | CAN Trace Recording |
| TC_CAN_004 | CAN-REQ-014 | CAN Arbitration Statistics |
| TC_CAN_005 | ABS-REQ-007 | ABS Message Periodicity |
| TC_CAN_006 | PT-REQ-006 | Powertrain Message Periodicity |
| TC_CAN_007 | STR-REQ-003 | Steering Message Periodicity |
| TC_CAN_008 | CAN-REQ-018 | CAN Bus Utilization |
| TC_CAN_009 | CAN-REQ-009 | CAN Trace Timing Evidence |
| TC_CAN_010 | CAN-REQ-006 | CAN Transmission Duration |
| TC_CAN_011 | CAN-REQ-007 | CAN Waiting Time |
| TC_CAN_012 | CAN-REQ-013 | CAN Frame Count |
| TC_CAN_013 | CAN-REQ-015 | CAN Average Waiting Time |
| TC_CAN_014 | CAN-REQ-016 | CAN Maximum Waiting Time |
| TC_CAN_015 | CAN-REQ-017 | CAN Total Transmission Time |
| TC_CAN_016 | CAN-REQ-019 | CAN Per-Message Statistics |
| TC_CAN_017 | CAN-REQ-005 | CAN Pending Frame Behavior |
| TC_CAN_018 | CAN-REQ-012 | CAN Waiting-Time Trace Evidence |
| TC_CAN_019 | CAN-REQ-004 | CAN Exclusive Bus Access |
| TC_CAN_020 | CAN-REQ-010 | CAN Trace Identifier Evidence |
| TC_CAN_021 | CAN-REQ-011 | CAN Trace Payload Evidence |
| TC_MSG_001 | MSG-ABS-REQ-003 | ABS Codec Roundtrip |

---

## 4. Traceability

Project-level traceability is maintained in `TraceabilityMatrix.md`.

The matrix distinguishes direct verification from supporting evidence so that
implemented behavior is not incorrectly reported as formally verified.

### Verification Evidence

- `TraceabilityMatrix.md` — complete requirement-to-evidence mapping.
- `InspectionVerification.md` — formal inspection records for requirements whose
  verification method is Inspection.

## Baseline v1.2 Verification Closure

- Automated suite: **84 PASS / 0 FAIL**
- Baseline requirements: **94**
- Verified baseline requirements: **94**
- Unverified baseline requirements: **0**
- Inspection evidence: `InspectionVerification.md`

The baseline is verification-closed for the currently defined VirtualVehicle
requirements. Capabilities outside the baseline remain intentionally unclaimed.
