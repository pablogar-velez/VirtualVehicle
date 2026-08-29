# VirtualVehicle Vehicle Model Requirements

**Document ID:** VV-VEH-REQ  
**Baseline:** 1.0  
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

---

### VEH-REQ-008 — Hard Braking Behavior

During the Hard Braking driving scenario, the vehicle model shall decrease
vehicle speed while braking is applied.

**Verification Method:** Test

---

## 4. Wheel Dynamics Requirements

### VEH-REQ-009 — Independent Wheel Speeds

The vehicle model shall provide independent simulated front-left and front-right
wheel speeds for ABS processing.

**Verification Method:** Test

---

## 5. Verification Status

| Requirement | Implementation | Verification |
|---|---|---|
| VEH-REQ-001 | Implemented | Unverified |
| VEH-REQ-002 | Implemented | Unverified |
| VEH-REQ-003 | Implemented | Unverified |
| VEH-REQ-004 | Implemented | Unverified |
| VEH-REQ-005 | Implemented | Unverified |
| VEH-REQ-006 | Implemented | Unverified |
| VEH-REQ-007 | Implemented | Unverified |
| VEH-REQ-008 | Implemented | Unverified |
| VEH-REQ-009 | Implemented | Unverified |

---

## 6. Traceability

Vehicle-model verification test cases will be added to the automated validation
suite.

Future test cases may include:

- Vehicle acceleration verification
- Hard braking deceleration verification
- Wheel-speed behavior verification
- Brake-input verification
- Steering-angle verification