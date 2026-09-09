VirtualVehicle Requirements Specification

Document ID: VV-SRS-001
Version: 1.8
Status: VERIFICATION CLOSED
System: VirtualVehicle Platform

Verification Workflow

Requirement → Implementation → Test Case → Execution → Evidence → PASS / FAIL

Implementation alone does not constitute verification.

Current Baseline Evidence

Parent Baseline v1.7:
- Requirements: 264 / 264 VERIFIED
- Automated Tests: 254 PASS / 0 FAIL
- Status: VERIFICATION CLOSED

Diagnostic Fault Expansion v1.8:
- New Requirements: 15
- Dedicated Tests: 15 PASS / 0 FAIL
- DFX Status: VERIFICATION CLOSED

Complete v1.8 Automated Execution:
- Automated Tests: 269
- Passed: 269
- Failed: 0
- Pass Rate: 100.0%
- Unique Primary Requirement IDs Exercised by Test Runner: 268

Requirement Categories

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
| DFX | Diagnostic Fault Expansion | DiagnosticFaultExpansionRequirements.md |
| UDS | Unified Diagnostic Services | UdsRequirements.md |
| ISOTP | ISO-TP Transport | IsotRequirements.md |
| UDSTP | UDS / ISO-TP Integration | UdsTransportRequirements.md |
| UDSCAN | UDS Virtual CAN Transport | UdsCanTransportRequirements.md |
| UDSRT | UDS Runtime Integration | UdsRuntimeIntegrationRequirements.md |
| ETH | Automotive Ethernet | EthernetRequirements.md |
| VAL | Automated Validation | ValidationRequirements.md |

Baseline v1.8 Scope

Baseline v1.8 extends the verification-closed v1.7 platform with deterministic
runtime communication fault injection for:
- ABS CAN communication
- Powertrain CAN communication
- Steering CAN communication
- Ethernet Node A communication

The expansion includes distinct DTC registration, deterministic recovery/reset
behavior, and regression preservation of the complete v1.7 baseline.

The existing platform includes:
- deterministic vehicle simulation
- ABS, Powertrain, Steering, and Dashboard ECU behavior
- Virtual CAN arbitration, timing, trace, and statistics
- DTC management and diagnostic fault injection
- simplified UDS services
- simplified ISO-TP transport
- UDS over Virtual CAN
- shared-bus runtime UDS integration
- simplified Automotive Ethernet
- Qt validation/monitoring GUI
- automated requirements-based verification

Verification Boundary

The project is a software-in-the-loop portfolio simulation. It does not claim
full ISO 14229, ISO 15765-2, IEEE 802.3, AUTOSAR, physical CAN/Ethernet, or HIL
compliance.

Verification Closure

The final Automotive Ethernet requirement/test alignment was completed for:

- ETH-REQ-034 — Regression safety
- ETH-REQ-035 — Legacy Baseline v1.6 regression
- ETH-REQ-036 — Ethernet determinism
- ETH-REQ-037 — System determinism with Ethernet enabled

After alignment, the complete Baseline v1.8 automated suite was rerun
successfully.

Final execution evidence:

- Automated Tests: 269
- Passed: 269
- Failed: 0
- Pass Rate: 100.0%
- Unique Primary Requirement IDs Exercised by Test Runner: 268
- Parent Baseline v1.7: 254 / 254 PASS
- Diagnostic Fault Expansion: 15 / 15 PASS
- ETH-REQ-034 through ETH-REQ-037: ALIGNED / PASS

Baseline v1.8 Status: VERIFICATION CLOSED

The value 268 represents unique primary requirement IDs exercised by the
automated Test Runner and shall not be interpreted as the total number of
defined requirements. The verification baseline also includes inspection and
regression evidence, and test-to-requirement relationships are not required to
be one-to-one.
