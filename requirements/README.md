# VirtualVehicle Requirements Specification

**Document ID:** VV-SRS-001  
**Version:** 1.5  
**Status:** Baseline / Verification Closed  
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

A requirement shall only be classified as `VERIFIED / PASS` when the defined
verification evidence has been successfully executed or inspected.

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
| ISOTP | ISO-TP Transport | IsotpRequirements.md |
| UDSTP | UDS / ISO-TP Integration | UdsTransportRequirements.md |
| UDSCAN | UDS Virtual CAN Transport | UdsCanTransportRequirements.md |
| VAL | Automated Validation | ValidationRequirements.md |

---

## 3. Baseline v1.5 Scope

Baseline v1.5 extends the verified VirtualVehicle platform with a simplified
ISO-TP diagnostic transport layer and UDS communication through the
Virtual CAN Bus.

The diagnostic stack now provides:

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
- ISO-TP Single Frame transport
- ISO-TP First Frame transport
- ISO-TP Consecutive Frame transport
- ISO-TP Flow Control frame representation
- ISO-TP diagnostic payload segmentation
- ISO-TP diagnostic payload reassembly
- ISO-TP sequence-number validation
- ISO-TP sequence-number rollover
- UDS request serialization
- UDS response serialization
- UDS request transport through ISO-TP
- UDS response transport through ISO-TP
- Diagnostic request CAN ID `0x7E0`
- Diagnostic response CAN ID `0x7E8`
- UDS request transmission through VirtualCanBus
- UDS response transmission through VirtualCanBus
- Diagnostic CAN trace evidence
- Diagnostic CAN timing evidence
- Single-frame diagnostic transactions
- Multi-frame diagnostic transactions
- Positive-response preservation through the transport stack
- Negative-response preservation through the transport stack
- Deterministic diagnostic CAN transactions

The UDS subsystem remains integrated with the existing DTC Manager and
SimulationEngine.

The ISO-TP subsystem provides deterministic diagnostic payload segmentation
and reassembly using simulated Classic CAN frames.

The UDS Transport subsystem connects UDS request and response processing to
ISO-TP.

The UDS CAN Transport subsystem routes diagnostic ISO-TP frames through the
existing VirtualCanBus for isolated diagnostic transactions.

Baseline v1.5 implements a simplified UDS-over-ISO-TP diagnostic communication
model for software-in-the-loop simulation and verification.

It does not claim full ISO 14229 or ISO 15765-2 compliance.

---

## 4. Diagnostic Architecture

The verified Baseline v1.5 diagnostic transport architecture is:

```text
Diagnostic Client
       |
       v
    UdsRequest
       |
       v
   UdsTransport
       |
       v
     ISO-TP
       |
       v
     0x7E0
       |
       v
 VirtualCanBus
       |
       v
     ISO-TP
       |
       v
    UdsServer
       |
       v
     ISO-TP
       |
       v
     0x7E8
       |
       v
 VirtualCanBus
       |
       v
     ISO-TP
       |
       v
   UdsResponse
       |
       v
Diagnostic Client
```

The current UDS CAN transport is verified as an isolated synchronous
diagnostic transaction.

Integration of diagnostic traffic into the SimulationEngine shared runtime CAN
traffic remains outside the Baseline v1.5 boundary.

---

## 5. Current Automated Test Coverage

**Suite result: 188 Passed / 0 Failed**

The Baseline v1.5 automated validation suite contains:

| Domain | Test Range | Count |
|---|---|---:|
| System | `TC_SYS_001` – `TC_SYS_004` | 4 |
| Vehicle | `TC_VEH_001` – `TC_VEH_011` | 11 |
| ABS | `TC_ABS_001` – `TC_ABS_007` | 7 |
| Sensors | `TC_SNS_001` – `TC_SNS_006` | 6 |
| Powertrain | `TC_PT_001` – `TC_PT_007` | 7 |
| Steering | `TC_STR_001` – `TC_STR_004` | 4 |
| Diagnostics / Fault Injection | `TC_DIA_001` – `TC_DIA_007` | 7 |
| Diagnostic Trouble Codes | `TC_DTC_001` – `TC_DTC_018` | 18 |
| Unified Diagnostic Services | `TC_UDS_001` – `TC_UDS_033` | 33 |
| ISO-TP | `TC_ISOTP_001` – `TC_ISOTP_026` | 26 |
| UDS Transport | `TC_UDSTP_001` – `TC_UDSTP_012` | 12 |
| UDS CAN Transport | `TC_UDSCAN_001` – `TC_UDSCAN_015` | 15 |
| CAN | `TC_CAN_001` – `TC_CAN_022` | 22 |
| CAN Messages | `TC_MSG_001` – `TC_MSG_010` | 10 |
| Validation Framework | `TC_VAL_001` – `TC_VAL_006` | 6 |

Total automated tests: **188**

---

## 6. UDS Automated Verification

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

**Result: 33 PASS / 0 FAIL**

---

## 7. ISO-TP Automated Verification

| Test Range | Requirement Range | Result |
|---|---|---|
| `TC_ISOTP_001` – `TC_ISOTP_026` | `ISOTP-REQ-001` – `ISOTP-REQ-026` | 26 PASS / 0 FAIL |

The ISO-TP automated verification covers:

- Transport component behavior
- Classic CAN frame size
- Standard 11-bit CAN identifiers
- Maximum payload length
- Empty-payload rejection
- Single Frame generation
- Single Frame payload capacity
- Single Frame PCI encoding
- First Frame generation
- First Frame payload-length encoding
- First Frame data capacity
- Consecutive Frame generation
- Consecutive Frame data capacity
- Initial sequence number
- Sequence-number increment
- Sequence-number rollover
- Flow Control frame representation
- Flow Status representation
- Flow Control parameters
- Single Frame reassembly
- Multi-frame reassembly
- Sequence-number validation
- CAN identifier validation
- Incomplete-message detection
- Deterministic segmentation
- Deterministic reassembly

All ISO-TP tests completed with PASS status.

**Result: 26 PASS / 0 FAIL**

---

## 8. UDS Transport Automated Verification

| Test Case | Primary Requirement | Description |
|---|---|---|
| TC_UDSTP_001 | UDSTP-REQ-001 | UDS Request Transport |
| TC_UDSTP_002 | UDSTP-REQ-002 | UDS Response Transport |
| TC_UDSTP_003 | UDSTP-REQ-003 | Request CAN Identifier |
| TC_UDSTP_004 | UDSTP-REQ-004 | Response CAN Identifier |
| TC_UDSTP_005 | UDSTP-REQ-005 | Request Serialization |
| TC_UDSTP_006 | UDSTP-REQ-006 | Response Serialization |
| TC_UDSTP_007 | UDSTP-REQ-007 | Positive Response Preservation |
| TC_UDSTP_008 | UDSTP-REQ-008 | Negative Response Preservation |
| TC_UDSTP_009 | UDSTP-REQ-009 | Request Identifier Validation |
| TC_UDSTP_010 | UDSTP-REQ-010 | Response Identifier Validation |
| TC_UDSTP_011 | UDSTP-REQ-011 | End-to-End UDS Processing |
| TC_UDSTP_012 | UDSTP-REQ-012 | Deterministic Transport |

All UDS Transport tests completed with PASS status.

**Result: 12 PASS / 0 FAIL**

---

## 9. UDS CAN Transport Automated Verification

| Test Case | Primary Requirement | Description |
|---|---|---|
| TC_UDSCAN_001 | UDSCAN-REQ-001 | UDS CAN Transport Component |
| TC_UDSCAN_002 | UDSCAN-REQ-002 | Request Transmission |
| TC_UDSCAN_003 | UDSCAN-REQ-003 | Response Transmission |
| TC_UDSCAN_004 | UDSCAN-REQ-004 | Request Identifier |
| TC_UDSCAN_005 | UDSCAN-REQ-005 | Response Identifier |
| TC_UDSCAN_006 | UDSCAN-REQ-006 | Request Reassembly |
| TC_UDSCAN_007 | UDSCAN-REQ-007 | Response Reassembly |
| TC_UDSCAN_008 | UDSCAN-REQ-008 | End-to-End Diagnostic Transaction |
| TC_UDSCAN_009 | UDSCAN-REQ-009 | Diagnostic CAN Trace |
| TC_UDSCAN_010 | UDSCAN-REQ-010 | Diagnostic CAN Timing |
| TC_UDSCAN_011 | UDSCAN-REQ-011 | Transaction Evidence |
| TC_UDSCAN_012 | UDSCAN-REQ-012 | Positive Response Preservation |
| TC_UDSCAN_013 | UDSCAN-REQ-013 | Negative Response Preservation |
| TC_UDSCAN_014 | UDSCAN-REQ-014 | Multi-Frame Diagnostic Transport |
| TC_UDSCAN_015 | UDSCAN-REQ-015 | Deterministic CAN Diagnostic Transaction |

All UDS CAN Transport tests completed with PASS status.

**Result: 15 PASS / 0 FAIL**

---

## 10. Traceability

Project-level traceability is maintained in `TraceabilityMatrix.md`.

The matrix distinguishes direct verification from supporting evidence so that
implemented behavior is not incorrectly reported as formally verified.

### Verification Evidence

- `TraceabilityMatrix.md` — complete requirement-to-evidence mapping.
- `InspectionVerification.md` — formal inspection records for requirements whose
  verification method is Inspection.
- Automated Test Runner — executable requirements-based verification evidence.
- VirtualCanBus trace — simulated CAN transmission evidence.
- TestResult expected / actual fields — test execution evidence.
- TestResult timing fields — execution and response timing evidence where
  applicable.

---

## 11. Baseline Evolution

### Baseline v1.2

- Automated tests: **84**
- Verified requirements: **94**

### Baseline v1.3

Added Diagnostic Trouble Code management.

- Automated tests: **102**
- Verified requirements: **112**

### Baseline v1.4

Added simplified Unified Diagnostic Services.

- Automated tests: **135**
- Verified requirements: **145**

### Baseline v1.5

Added:

- ISO-TP transport core
- UDS / ISO-TP transport integration
- UDS diagnostic transport through VirtualCanBus

Baseline v1.5:

- Automated tests: **188**
- Verified requirements: **198**

---

## 12. Baseline v1.5 Verification Closure

- Automated suite: **188 PASS / 0 FAIL**
- Baseline requirements: **198**
- Verified baseline requirements: **198**
- Unverified baseline requirements: **0**
- DTC requirements: **18**
- DTC requirements verified: **18**
- UDS requirements: **33**
- UDS requirements verified: **33**
- ISO-TP requirements: **26**
- ISO-TP requirements verified: **26**
- UDS Transport requirements: **12**
- UDS Transport requirements verified: **12**
- UDS CAN Transport requirements: **15**
- UDS CAN Transport requirements verified: **15**
- Inspection evidence: `InspectionVerification.md`

Baseline v1.5 is verification-closed for the currently defined VirtualVehicle
requirements.

---

## 13. Baseline Boundary

The following capabilities remain outside Baseline v1.5 and are not claimed as
requirements-verified:

- Full ISO 14229 compliance
- Full ISO 15765-2 compliance
- Runtime UDS CAN integration into the SimulationEngine shared ECU bus
- Parallel diagnostic clients
- ISO-TP runtime Flow Control scheduling
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

Future requirements for these capabilities shall be introduced in a subsequent
baseline before implementation is claimed as requirements-verified.

---

## 14. Baseline Status

```text
VirtualVehicle Baseline v1.5

Automated Tests:        188
Passed:                 188
Failed:                   0

Requirements:           198
Verified:               198
Unverified:               0

Status:
VERIFICATION CLOSED
```