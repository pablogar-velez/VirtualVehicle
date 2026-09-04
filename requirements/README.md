# VirtualVehicle Requirements Specification

**Document ID:** VV-SRS-001  
**Version:** 1.3  
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
| VAL | Automated Validation | ValidationRequirements.md |

---

## 3. Baseline v1.3 Scope

Baseline v1.3 extends the verified VirtualVehicle platform with Diagnostic
Trouble Code management.

The DTC subsystem provides:

- Front-left wheel-speed sensor DTC `0xC0031`
- Front-right wheel-speed sensor DTC `0xC0034`
- Runtime DTC activation
- Runtime DTC recovery
- Stored diagnostic history
- Individual DTC clearing
- Global DTC clearing
- First-detection timestamps
- Last-detection timestamps
- Fault occurrence counting
- DTC lookup
- DTC collection access
- Active DTC queries
- Deterministic diagnostic reset behavior
- Deterministic diagnostic execution

The DTC subsystem is integrated with the existing wheel-speed sensor and ABS
simulation path.

---

## 4. Current Automated Test Coverage

**Suite result: 102 Passed / 0 Failed**

The Baseline v1.3 automated validation suite contains:

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
| CAN | `TC_CAN_001` – `TC_CAN_022` |
| CAN Messages | `TC_MSG_001` – `TC_MSG_010` |
| Validation Framework | `TC_VAL_001` – `TC_VAL_006` |

Total automated tests: **102**

---

## 5. DTC Automated Verification

| Test Case | Primary Requirement | Description |
|---|---|---|
| TC_DTC_001 | DTC-REQ-001 | Front-Left DTC Definition |
| TC_DTC_002 | DTC-REQ-002 | Front-Right DTC Definition |
| TC_DTC_003 | DTC-REQ-003 | Front-Left DTC Activation |
| TC_DTC_004 | DTC-REQ-004 | Front-Right DTC Activation |
| TC_DTC_005 | DTC-REQ-005 | Front-Left DTC Recovery |
| TC_DTC_006 | DTC-REQ-006 | Front-Right DTC Recovery |
| TC_DTC_007 | DTC-REQ-007 | Individual DTC Clear |
| TC_DTC_008 | DTC-REQ-008 | Clear All DTCs |
| TC_DTC_009 | DTC-REQ-009 | First Detection Timestamp |
| TC_DTC_010 | DTC-REQ-010 | Last Detection Timestamp |
| TC_DTC_011 | DTC-REQ-011 | DTC Occurrence Count |
| TC_DTC_012 | DTC-REQ-012 | DTC Lookup |
| TC_DTC_013 | DTC-REQ-013 | DTC Collection Access |
| TC_DTC_014 | DTC-REQ-014 | Active DTC Query |
| TC_DTC_015 | DTC-REQ-015 | Runtime DTC Detection |
| TC_DTC_016 | DTC-REQ-016 | Runtime DTC Recovery |
| TC_DTC_017 | DTC-REQ-017 | Diagnostic Reset |
| TC_DTC_018 | DTC-REQ-018 | Deterministic Diagnostic Behavior |

All DTC tests completed with PASS status.

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

## 7. Baseline v1.3 Verification Closure

- Automated suite: **102 PASS / 0 FAIL**
- Baseline requirements: **112**
- Verified baseline requirements: **112**
- Unverified baseline requirements: **0**
- DTC requirements: **18**
- DTC requirements verified: **18**
- Inspection evidence: `InspectionVerification.md`

Baseline v1.3 is verification-closed for the currently defined VirtualVehicle
requirements.

---

## 8. Baseline Boundary

The following capabilities remain outside Baseline v1.3 and are not claimed as
requirements-verified:

- UDS diagnostic services
- ISO-TP transport
- Diagnostic sessions
- Security access
- ECU reprogramming / flashing
- AUTOSAR runtime behavior
- Automotive Ethernet
- Physical CAN transceiver or hardware-bus validation

Future requirements for these capabilities shall be introduced in a subsequent
baseline before implementation is claimed as requirements-verified.