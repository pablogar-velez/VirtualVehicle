# VirtualVehicle CAN Bus Requirements

**Document ID:** VV-CAN-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

The Virtual CAN Bus provides communication between simulated ECUs and models CAN transmission, arbitration, timing, tracing, and statistics.

---

## 2. CAN Requirements

### CAN-REQ-001 — Nominal Bitrate

The Virtual CAN Bus shall operate at a configured nominal bitrate of 500 kbit/s.

**Verification Method:** Test
**Verification Test:** TC_CAN_002

---

### CAN-REQ-002 — Multiple ECU Transmission

The Virtual CAN Bus shall accept transmission requests from multiple simulated ECUs.

**Verification Method:** Test

---

### CAN-REQ-003 — Identifier Priority

When multiple CAN frames contend for transmission, the frame with the numerically lowest CAN identifier shall receive the highest arbitration priority.

**Verification Method:** Test
**Verification Test:** TC_CAN_001

---

### CAN-REQ-004 — Exclusive Bus Access

Only one CAN frame shall occupy the Virtual CAN Bus at a given simulation time.

**Verification Method:** Test
**Verification Test:** TC_CAN_019

---

### CAN-REQ-005 — Pending Frame Behavior

A CAN frame waiting for bus access shall remain pending until it is selected for transmission.

**Verification Method:** Test
**Verification Test:** TC_CAN_017

---

### CAN-REQ-006 — Transmission Duration

The Virtual CAN Bus shall calculate CAN frame transmission duration according to the configured CAN timing model and nominal bitrate.

**Verification Method:** Test / Analysis
**Verification Test:** TC_CAN_010

---

### CAN-REQ-007 — Waiting Time

The Virtual CAN Bus shall calculate the waiting time experienced by a CAN frame before transmission.

**Verification Method:** Test / Analysis
**Verification Test:** TC_CAN_011

---

### CAN-REQ-008 — Transmission Trace

The Virtual CAN Bus shall record successfully transmitted CAN frames in the CAN trace.

**Verification Method:** Test
**Verification Test:** TC_CAN_003

---

### CAN-REQ-009 — Transmission Timestamp

Each CAN trace entry shall provide the simulation time associated with the frame transmission.

**Verification Method:** Test
**Verification Test:** TC_CAN_009

---

### CAN-REQ-010 — CAN Identifier

Each CAN trace entry shall identify the CAN identifier of the transmitted frame.

**Verification Method:** Test
**Verification Test:** TC_CAN_020

---

### CAN-REQ-011 — Payload Data

Each CAN trace entry shall provide the transmitted CAN payload data.

**Verification Method:** Test
**Verification Test:** TC_CAN_021

---

### CAN-REQ-012 — Waiting-Time Evidence

Each CAN trace entry shall provide the calculated frame waiting time when applicable.

**Verification Method:** Test
**Verification Test:** TC_CAN_018

---

### CAN-REQ-013 — Frame Count

The CAN subsystem shall maintain the total number of transmitted CAN frames.

**Verification Method:** Test
**Verification Test:** TC_CAN_012

---

### CAN-REQ-014 — Arbitration Count

The CAN subsystem shall maintain the number of CAN arbitration occurrences.

**Verification Method:** Test
**Verification Test:** TC_CAN_004

---

### CAN-REQ-015 — Average Waiting Time

The CAN subsystem shall calculate the average CAN frame waiting time.

**Verification Method:** Analysis
**Verification Test:** TC_CAN_013

---

### CAN-REQ-016 — Maximum Waiting Time

The CAN subsystem shall calculate the maximum CAN frame waiting time.

**Verification Method:** Analysis
**Verification Test:** TC_CAN_014

---

### CAN-REQ-017 — Transmission Time

The CAN subsystem shall maintain the accumulated CAN frame transmission time.

**Verification Method:** Analysis
**Verification Test:** TC_CAN_015

---

### CAN-REQ-018 — Bus Utilization

The CAN subsystem shall calculate bus utilization using accumulated transmission time and simulation time.

**Verification Method:** Analysis
**Verification Test:** TC_CAN_008

---

### CAN-REQ-019 — Per-Message Statistics

The CAN subsystem shall maintain transmission statistics associated with individual CAN message identifiers.

**Verification Method:** Test / Analysis
**Verification Test:** TC_CAN_016

---

## 3. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| CAN-REQ-001 | Implemented | TC_CAN_002 | VERIFIED / PASS |
| CAN-REQ-002 | Implemented | TC_CAN_022 | VERIFIED / PASS |
| CAN-REQ-003 | Implemented | TC_CAN_001 | VERIFIED / PASS |
| CAN-REQ-004 | Implemented | TC_CAN_019 | VERIFIED / PASS |
| CAN-REQ-005 | Implemented | TC_CAN_017 | VERIFIED / PASS |
| CAN-REQ-006 | Implemented | TC_CAN_010 | VERIFIED / PASS |
| CAN-REQ-007 | Implemented | TC_CAN_011 | VERIFIED / PASS |
| CAN-REQ-008 | Implemented | TC_CAN_003 | VERIFIED / PASS |
| CAN-REQ-009 | Implemented | TC_CAN_009 | VERIFIED / PASS |
| CAN-REQ-010 | Implemented | TC_CAN_020 | VERIFIED / PASS |
| CAN-REQ-011 | Implemented | TC_CAN_021 | VERIFIED / PASS |
| CAN-REQ-012 | Implemented | TC_CAN_018 | VERIFIED / PASS |
| CAN-REQ-013 | Implemented | TC_CAN_012 | VERIFIED / PASS |
| CAN-REQ-014 | Implemented | TC_CAN_004 | VERIFIED / PASS |
| CAN-REQ-015 | Implemented | TC_CAN_013 | VERIFIED / PASS |
| CAN-REQ-016 | Implemented | TC_CAN_014 | VERIFIED / PASS |
| CAN-REQ-017 | Implemented | TC_CAN_015 | VERIFIED / PASS |
| CAN-REQ-018 | Implemented | TC_CAN_008 | VERIFIED / PASS |
| CAN-REQ-019 | Implemented | TC_CAN_016 | VERIFIED / PASS |

## 4. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| CAN-REQ-001 | TC_CAN_002 | Verify configured bitrate is 500 kbit/s | PASS |
| CAN-REQ-003 | TC_CAN_001 | Verify lowest numeric CAN ID wins arbitration | PASS |
| CAN-REQ-004 | TC_CAN_019 | Verify CAN transmissions do not overlap on the bus | PASS |
| CAN-REQ-005 | TC_CAN_017 | Verify a non-winning frame remains pending until later transmission | PASS |
| CAN-REQ-006 | TC_CAN_010 | Verify transmission duration matches the configured timing model and bitrate | PASS |
| CAN-REQ-007 | TC_CAN_011 | Verify waiting time equals transmission start minus request time | PASS |
| CAN-REQ-008 | TC_CAN_003 | Verify transmitted frame is recorded in trace | PASS |
| CAN-REQ-009 | TC_CAN_009 | Verify trace timing evidence is recorded | PASS |
| CAN-REQ-010 | TC_CAN_020 | Verify trace records the transmitted CAN identifier | PASS |
| CAN-REQ-011 | TC_CAN_021 | Verify trace records DLC and payload data | PASS |
| CAN-REQ-012 | TC_CAN_018 | Verify calculated waiting time is preserved in the CAN trace | PASS |
| CAN-REQ-013 | TC_CAN_012 | Verify total transmitted frame count is maintained | PASS |
| CAN-REQ-014 | TC_CAN_004 | Verify arbitration count is maintained | PASS |
| CAN-REQ-015 | TC_CAN_013 | Verify average waiting time calculation | PASS |
| CAN-REQ-016 | TC_CAN_014 | Verify maximum waiting time calculation | PASS |
| CAN-REQ-017 | TC_CAN_015 | Verify accumulated transmission time calculation | PASS |
| CAN-REQ-018 | TC_CAN_008 | Verify bus utilization calculation | PASS |
| CAN-REQ-019 | TC_CAN_016 | Verify per-message statistics are maintained by CAN identifier | PASS |

CAN-REQ-002 remains implemented with supporting multi-frame arbitration evidence,
but does not yet have a dedicated primary verification test.

Current arbitration priority baseline: 0x080 ABS_WHEEL_STATE, 0x100
POWERTRAIN_STATE, 0x120 STEERING_STATE.

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| CAN-REQ-001 | TC_CAN_002 | VERIFIED / PASS |
| CAN-REQ-002 | TC_CAN_022 | VERIFIED / PASS |
| CAN-REQ-003 | TC_CAN_001 | VERIFIED / PASS |
| CAN-REQ-004 | TC_CAN_019 | VERIFIED / PASS |
| CAN-REQ-005 | TC_CAN_017 | VERIFIED / PASS |
| CAN-REQ-006 | TC_CAN_010 | VERIFIED / PASS |
| CAN-REQ-007 | TC_CAN_011 | VERIFIED / PASS |
| CAN-REQ-008 | TC_CAN_003 | VERIFIED / PASS |
| CAN-REQ-009 | TC_CAN_009 | VERIFIED / PASS |
| CAN-REQ-010 | TC_CAN_020 | VERIFIED / PASS |
| CAN-REQ-011 | TC_CAN_021 | VERIFIED / PASS |
| CAN-REQ-012 | TC_CAN_018 | VERIFIED / PASS |
| CAN-REQ-013 | TC_CAN_012 | VERIFIED / PASS |
| CAN-REQ-014 | TC_CAN_004 | VERIFIED / PASS |
| CAN-REQ-015 | TC_CAN_013 | VERIFIED / PASS |
| CAN-REQ-016 | TC_CAN_014 | VERIFIED / PASS |
| CAN-REQ-017 | TC_CAN_015 | VERIFIED / PASS |
| CAN-REQ-018 | TC_CAN_008 | VERIFIED / PASS |
| CAN-REQ-019 | TC_CAN_016 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
