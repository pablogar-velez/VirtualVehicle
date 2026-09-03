# VirtualVehicle Vehicle Model Requirements

**Document ID:** VV-VEH-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

This document defines the requirements for the VirtualVehicle simulated vehicle
model.

The vehicle model provides the physical state used by the simulated ECUs,
sensors, scenarios, and validation environment.

---

## 2. Vehicle State Requirements

### VEH-REQ-001 — Vehicle Speed

The vehicle model shall provide simulated vehicle speed.

**Verification Method:** Test

---

### VEH-REQ-002 — Front-Left Wheel Speed

The vehicle model shall provide simulated front-left wheel speed.

**Verification Method:** Test

---

### VEH-REQ-003 — Front-Right Wheel Speed

The vehicle model shall provide simulated front-right wheel speed.

**Verification Method:** Test

---

### VEH-REQ-004 — Brake Input

The vehicle model shall provide simulated brake input as a percentage.

**Verification Method:** Test

---

### VEH-REQ-005 — Steering Angle

The vehicle model shall provide simulated steering angle.

**Verification Method:** Test

---


## 3. Vehicle Dynamics Requirements

### VEH-REQ-006 — Dynamic State Update

The vehicle model shall update its dynamic state as simulation time advances.

**Verification Method:** Test

---

### VEH-REQ-007 — Acceleration Behavior

During the Acceleration driving scenario, the vehicle model shall increase
vehicle speed from its initial condition.

**Verification Method:** Test
**Verification Test:** TC_VEH_001

---

### VEH-REQ-008 — Hard Braking Behavior

During the Hard Braking driving scenario, the vehicle model shall decrease
vehicle speed while braking is applied.

**Verification Method:** Test
**Verification Test:** TC_VEH_003

---


## 4. Wheel Dynamics Requirements

### VEH-REQ-009 — Independent Wheel Speeds

The vehicle model shall provide independent simulated front-left and front-right
wheel speeds for ABS processing.

**Verification Method:** Test

---

## 5. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| VEH-REQ-001 | Implemented | TC_VEH_005 | VERIFIED / PASS |
| VEH-REQ-002 | Implemented | TC_VEH_006 | VERIFIED / PASS |
| VEH-REQ-003 | Implemented | TC_VEH_007 | VERIFIED / PASS |
| VEH-REQ-004 | Implemented | TC_VEH_008 | VERIFIED / PASS |
| VEH-REQ-005 | Implemented | TC_VEH_009 | VERIFIED / PASS |
| VEH-REQ-006 | Implemented | TC_VEH_010 | VERIFIED / PASS |
| VEH-REQ-007 | Implemented | TC_VEH_001 | VERIFIED / PASS |
| VEH-REQ-008 | Implemented | TC_VEH_003 | VERIFIED / PASS |
| VEH-REQ-009 | Implemented | TC_VEH_011 | VERIFIED / PASS |

## 6. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| VEH-REQ-007 | TC_VEH_001 | Verify speed increases during Acceleration | PASS |
| VEH-REQ-008 | TC_VEH_003 | Verify speed decreases while braking during Hard Braking | PASS |

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| VEH-REQ-001 | TC_VEH_005 | VERIFIED / PASS |
| VEH-REQ-002 | TC_VEH_006 | VERIFIED / PASS |
| VEH-REQ-003 | TC_VEH_007 | VERIFIED / PASS |
| VEH-REQ-004 | TC_VEH_008 | VERIFIED / PASS |
| VEH-REQ-005 | TC_VEH_009 | VERIFIED / PASS |
| VEH-REQ-006 | TC_VEH_010 | VERIFIED / PASS |
| VEH-REQ-007 | TC_VEH_001 | VERIFIED / PASS |
| VEH-REQ-008 | TC_VEH_003 | VERIFIED / PASS |
| VEH-REQ-009 | TC_VEH_011 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
