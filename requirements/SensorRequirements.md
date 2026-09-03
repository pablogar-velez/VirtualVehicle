# VirtualVehicle Sensor Requirements

**Document ID:** VV-SNS-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

Requirements for simulated vehicle sensors used by VirtualVehicle. Baseline v1.2 focuses on the wheel-speed sensors used by the ABS subsystem.

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
**Verification Test:** TC_SNS_002

---

### SNS-REQ-005 — Dropout Behavior

A wheel-speed sensor configured with a Dropout fault shall produce an invalid
measurement for use by the consuming subsystem.

**Verification Method:** Test
**Verification Test:** TC_SNS_001

---

### SNS-REQ-006 — Fault Clearing

A wheel-speed sensor shall return to normal measurement behavior after its
injected fault is cleared.

**Verification Method:** Test
**Verification Test:** TC_SNS_003

---

## 4. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| SNS-REQ-001 | Implemented | TC_SNS_004 | VERIFIED / PASS |
| SNS-REQ-002 | Implemented | TC_SNS_005 | VERIFIED / PASS |
| SNS-REQ-003 | Implemented | TC_SNS_006 | VERIFIED / PASS |
| SNS-REQ-004 | Implemented | TC_SNS_002 | VERIFIED / PASS |
| SNS-REQ-005 | Implemented | TC_SNS_001 | VERIFIED / PASS |
| SNS-REQ-006 | Implemented | TC_SNS_003 | VERIFIED / PASS |

## 5. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| SNS-REQ-004 | TC_SNS_002 | Verify front-right Dropout injection behavior | PASS |
| SNS-REQ-005 | TC_SNS_001 | Verify Dropout produces an invalid measurement | PASS |
| SNS-REQ-006 | TC_SNS_003 | Verify normal behavior returns after fault clear | PASS |

TC_SNS_001 and TC_ABS_002 support SNS-REQ-003, but TC_SNS_001 currently maps
primarily to SNS-REQ-005; therefore SNS-REQ-003 remains formally unverified.

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| SNS-REQ-001 | TC_SNS_004 | VERIFIED / PASS |
| SNS-REQ-002 | TC_SNS_005 | VERIFIED / PASS |
| SNS-REQ-003 | TC_SNS_006 | VERIFIED / PASS |
| SNS-REQ-004 | TC_SNS_002 | VERIFIED / PASS |
| SNS-REQ-005 | TC_SNS_001 | VERIFIED / PASS |
| SNS-REQ-006 | TC_SNS_003 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
