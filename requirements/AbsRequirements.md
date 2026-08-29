# VirtualVehicle ABS ECU Requirements

**Document ID:** VV-ABS-REQ  
**Baseline:** 1.0  
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

---

### ABS-REQ-008 — ABS CAN Identifier

The ABS ECU shall transmit ABS Wheel State using CAN identifier 0x080.

**Verification Method:** Test

---

## 5. Verification Status

| Requirement | Implementation | Verification Test | Status |
|---|---|---|---|
| ABS-REQ-001 | Implemented | Pending | Unverified |
| ABS-REQ-002 | Implemented | TC_ABS_001 | Verified |
| ABS-REQ-003 | Implemented | Pending | Unverified |
| ABS-REQ-004 | Implemented | TC_ABS_002 | Verified |
| ABS-REQ-005 | Implemented | TC_ABS_003 | Verified |
| ABS-REQ-006 | Implemented | Pending | Unverified |
| ABS-REQ-007 | Implemented | Pending | Unverified |
| ABS-REQ-008 | Implemented | Pending | Unverified |

---

## 6. Traceability

Current requirements-to-test traceability:

| Requirement | Test Case | Verification Objective |
|---|---|---|
| ABS-REQ-002 | TC_ABS_001 | Verify ABS activation during emergency braking |
| ABS-REQ-004 | TC_ABS_002 | Verify FL sensor dropout detection within 20 ms |
| ABS-REQ-005 | TC_ABS_003 | Verify FR sensor dropout detection within 20 ms |

The remaining ABS requirements shall receive dedicated verification coverage
as the automated validation suite is expanded.

---

## 7. Timing Requirements

Timing requirements are evaluated using VirtualVehicle simulation time rather
than wall-clock execution time.

For requirements ABS-REQ-004 and ABS-REQ-005:

Maximum allowed response time = 20 ms

The response time shall be measured from fault injection until the ABS ECU
reports DEGRADED health status.