# VirtualVehicle ABS ECU Requirements

**Document ID:** VV-ABS-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

This document defines the functional and verification requirements for the
VirtualVehicle Anti-lock Braking System (ABS) ECU.

The ABS ECU processes wheel-speed sensor information and vehicle braking
conditions to determine ABS intervention and subsystem health.

---

## 2. ABS Functional Requirements

### ABS-REQ-001 — Wheel Slip Processing

The ABS ECU shall process vehicle and wheel-speed information to determine
wheel-slip conditions.

**Verification Method:** Test

---

### ABS-REQ-002 — ABS Activation

The ABS ECU shall activate ABS intervention when wheel slip requiring
intervention is detected while braking.

**Verification Method:** Test
**Verification Test:** TC_ABS_001

---

### ABS-REQ-003 — Healthy State

The ABS ECU shall report HEALTHY status when all required wheel-speed sensor
inputs are valid and no monitored sensor fault is present.

**Verification Method:** Test
**Verification Test:** TC_ABS_004

---


## 3. Sensor Fault Requirements

### ABS-REQ-004 — Front-Left Sensor Dropout Detection

The ABS ECU shall transition to DEGRADED status within 20 ms after a
front-left wheel-speed sensor dropout is detected.

**Verification Method:** Test
**Verification Test:** TC_ABS_002
**Maximum Response Time:** 20 ms

---

### ABS-REQ-005 — Front-Right Sensor Dropout Detection

The ABS ECU shall transition to DEGRADED status within 20 ms after a
front-right wheel-speed sensor dropout is detected.

**Verification Method:** Test
**Verification Test:** TC_ABS_003
**Maximum Response Time:** 20 ms

---


## 4. ABS State Requirements

### ABS-REQ-006 — Intervention State

The ABS ECU shall expose whether ABS intervention is active.

**Verification Method:** Test

---

### ABS-REQ-007 — CAN State Transmission

The ABS ECU shall periodically transmit its state through the Virtual CAN Bus.

**Verification Method:** Test
**Verification Test:** TC_CAN_005

---

### ABS-REQ-008 — ABS CAN Identifier

The ABS ECU shall transmit ABS Wheel State using CAN identifier 0x080.

**Verification Method:** Test

---

## 5. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| ABS-REQ-001 | Implemented | TC_ABS_007 | VERIFIED / PASS |
| ABS-REQ-002 | Implemented | TC_ABS_001 | VERIFIED / PASS |
| ABS-REQ-003 | Implemented | TC_ABS_004 | VERIFIED / PASS |
| ABS-REQ-004 | Implemented | TC_ABS_002 | VERIFIED / PASS |
| ABS-REQ-005 | Implemented | TC_ABS_003 | VERIFIED / PASS |
| ABS-REQ-006 | Implemented | TC_ABS_005 | VERIFIED / PASS |
| ABS-REQ-007 | Implemented | TC_CAN_005 | VERIFIED / PASS |
| ABS-REQ-008 | Implemented | TC_ABS_006 | VERIFIED / PASS |

## 6. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| ABS-REQ-002 | TC_ABS_001 | Verify ABS activation during emergency braking | PASS |
| ABS-REQ-003 | TC_ABS_004 | Verify HEALTHY state with valid wheel-speed inputs and no monitored fault | PASS |
| ABS-REQ-004 | TC_ABS_002 | Verify FL sensor dropout detection within 20 ms | PASS |
| ABS-REQ-005 | TC_ABS_003 | Verify FR sensor dropout detection within 20 ms | PASS |
| ABS-REQ-007 | TC_CAN_005 | Verify periodic ABS CAN transmission requests | PASS |

The 20 ms response-time limit is a VirtualVehicle project-defined design
requirement for Baseline v1.2.

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| ABS-REQ-001 | TC_ABS_007 | VERIFIED / PASS |
| ABS-REQ-002 | TC_ABS_001 | VERIFIED / PASS |
| ABS-REQ-003 | TC_ABS_004 | VERIFIED / PASS |
| ABS-REQ-004 | TC_ABS_002 | VERIFIED / PASS |
| ABS-REQ-005 | TC_ABS_003 | VERIFIED / PASS |
| ABS-REQ-006 | TC_ABS_005 | VERIFIED / PASS |
| ABS-REQ-007 | TC_CAN_005 | VERIFIED / PASS |
| ABS-REQ-008 | TC_ABS_006 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
