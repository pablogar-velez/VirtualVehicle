# VirtualVehicle Requirements Specification

**Document ID:** VV-SRS-001  
**Version:** 1.4  
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
| DTC | Diagnostic Trouble Codes | DtcRequirements.md |
| UDS | Unified Diagnostic Services | UdsRequirements.md |
| VAL | Automated Validation | ValidationRequirements.md |

---

## 3. Baseline v1.4 Scope

Baseline v1.4 extends the verified VirtualVehicle platform with a simplified
Unified Diagnostic Services diagnostic layer.

The UDS subsystem provides:

- Diagnostic request representation
- Diagnostic response representation
- UDS server request processing
- Positive diagnostic responses
- Negative diagnostic responses
- Diagnostic Session Control service `0x10`
- Default Diagnostic Session `0x01`
- Extended Diagnostic Session `0x03`
- Read DTC Information service `0x19`
- Active DTC reporting
- Clear Diagnostic Information service `0x14`
- DTC clearing through the existing DTC Manager
- Read Data By Identifier service `0x22`
- Vehicle Speed DID `0xF100`
- Engine RPM DID `0xF101`
- Steering Angle DID `0xF102`
- Runtime diagnostic processing
- Current simulation-state access
- Diagnostic reset behavior
- Deterministic diagnostic responses

The UDS subsystem is integrated with the existing DTC Manager and
SimulationEngine.

Baseline v1.4 implements a simplified UDS diagnostic service model for
software-in-the-loop simulation and verification.

It does not claim full ISO 14229 compliance.

---

## 4. Current Automated Test Coverage

**Suite result: 135 Passed / 0 Failed**

The Baseline v1.4 automated validation suite contains:

| Domain | Test Range |
|---|---|
| System | `TC_SYS_001` – `TC_SYS_004` |
| Vehicle | `TC_VEH_001` – `TC_VEH_011` |
| ABS | `TC_ABS_001` – `TC_ABS_007` |
| Sensors | `TC_SNS_001` – `TC_SNS_006` |
| Powertrain | `TC_PT_001` – `TC_PT_007` |
| Steering | `TC_STR_001` – `TC_STR_004` |
| Diagnostics / Fault Injection | `TC_DIA_001` – `TC_DIA_007` |
| Diagnostic Trouble Codes | `TC_DTC_001` – `TC_DTC_018` |
| Unified Diagnostic Services | `TC_UDS_001` – `TC_UDS_033` |
| CAN | `TC_CAN_001` – `TC_CAN_022` |
| CAN Messages | `TC_MSG_001` – `TC_MSG_010` |
| Validation Framework | `TC_VAL_001` – `TC_VAL_006` |

Total automated tests: **135**

---

## 5. UDS Automated Verification

| Test Case | Primary Requirement | Description |
|---|---|---|
| TC_UDS_001 | UDS-REQ-001 | UDS Request Representation |
| TC_UDS_002 | UDS-REQ-002 | UDS Response Representation |
| TC_UDS_003 | UDS-REQ-003 | UDS Server Request Processing |
| TC_UDS_004 | UDS-REQ-004 | Positive Response SID |
| TC_UDS_005 | UDS-REQ-005 | Negative Response Format |
| TC_UDS_006 | UDS-REQ-006 | Unsupported Service |
| TC_UDS_007 | UDS-REQ-007 | Diagnostic Session Control Support |
| TC_UDS_008 | UDS-REQ-008 | Default Diagnostic Session |
| TC_UDS_009 | UDS-REQ-009 | Extended Diagnostic Session |
| TC_UDS_010 | UDS-REQ-010 | Diagnostic Session State Update |
| TC_UDS_011 | UDS-REQ-011 | Unsupported Diagnostic Session |
| TC_UDS_012 | UDS-REQ-012 | Read DTC Information Support |
| TC_UDS_013 | UDS-REQ-013 | Read Active DTCs |
| TC_UDS_014 | UDS-REQ-014 | Front-Left DTC Reporting |
| TC_UDS_015 | UDS-REQ-015 | Front-Right DTC Reporting |
| TC_UDS_016 | UDS-REQ-016 | No Active DTC Response |
| TC_UDS_017 | UDS-REQ-017 | Clear Diagnostic Information Support |
| TC_UDS_018 | UDS-REQ-018 | Clear All DTCs Through UDS |
| TC_UDS_019 | UDS-REQ-019 | Clear Diagnostic Positive Response |
| TC_UDS_020 | UDS-REQ-020 | Read Data By Identifier Support |
| TC_UDS_021 | UDS-REQ-021 | Vehicle Speed DID Definition |
| TC_UDS_022 | UDS-REQ-022 | Vehicle Speed DID Readout |
| TC_UDS_023 | UDS-REQ-023 | Engine RPM DID Definition |
| TC_UDS_024 | UDS-REQ-024 | Engine RPM DID Readout |
| TC_UDS_025 | UDS-REQ-025 | Steering Angle DID Definition |
| TC_UDS_026 | UDS-REQ-026 | Steering Angle DID Readout |
| TC_UDS_027 | UDS-REQ-027 | Unsupported DID |
| TC_UDS_028 | UDS-REQ-028 | DTC Manager Access |
| TC_UDS_029 | UDS-REQ-029 | DTC Clear Integration |
| TC_UDS_030 | UDS-REQ-030 | Runtime Diagnostic Processing |
| TC_UDS_031 | UDS-REQ-031 | Current Vehicle Data |
| TC_UDS_032 | UDS-REQ-032 | UDS Reset State |
| TC_UDS_033 | UDS-REQ-033 | Deterministic Diagnostic Response |

All UDS tests completed with PASS status.

---

## 6. Traceability

Project-level traceability is maintained in `TraceabilityMatrix.md`.

The matrix distinguishes direct verification from supporting evidence so that
implemented behavior is not incorrectly reported as formally verified.

### Verification Evidence

- `TraceabilityMatrix.md` — complete requirement-to-evidence mapping.
- `InspectionVerification.md` — formal inspection records for requirements whose
  verification method is Inspection.
- Automated Test Runner — executable requirements-based verification evidence.

---

## 7. Baseline v1.4 Verification Closure

- Automated suite: **135 PASS / 0 FAIL**
- Baseline requirements: **145**
- Verified baseline requirements: **145**
- Unverified baseline requirements: **0**
- DTC requirements: **18**
- DTC requirements verified: **18**
- UDS requirements: **33**
- UDS requirements verified: **33**
- Inspection evidence: `InspectionVerification.md`

Baseline v1.4 is verification-closed for the currently defined VirtualVehicle
requirements.

---

## 8. Baseline Boundary

The following capabilities remain outside Baseline v1.4 and are not claimed as
requirements-verified:

- Full ISO 14229 compliance
- ISO-TP transport
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

Future requirements for these capabilities shall be introduced in a subsequent
baseline before implementation is claimed as requirements-verified.