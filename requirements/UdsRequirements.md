# VirtualVehicle — UDS Transport Requirements

## Document Information

- Project: VirtualVehicle
- Document: UDS Transport Requirements
- Baseline: v1.5
- Status: Verified / Baseline
- Verification Method: Automated Test
- Verification Result: 12 PASS / 0 FAIL

---

# 1. Purpose

This document defines the requirements for the transport integration between
the VirtualVehicle UDS diagnostic server and the ISO-TP transport layer.

The UDS transport component converts UDS requests and responses to and from
diagnostic byte payloads and uses the ISO-TP subsystem for CAN-frame
segmentation and reassembly.

This requirement set verifies the integration between UDS and ISO-TP
independently of direct VirtualCanBus transmission.

---

# 2. Scope

The UDS transport subsystem includes:

- UDS request serialization
- UDS request deserialization
- UDS response serialization
- UDS response deserialization
- ISO-TP request segmentation
- ISO-TP response segmentation
- ISO-TP request reassembly
- ISO-TP response reassembly
- Diagnostic request CAN identifier validation
- Diagnostic response CAN identifier validation
- Positive UDS response preservation
- Negative UDS response preservation
- End-to-end UDS request processing through ISO-TP
- Deterministic transport behavior

The following functionality remains outside this requirement set:

- Direct runtime transmission through VirtualCanBus
- ISO-TP Flow Control scheduling
- ISO-TP Block Size enforcement
- ISO-TP STmin timing enforcement
- Physical CAN hardware
- CAN FD diagnostics
- Extended addressing
- Functional addressing
- Full ISO 14229 conformance
- Full ISO 15765-2 conformance

Direct VirtualCanBus diagnostic transport is defined separately by
`UdsCanTransportRequirements.md`.

---

# 3. General Transport Requirements

## UDSTP-REQ-001 — UDS Request Transport

The UDS transport component shall convert a UDS request into one or more
ISO-TP CAN frames.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_001`  
Verification Status: VERIFIED / PASS

---

## UDSTP-REQ-002 — UDS Response Transport

The UDS transport component shall convert a UDS response into one or more
ISO-TP CAN frames.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_002`  
Verification Status: VERIFIED / PASS

---

# 4. CAN Identifier Requirements

## UDSTP-REQ-003 — Request CAN Identifier

The default UDS request CAN identifier shall be `0x7E0`.

All CAN frames generated for a default UDS request shall use identifier
`0x7E0`.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_003`  
Verification Status: VERIFIED / PASS

---

## UDSTP-REQ-004 — Response CAN Identifier

The default UDS response CAN identifier shall be `0x7E8`.

All CAN frames generated for a default UDS response shall use identifier
`0x7E8`.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_004`  
Verification Status: VERIFIED / PASS

---

# 5. Serialization Requirements

## UDSTP-REQ-005 — Request Serialization

The serialized UDS request payload shall contain the request Service
Identifier as the first byte followed by the request payload bytes in their
original order.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_005`  
Verification Status: VERIFIED / PASS

---

## UDSTP-REQ-006 — Response Serialization

The serialized UDS response payload shall contain the response Service
Identifier as the first byte followed by the response payload bytes in their
original order.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_006`  
Verification Status: VERIFIED / PASS

---

# 6. Response Requirements

## UDSTP-REQ-007 — Positive Response Preservation

A positive UDS response transported through ISO-TP shall remain identified as
a positive response after ISO-TP reassembly and UDS response deserialization.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_007`  
Verification Status: VERIFIED / PASS

---

## UDSTP-REQ-008 — Negative Response Preservation

A UDS negative response beginning with Service Identifier `0x7F` shall remain
identified as a negative response after ISO-TP reassembly and UDS response
deserialization.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_008`  
Verification Status: VERIFIED / PASS

---

# 7. Validation Requirements

## UDSTP-REQ-009 — Request Identifier Validation

The UDS transport component shall reject request CAN frames whose arbitration
identifier does not match the configured UDS request CAN identifier.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_009`  
Verification Status: VERIFIED / PASS

---

## UDSTP-REQ-010 — Response Identifier Validation

The UDS transport component shall reject response CAN frames whose arbitration
identifier does not match the configured UDS response CAN identifier.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_010`  
Verification Status: VERIFIED / PASS

---

# 8. End-to-End Processing Requirements

## UDSTP-REQ-011 — End-to-End UDS Processing

The UDS transport component shall support the following processing path:

1. Receive a UDS request.
2. Serialize the UDS request.
3. Segment the request using ISO-TP.
4. Reassemble the ISO-TP request.
5. Process the request using UdsServer.
6. Serialize the UDS response.
7. Segment the response using ISO-TP.
8. Reassemble the ISO-TP response.
9. Return the reconstructed UDS response.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_011`  
Verification Status: VERIFIED / PASS

---

## UDSTP-REQ-012 — Deterministic Transport

For identical UDS requests and identical UDS server state, the UDS transport
component shall produce identical final UDS responses.

Verification Method: Automated Test  
Verification Test: `TC_UDSTP_012`  
Verification Status: VERIFIED / PASS

---

# 9. Verified Architecture

The verified architecture for this requirement set is:

```text
UdsRequest
    |
    v
UdsTransport
    |
    v
ISO-TP Segmentation
    |
    v
ISO-TP CAN Frames
    |
    v
ISO-TP Reassembly
    |
    v
UdsServer
    |
    v
UdsResponse
    |
    v
ISO-TP Segmentation
    |
    v
ISO-TP CAN Frames
    |
    v
ISO-TP Reassembly
    |
    v
UdsResponse
```

Virtual CAN transmission is verified separately by the UDS CAN Transport
requirement set.

---

# 10. Verification Summary

| Requirement Range | Test Range | Result |
|---|---|---|
| `UDSTP-REQ-001` – `UDSTP-REQ-012` | `TC_UDSTP_001` – `TC_UDSTP_012` | PASS |

- Requirements: **12**
- Verified requirements: **12**
- Unverified requirements: **0**
- Automated tests: **12**
- Passed: **12**
- Failed: **0**

UDS Transport Baseline v1.5 verification status:

**VERIFICATION CLOSED**

---

# 11. Verification Boundary

The following capabilities are not claimed as verified by this requirement
set:

- Direct VirtualCanBus transport
- Runtime SimulationEngine diagnostic scheduling
- Parallel diagnostic clients
- ISO-TP Flow Control scheduling
- ISO-TP Block Size enforcement
- ISO-TP STmin timing enforcement
- CAN FD diagnostics
- Extended diagnostic addressing
- Functional diagnostic addressing
- Physical CAN hardware
- Full ISO 14229 compliance
- Full ISO 15765-2 compliance