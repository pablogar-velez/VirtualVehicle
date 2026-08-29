# VirtualVehicle Sensor Requirements

**Document ID:** VV-SNS-REQ  
**Baseline:** 1.0  
**Status:** Baseline  

---

## 1. Purpose

This document defines the requirements for simulated vehicle sensors used by
the VirtualVehicle platform.

Requirements Baseline v1.0 focuses on the wheel-speed sensors used by the ABS
subsystem.

---

## 2. Wheel-Speed Sensor Requirements

### SNS-REQ-001 — Front-Left Wheel-Speed Measurement

The front-left wheel-speed sensor shall provide the simulated front-left wheel
speed to the ABS subsystem.

**Verification Method:** Test

---

### SNS-REQ-002 — Front-Right Wheel-Speed Measurement

The front-right wheel-speed sensor shall provide the simulated front-right
wheel speed to the ABS subsystem.

**Verification Method:** Test

---

## 3. Fault Injection Requirements

### SNS-REQ-003 — Front-Left Dropout Injection

The VirtualVehicle platform shall support injection of a Dropout fault into the
front-left wheel-speed sensor.

**Verification Method:** Test

---

### SNS-REQ-004 — Front-Right Dropout Injection

The VirtualVehicle platform shall support injection of a Dropout fault into the
front-right wheel-speed sensor.

**Verification Method:** Test

---

### SNS-REQ-005 — Dropout Behavior

A wheel-speed sensor configured with a Dropout fault shall produce an invalid
measurement for use by the consuming subsystem.

**Verification Method:** Test

---

### SNS-REQ-006 — Fault Clearing

A wheel-speed sensor shall return to normal measurement behavior after its
injected fault is cleared.

**Verification Method:** Test

---

## 4. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| SNS-REQ-001 | Implemented | Pending | Unverified |
| SNS-REQ-002 | Implemented | Pending | Unverified |
| SNS-REQ-003 | Implemented | Partial coverage through TC_ABS_002 | Unverified |
| SNS-REQ-004 | Implemented | Partial coverage through TC_ABS_003 | Unverified |
| SNS-REQ-005 | Implemented | Partial coverage through ABS tests | Unverified |
| SNS-REQ-006 | Implemented | Pending | Unverified |

---

## 5. Traceability Notes

TC_ABS_002 and TC_ABS_003 currently exercise wheel-speed sensor fault
injection as part of ABS ECU verification.

These tests provide indirect evidence for sensor fault behavior but are not
considered dedicated verification of the sensor requirements.

Dedicated sensor-level tests shall be introduced to provide direct
requirements coverage.

---

## 6. Future Verification

Potential sensor verification tests include:

### TC_SNS_001 — Front-Left Sensor Dropout

Verify that a front-left wheel-speed sensor configured with Dropout produces an
invalid measurement.

Target requirements:

- SNS-REQ-003
- SNS-REQ-005

### TC_SNS_002 — Front-Right Sensor Dropout

Verify that a front-right wheel-speed sensor configured with Dropout produces
an invalid measurement.

Target requirements:

- SNS-REQ-004
- SNS-REQ-005

### TC_SNS_003 — Sensor Fault Recovery

Verify that clearing an injected wheel-speed sensor fault restores normal
sensor measurement behavior.

Target requirement:

- SNS-REQ-006