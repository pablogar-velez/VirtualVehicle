# VirtualVehicle — UDS CAN Transport Requirements

## Document Information

- Project: VirtualVehicle
- Document: UDS CAN Transport Requirements
- Baseline: v1.5
- Status: Verified / Baseline
- Verification Method: Automated Test
- Verification Result: 15 PASS / 0 FAIL

---

# 1. Purpose

This document defines the requirements for transporting UDS diagnostic traffic
through the VirtualVehicle Virtual CAN Bus.

The UDS CAN transport component connects the existing UDS transport and
ISO-TP layers to VirtualCanBus.

This requirement set verifies simulated CAN transmission of diagnostic request
and response frames.

---

# 2. Scope

The UDS CAN transport subsystem includes:

- UDS request ISO-TP frame generation
- UDS request transmission through VirtualCanBus
- UDS request reception from VirtualCanBus
- UDS request ISO-TP reassembly
- UDS server processing
- UDS response ISO-TP frame generation
- UDS response transmission through VirtualCanBus
- UDS response reception from VirtualCanBus
- UDS response ISO-TP reassembly
- CAN trace generation for diagnostic traffic
- CAN timing participation
- Diagnostic request identifier `0x7E0`
- Diagnostic response identifier `0x7E8`
- Positive-response preservation
- Negative-response preservation
- Multi-frame diagnostic transport
- Deterministic diagnostic transactions
- Diagnostic transaction evidence

The following functionality remains outside this requirement set:

- Runtime integration into SimulationEngine shared ECU traffic
- Parallel diagnostic clients
- ISO-TP Flow Control scheduling
- ISO-TP Block Size enforcement
- ISO-TP STmin enforcement
- Functional addressing
- Extended addressing
- CAN FD diagnostics
- Physical CAN hardware
- Full ISO 14229 conformance
- Full ISO 15765-2 conformance

---

# 3. General CAN Transport Requirements

## UDSCAN-REQ-001 — CAN Transport Component

The system shall provide a UDS CAN transport component that uses
VirtualCanBus to transmit UDS diagnostic traffic.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_001`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-002 — Request Transmission

The UDS CAN transport component shall transmit ISO-TP request frames through
VirtualCanBus.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_002`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-003 — Response Transmission

The UDS CAN transport component shall transmit ISO-TP response frames through
VirtualCanBus.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_003`  
Verification Status: VERIFIED / PASS

---

# 4. Diagnostic Identifier Requirements

## UDSCAN-REQ-004 — Request Identifier

UDS diagnostic request frames transmitted through VirtualCanBus shall use CAN
identifier `0x7E0`.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_004`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-005 — Response Identifier

UDS diagnostic response frames transmitted through VirtualCanBus shall use CAN
identifier `0x7E8`.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_005`  
Verification Status: VERIFIED / PASS

---

# 5. Processing Requirements

## UDSCAN-REQ-006 — Request Reassembly

The UDS CAN transport component shall reassemble the request frames received
from VirtualCanBus before processing the request with UdsServer.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_006`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-007 — Response Reassembly

The UDS CAN transport component shall reassemble the response frames received
from VirtualCanBus before returning the final UDS response.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_007`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-008 — End-to-End Diagnostic Transaction

The UDS CAN transport component shall support the following diagnostic
transaction:

1. Receive a UDS request from the diagnostic client.
2. Convert the request to ISO-TP frames.
3. Transmit the request frames through VirtualCanBus.
4. Receive the request frames from VirtualCanBus.
5. Reassemble the request.
6. Process the request using UdsServer.
7. Convert the UDS response to ISO-TP frames.
8. Transmit the response frames through VirtualCanBus.
9. Receive the response frames from VirtualCanBus.
10. Reassemble the response.
11. Return the final UDS response to the diagnostic client.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_008`  
Verification Status: VERIFIED / PASS

---

# 6. CAN Evidence Requirements

## UDSCAN-REQ-009 — Diagnostic CAN Trace

A completed UDS CAN transaction shall produce VirtualCanBus trace entries for
both diagnostic request and diagnostic response traffic.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_009`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-010 — Diagnostic CAN Timing

Diagnostic CAN frames shall participate in the normal VirtualCanBus
transmission timing model.

The transaction completion time shall occur after its transaction start time.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_010`  
Verification Status: VERIFIED / PASS

---

# 7. Transaction Result Requirements

## UDSCAN-REQ-011 — Transaction Evidence

The UDS CAN transport component shall provide transaction evidence containing:

- Received request CAN frames
- Received response CAN frames
- Final UDS response
- Transaction start time
- Transaction completion time

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_011`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-012 — Positive Response Preservation

A positive UDS response transported through VirtualCanBus shall remain a
positive UDS response after final ISO-TP reassembly.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_012`  
Verification Status: VERIFIED / PASS

---

## UDSCAN-REQ-013 — Negative Response Preservation

A negative UDS response transported through VirtualCanBus shall remain a
negative UDS response after final ISO-TP reassembly.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_013`  
Verification Status: VERIFIED / PASS

---

# 8. Multi-Frame Requirements

## UDSCAN-REQ-014 — Multi-Frame Diagnostic Transport

The UDS CAN transport component shall support diagnostic messages requiring
multiple ISO-TP CAN frames.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_014`  
Verification Status: VERIFIED / PASS

---

# 9. Determinism

## UDSCAN-REQ-015 — Deterministic CAN Diagnostic Transaction

For identical initial bus state, identical UDS server state, identical
transaction start time, and identical UDS request, the UDS CAN transport
component shall produce equivalent diagnostic transaction results.

Verification Method: Automated Test  
Verification Test: `TC_UDSCAN_015`  
Verification Status: VERIFIED / PASS

---

# 10. Verified Architecture

The architecture verified by this requirement set is:

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

The verified component operates as an isolated synchronous diagnostic
transaction.

Runtime arbitration with periodic ECU traffic through SimulationEngine is not
claimed by this requirement set.

---

# 11. Verification Summary

| Requirement Range | Test Range | Result |
|---|---|---|
| `UDSCAN-REQ-001` – `UDSCAN-REQ-015` | `TC_UDSCAN_001` – `TC_UDSCAN_015` | PASS |

- Requirements: **15**
- Verified requirements: **15**
- Unverified requirements: **0**
- Automated tests: **15**
- Passed: **15**
- Failed: **0**

UDS CAN Transport Baseline v1.5 verification status:

**VERIFICATION CLOSED**

---

# 12. Verification Boundary

The following capabilities are not claimed as verified by this requirement
set:

- Runtime integration into SimulationEngine shared ECU traffic
- Concurrent periodic ECU and diagnostic traffic
- Parallel diagnostic clients
- ISO-TP Flow Control scheduling
- ISO-TP Block Size enforcement
- ISO-TP STmin timing enforcement
- Functional diagnostic addressing
- Extended diagnostic addressing
- CAN FD diagnostics
- Physical CAN hardware
- Physical diagnostic hardware
- Full ISO 14229 compliance
- Full ISO 15765-2 compliance
- Hardware-in-the-loop diagnostic transport