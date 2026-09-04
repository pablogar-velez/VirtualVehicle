# VirtualVehicle — ISO-TP Requirements

## Document Information

- Project: VirtualVehicle
- Document: ISO-TP Requirements
- Baseline: v1.5
- Status: Verified / Baseline
- Verification Method: Automated Test unless otherwise specified
- Verification Result: 26 PASS / 0 FAIL

---

# 1. Purpose

This document defines the requirements for the simplified ISO-TP transport
layer within the VirtualVehicle simulation platform.

The ISO-TP subsystem provides segmentation and reassembly of diagnostic
payloads using simulated Classic CAN frames.

The implementation provides the transport layer used between the
VirtualVehicle UDS subsystem and the simulated CAN diagnostic architecture.

This baseline does not claim complete ISO 15765-2 compliance.

---

# 2. Scope

The ISO-TP subsystem includes:

- ISO-TP frame-type identification
- Single Frame generation
- First Frame generation
- Consecutive Frame generation
- Flow Control frame generation
- Payload segmentation
- Payload reassembly
- Consecutive Frame sequence numbering
- Sequence-number rollover
- Payload-length validation
- CAN identifier consistency validation
- Deterministic segmentation
- Deterministic reassembly
- Classic CAN 8-byte frame support

The following functionality is outside the Baseline v1.5 scope:

- CAN FD ISO-TP
- Extended Single Frame length encoding
- Extended First Frame length encoding
- Real-time Flow Control scheduling
- Block-size enforcement during transmission
- STmin timing enforcement
- Physical CAN hardware
- Physical diagnostic interfaces
- Full ISO 15765-2 conformance testing

---

# 3. General Transport Requirements

## ISOTP-REQ-001 — ISO-TP Transport Component

The system shall provide an ISO-TP transport component capable of segmenting
diagnostic payloads into CAN frames and reassembling CAN frames into diagnostic
payloads.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_001`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-002 — Classic CAN Frame Size

The ISO-TP subsystem shall use Classic CAN frames with a maximum payload length
of 8 bytes.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_002`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-003 — Standard CAN Identifier

The initial ISO-TP implementation shall support standard 11-bit CAN identifiers
from `0x000` through `0x7FF`.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_003`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-004 — Maximum ISO-TP Payload

The initial ISO-TP implementation shall support diagnostic payload lengths up
to 4095 bytes.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_004`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-005 — Empty Payload Rejection

The ISO-TP segmentation function shall reject an empty diagnostic payload.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_005`  
Verification Status: VERIFIED / PASS

---

# 4. Single Frame

## ISOTP-REQ-006 — Single Frame Support

The ISO-TP subsystem shall support Single Frame transport.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_006`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-007 — Single Frame Payload Capacity

A diagnostic payload containing between 1 and 7 bytes shall be transported in
one ISO-TP Single Frame.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_007`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-008 — Single Frame PCI

For a Single Frame, the upper nibble of the first CAN data byte shall identify
the frame as a Single Frame.

The lower nibble shall contain the diagnostic payload length.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_008`  
Verification Status: VERIFIED / PASS

---

# 5. First Frame

## ISOTP-REQ-009 — First Frame Support

A diagnostic payload larger than 7 bytes shall begin with an ISO-TP First
Frame.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_009`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-010 — First Frame Payload Length

The First Frame shall encode the total diagnostic payload length using the
12-bit ISO-TP length field.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_010`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-011 — First Frame Data Capacity

The First Frame shall contain the first 6 bytes of a segmented diagnostic
payload.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_011`  
Verification Status: VERIFIED / PASS

---

# 6. Consecutive Frames

## ISOTP-REQ-012 — Consecutive Frame Support

The ISO-TP subsystem shall use Consecutive Frames to transport the remaining
payload after a First Frame.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_012`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-013 — Consecutive Frame Data Capacity

Each Consecutive Frame shall contain up to 7 bytes of diagnostic payload.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_013`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-014 — Initial Sequence Number

The first Consecutive Frame following a First Frame shall use sequence number
`0x1`.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_014`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-015 — Sequence Number Increment

Each subsequent Consecutive Frame shall increment the ISO-TP sequence number by
one.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_015`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-016 — Sequence Number Rollover

After sequence number `0xF`, the next Consecutive Frame sequence number shall
roll over to `0x0`.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_016`  
Verification Status: VERIFIED / PASS

---

# 7. Flow Control

## ISOTP-REQ-017 — Flow Control Frame Support

The ISO-TP subsystem shall provide a representation for ISO-TP Flow Control
frames.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_017`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-018 — Flow Status

A Flow Control frame shall support the following flow-status values:

- Continue To Send
- Wait
- Overflow

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_018`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-019 — Flow Control Parameters

A Flow Control frame shall contain:

- Flow Status
- Block Size
- Separation Time Minimum

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_019`  
Verification Status: VERIFIED / PASS

---

# 8. Reassembly

## ISOTP-REQ-020 — Single Frame Reassembly

The ISO-TP subsystem shall reconstruct the original diagnostic payload from a
valid Single Frame.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_020`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-021 — Multi-Frame Reassembly

The ISO-TP subsystem shall reconstruct the original diagnostic payload from a
valid First Frame followed by the required Consecutive Frames.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_021`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-022 — Sequence Validation

During multi-frame reassembly, the ISO-TP subsystem shall verify Consecutive
Frame sequence numbers.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_022`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-023 — CAN Identifier Validation

All data frames belonging to the same ISO-TP message shall use the same CAN
arbitration identifier.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_023`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-024 — Incomplete Message Detection

The ISO-TP subsystem shall reject an incomplete multi-frame message when the
available Consecutive Frames do not contain the declared diagnostic payload
length.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_024`  
Verification Status: VERIFIED / PASS

---

# 9. Determinism

## ISOTP-REQ-025 — Deterministic Segmentation

For identical CAN identifiers and diagnostic payloads, the ISO-TP subsystem
shall produce identical segmented CAN frames.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_025`  
Verification Status: VERIFIED / PASS

---

## ISOTP-REQ-026 — Deterministic Reassembly

For identical valid ISO-TP CAN-frame sequences, the ISO-TP subsystem shall
produce identical reassembled diagnostic payloads.

Verification Method: Automated Test  
Verification Test: `TC_ISOTP_026`  
Verification Status: VERIFIED / PASS

---

# 10. Verified Architecture

The verified transport architecture includes:

```text
Diagnostic Payload
       |
       v
ISO-TP Segmentation
       |
       v
Classic CAN Frames
       |
       v
ISO-TP Reassembly
       |
       v
Diagnostic Payload
```

The ISO-TP component is also used by the verified UDS Transport and UDS CAN
Transport layers.

---

# 11. Verification Summary

| Requirement Range | Test Range | Result |
|---|---|---|
| `ISOTP-REQ-001` – `ISOTP-REQ-026` | `TC_ISOTP_001` – `TC_ISOTP_026` | PASS |

- Requirements: **26**
- Verified requirements: **26**
- Unverified requirements: **0**
- Automated tests: **26**
- Passed: **26**
- Failed: **0**

ISO-TP Baseline v1.5 verification status:

**VERIFICATION CLOSED**

---

# 12. Verification Boundary

The following capabilities are not claimed as verified by this requirement
set:

- Full ISO 15765-2 compliance
- CAN FD ISO-TP
- Extended Single Frame length encoding
- Extended First Frame length encoding
- Runtime Flow Control scheduling
- Block Size transmission enforcement
- STmin timing enforcement
- Physical CAN hardware
- Physical diagnostic interfaces
- Hardware-in-the-loop transport verification