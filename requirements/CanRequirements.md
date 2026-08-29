# VirtualVehicle CAN Bus Requirements

**Document ID:** VV-CAN-REQ  
**Baseline:** 1.0  
**Status:** Baseline  

---

## 1. Purpose

This document defines the functional and timing requirements for the
VirtualVehicle CAN Bus simulation.

The Virtual CAN Bus provides communication between simulated ECUs and models
CAN transmission, arbitration, timing, tracing, and statistics.

---

## 2. CAN Configuration Requirements

### CAN-REQ-001 — Nominal Bitrate

The Virtual CAN Bus shall operate at a configured nominal bitrate of
500 kbit/s.

**Verification Method:** Test

---

### CAN-REQ-002 — Multiple ECU Transmission

The Virtual CAN Bus shall accept transmission requests from multiple simulated
ECUs.

**Verification Method:** Test

---

## 3. CAN Arbitration Requirements

### CAN-REQ-003 — Identifier Priority

When multiple CAN frames contend for transmission, the frame with the
numerically lowest CAN identifier shall receive the highest arbitration
priority.

**Verification Method:** Test

---

### CAN-REQ-004 — Exclusive Bus Access

Only one CAN frame shall occupy the Virtual CAN Bus at a given simulation time.

**Verification Method:** Test

---

### CAN-REQ-005 — Pending Frame Behavior

A CAN frame waiting for bus access shall remain pending until it is selected
for transmission.

**Verification Method:** Test

---

## 4. CAN Timing Requirements

### CAN-REQ-006 — Transmission Duration

The Virtual CAN Bus shall calculate CAN frame transmission duration according
to the configured CAN timing model and nominal bitrate.

**Verification Method:** Test / Analysis

---

### CAN-REQ-007 — Waiting Time

The Virtual CAN Bus shall calculate the waiting time experienced by a CAN frame
before transmission.

**Verification Method:** Test / Analysis

---

## 5. CAN Trace Requirements

### CAN-REQ-008 — Transmission Trace

The Virtual CAN Bus shall record successfully transmitted CAN frames in the
CAN trace.

**Verification Method:** Test

---

### CAN-REQ-009 — Transmission Timestamp

Each CAN trace entry shall provide the simulation time associated with the
frame transmission.

**Verification Method:** Test

---

### CAN-REQ-010 — CAN Identifier

Each CAN trace entry shall identify the CAN identifier of the transmitted
frame.

**Verification Method:** Test

---

### CAN-REQ-011 — Payload Data

Each CAN trace entry shall provide the transmitted CAN payload data.

**Verification Method:** Test

---

### CAN-REQ-012 — Waiting-Time Evidence

Each CAN trace entry shall provide the calculated frame waiting time when
applicable.

**Verification Method:** Test

---

## 6. CAN Statistics Requirements

### CAN-REQ-013 — Frame Count

The CAN subsystem shall maintain the total number of transmitted CAN frames.

**Verification Method:** Test

---

### CAN-REQ-014 — Arbitration Count

The CAN subsystem shall maintain the number of CAN arbitration occurrences.

**Verification Method:** Test

---

### CAN-REQ-015 — Average Waiting Time

The CAN subsystem shall calculate the average CAN frame waiting time.

**Verification Method:** Analysis

---

### CAN-REQ-016 — Maximum Waiting Time

The CAN subsystem shall calculate the maximum CAN frame waiting time.

**Verification Method:** Analysis

---

### CAN-REQ-017 — Transmission Time

The CAN subsystem shall maintain the accumulated CAN frame transmission time.

**Verification Method:** Analysis

---

### CAN-REQ-018 — Bus Utilization

The CAN subsystem shall calculate bus utilization using accumulated
transmission time and simulation time.

**Verification Method:** Analysis

---

### CAN-REQ-019 — Per-Message Statistics

The CAN subsystem shall maintain transmission statistics associated with
individual CAN message identifiers.

**Verification Method:** Test / Analysis

---

## 7. CAN Priority Baseline

The current VirtualVehicle CAN message identifiers establish the following
arbitration priority:

| Priority | CAN ID | Message |
|---|---|---|
| Highest | 0x080 | ABS_WHEEL_STATE |
| Medium | 0x100 | POWERTRAIN_STATE |
| Lowest | 0x120 | STEERING_STATE |

CAN arbitration priority is determined by CAN identifier and not by ECU type.

---

## 8. Verification Status

| Requirement | Implementation | Verification |
|---|---|---|
| CAN-REQ-001 | Implemented | Unverified |
| CAN-REQ-002 | Implemented | Unverified |
| CAN-REQ-003 | Implemented | Unverified |
| CAN-REQ-004 | Implemented | Unverified |
| CAN-REQ-005 | Implemented | Unverified |
| CAN-REQ-006 | Implemented | Unverified |
| CAN-REQ-007 | Implemented | Unverified |
| CAN-REQ-008 | Implemented | Unverified |
| CAN-REQ-009 | Implemented | Unverified |
| CAN-REQ-010 | Implemented | Unverified |
| CAN-REQ-011 | Implemented | Unverified |
| CAN-REQ-012 | Implemented | Unverified |
| CAN-REQ-013 | Implemented | Unverified |
| CAN-REQ-014 | Implemented | Unverified |
| CAN-REQ-015 | Implemented | Unverified |
| CAN-REQ-016 | Implemented | Unverified |
| CAN-REQ-017 | Implemented | Unverified |
| CAN-REQ-018 | Implemented | Unverified |
| CAN-REQ-019 | Implemented | Unverified |

---

## 9. Traceability

Dedicated CAN verification tests will be introduced into the automated
validation suite.

Planned verification areas include:

- CAN arbitration priority
- CAN bitrate configuration
- Frame transmission timing
- Waiting-time calculation
- CAN trace generation
- Frame-count statistics
- Arbitration statistics
- Bus-utilization calculation
- Per-message statistics