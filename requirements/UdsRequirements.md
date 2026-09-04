# VirtualVehicle — UDS Requirements

## Document Information

- Project: VirtualVehicle
- Document: UDS Requirements
- Baseline: v1.4
- Status: Baseline / VERIFIED
- Verification Method: Automated Test unless otherwise specified

---

# 1. Purpose

This document defines the requirements for a simplified Unified Diagnostic
Services (UDS) subsystem within the VirtualVehicle simulation platform.

The UDS subsystem shall provide diagnostic request/response behavior over the
existing VirtualVehicle communication architecture.

The initial implementation shall provide selected diagnostic services required
to inspect ECU state, read Diagnostic Trouble Codes, clear diagnostic
information, and access selected simulated data.

This baseline does not claim full ISO 14229 compliance.

---

# 2. Scope

The UDS subsystem includes:

- Diagnostic request representation
- Diagnostic response representation
- UDS server processing
- Positive responses
- Negative responses
- Diagnostic Session Control
- Read DTC Information
- Clear Diagnostic Information
- Read Data By Identifier
- Deterministic request processing
- Integration with the existing DTC Manager
- Integration with simulated vehicle state

The following functionality is outside the scope of this document:

- Full ISO 14229 compliance
- ISO-TP multi-frame transport
- Security Access
- ECU Reset
- Communication Control
- Routine Control
- Request Download
- Transfer Data
- ECU flashing / reprogramming
- Physical diagnostic hardware
- Physical CAN transceiver validation

These capabilities may be introduced by future requirements.

---

# 3. Diagnostic Request and Response Framework

## UDS-REQ-001 — Diagnostic Request Representation

The diagnostic subsystem shall provide a representation for a diagnostic
request.

The request shall contain at minimum:

- Service Identifier
- Request payload

Verification Method: Automated Test

Verification Evidence: `TC_UDS_001`

Status: VERIFIED / PASS

---

## UDS-REQ-002 — Diagnostic Response Representation

The diagnostic subsystem shall provide a representation for a diagnostic
response.

The response shall contain at minimum:

- Response payload
- Positive or negative response indication

Verification Method: Automated Test

Verification Evidence: `TC_UDS_002`

Status: VERIFIED / PASS

---

## UDS-REQ-003 — UDS Server

The diagnostic subsystem shall provide a UDS server capable of receiving a
diagnostic request and producing a corresponding diagnostic response.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_003`

Status: VERIFIED / PASS

---

# 4. Positive and Negative Responses

## UDS-REQ-004 — Positive Response Service Identifier

For a successfully processed diagnostic request, the positive response Service
Identifier shall equal the requested Service Identifier plus `0x40`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_004`

Status: VERIFIED / PASS

---

## UDS-REQ-005 — Negative Response Format

When a supported diagnostic service cannot successfully process a valid
request, the server shall provide a negative response.

The negative response shall identify:

- Negative Response Service Identifier `0x7F`
- Original requested Service Identifier
- Negative Response Code

Verification Method: Automated Test

Verification Evidence: `TC_UDS_005`

Status: VERIFIED / PASS

---

## UDS-REQ-006 — Unsupported Service

When the UDS server receives an unsupported Service Identifier, it shall return
a negative response indicating that the requested service is not supported.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_006`

Status: VERIFIED / PASS

---

# 5. Diagnostic Session Control — Service 0x10

## UDS-REQ-007 — Diagnostic Session Control Support

The UDS server shall support Diagnostic Session Control service `0x10`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_007`

Status: VERIFIED / PASS

---

## UDS-REQ-008 — Default Diagnostic Session

The UDS subsystem shall support a Default Diagnostic Session.

The default session identifier shall be:

`0x01`

Verification Method: Automated Test

Verification Evidence: `TC_UDS_008`

Status: VERIFIED / PASS

---

## UDS-REQ-009 — Extended Diagnostic Session

The UDS subsystem shall support an Extended Diagnostic Session.

The extended session identifier shall be:

`0x03`

Verification Method: Automated Test

Verification Evidence: `TC_UDS_009`

Status: VERIFIED / PASS

---

## UDS-REQ-010 — Session State Update

When a supported Diagnostic Session Control request is successfully processed,
the UDS server shall update its current diagnostic session.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_010`

Status: VERIFIED / PASS

---

## UDS-REQ-011 — Unsupported Diagnostic Session

When an unsupported diagnostic session identifier is requested, the server
shall return a negative response.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_011`

Status: VERIFIED / PASS

---

# 6. Read DTC Information — Service 0x19

## UDS-REQ-012 — Read DTC Information Support

The UDS server shall support Read DTC Information service `0x19`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_012`

Status: VERIFIED / PASS

---

## UDS-REQ-013 — Read Active DTCs

The Read DTC Information service shall provide access to currently Active DTCs
stored by the DTC Manager.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_013`

Status: VERIFIED / PASS

---

## UDS-REQ-014 — Front-Left DTC Reporting

When DTC `0xC0031` is Active, a Read DTC Information request shall be capable
of returning diagnostic information identifying DTC `0xC0031`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_014`

Status: VERIFIED / PASS

---

## UDS-REQ-015 — Front-Right DTC Reporting

When DTC `0xC0034` is Active, a Read DTC Information request shall be capable
of returning diagnostic information identifying DTC `0xC0034`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_015`

Status: VERIFIED / PASS

---

## UDS-REQ-016 — No Active DTC Response

When no registered DTC is Active, the Read DTC Information service shall
successfully return a response indicating that no Active DTCs are present.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_016`

Status: VERIFIED / PASS

---

# 7. Clear Diagnostic Information — Service 0x14

## UDS-REQ-017 — Clear Diagnostic Information Support

The UDS server shall support Clear Diagnostic Information service `0x14`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_017`

Status: VERIFIED / PASS

---

## UDS-REQ-018 — Clear All DTCs Through UDS

A valid Clear Diagnostic Information request shall clear all registered DTC
records through the DTC Manager.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_018`

Status: VERIFIED / PASS

---

## UDS-REQ-019 — Clear Diagnostic Response

After a successful Clear Diagnostic Information request, the UDS server shall
return a positive response.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_019`

Status: VERIFIED / PASS

---

# 8. Read Data By Identifier — Service 0x22

## UDS-REQ-020 — Read Data By Identifier Support

The UDS server shall support Read Data By Identifier service `0x22`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_020`

Status: VERIFIED / PASS

---

## UDS-REQ-021 — Vehicle Speed DID

The UDS subsystem shall define a Data Identifier for simulated vehicle speed.

Initial DID:

`0xF100`

Verification Method: Automated Test

Verification Evidence: `TC_UDS_021`

Status: VERIFIED / PASS

---

## UDS-REQ-022 — Vehicle Speed Readout

A valid Read Data By Identifier request for DID `0xF100` shall return the
current simulated vehicle speed.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_022`

Status: VERIFIED / PASS

---

## UDS-REQ-023 — Engine Speed DID

The UDS subsystem shall define a Data Identifier for simulated engine RPM.

Initial DID:

`0xF101`

Verification Method: Automated Test

Verification Evidence: `TC_UDS_023`

Status: VERIFIED / PASS

---

## UDS-REQ-024 — Engine Speed Readout

A valid Read Data By Identifier request for DID `0xF101` shall return the
current simulated engine RPM.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_024`

Status: VERIFIED / PASS

---

## UDS-REQ-025 — Steering Angle DID

The UDS subsystem shall define a Data Identifier for simulated steering angle.

Initial DID:

`0xF102`

Verification Method: Automated Test

Verification Evidence: `TC_UDS_025`

Status: VERIFIED / PASS

---

## UDS-REQ-026 — Steering Angle Readout

A valid Read Data By Identifier request for DID `0xF102` shall return the
current simulated steering angle.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_026`

Status: VERIFIED / PASS

---

## UDS-REQ-027 — Unsupported DID

When a Read Data By Identifier request references an unsupported DID, the UDS
server shall return a negative response.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_027`

Status: VERIFIED / PASS

---

# 9. DTC Manager Integration

## UDS-REQ-028 — DTC Manager Access

The UDS server shall use the existing DTC Manager as the diagnostic source for
DTC information.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_028`

Status: VERIFIED / PASS

---

## UDS-REQ-029 — DTC Clear Integration

The UDS Clear Diagnostic Information service shall clear DTC information
through the existing DTC Manager rather than maintaining a separate diagnostic
fault database.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_029`

Status: VERIFIED / PASS

---

# 10. Simulation Integration

## UDS-REQ-030 — Runtime Diagnostic Processing

UDS requests shall be capable of being processed during normal VirtualVehicle
simulation execution without requiring a simulation restart.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_030`

Status: VERIFIED / PASS

---

## UDS-REQ-031 — Current Vehicle Data

Read Data By Identifier responses shall use the current VirtualVehicle
simulation state at the time the request is processed.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_031`

Status: VERIFIED / PASS

---

# 11. Reset Behavior

## UDS-REQ-032 — UDS Reset State

When the VirtualVehicle simulation is reset, the UDS subsystem shall return to
its initial diagnostic state.

The initial diagnostic session shall be Default Session `0x01`.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_032`

Status: VERIFIED / PASS

---

# 12. Determinism

## UDS-REQ-033 — Deterministic Diagnostic Responses

For identical initial conditions, diagnostic requests, DTC state, vehicle
state, and simulation timing, the UDS subsystem shall produce identical
diagnostic responses.

Verification Method: Automated Test

Verification Evidence: `TC_UDS_033`

Status: VERIFIED / PASS

---

# 13. Initial Service Inventory

| Service | SID | Initial Capability |
|---|---:|---|
| Diagnostic Session Control | `0x10` | Default and Extended Session |
| Clear Diagnostic Information | `0x14` | Clear all DTCs |
| Read DTC Information | `0x19` | Read Active DTCs |
| Read Data By Identifier | `0x22` | Vehicle Speed, RPM, Steering Angle |

---

# 14. Initial DID Inventory

| DID | Data |
|---|---|
| `0xF100` | Vehicle Speed |
| `0xF101` | Engine RPM |
| `0xF102` | Steering Angle |

---

# 15. Traceability

Each UDS requirement is mapped to an automated verification test case in the
VirtualVehicle traceability matrix.

UDS test cases use identifiers in the following format:

`TC_UDS_XXX`

The Baseline v1.4 UDS verification range is:

`TC_UDS_001` through `TC_UDS_033`.

---

# 16. Baseline Status

Baseline v1.4 introduces and verifies the initial VirtualVehicle UDS diagnostic
service framework.

Verification execution:

- UDS requirements: 33
- UDS automated tests: 33
- UDS tests passed: 33
- UDS tests failed: 0
- Verified UDS requirements: 33
- Unverified UDS requirements: 0

The complete VirtualVehicle Baseline v1.4 automated suite result is:

**135 PASS / 0 FAIL**

Current document status:

`BASELINE / VERIFIED`