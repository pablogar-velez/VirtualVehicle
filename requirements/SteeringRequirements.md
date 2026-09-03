# VirtualVehicle Steering ECU Requirements

**Document ID:** VV-STR-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

The Steering ECU obtains steering information from the simulated vehicle and periodically communicates steering state through the Virtual CAN Bus.

---

## 2. Steering State Requirements

### STR-REQ-001 — Steering State

The Steering ECU shall provide the simulated vehicle steering state.

**Verification Method:** Test

---

### STR-REQ-002 — Steering Angle

The Steering state shall include steering-angle information.

**Verification Method:** Test

---


## 3. CAN Communication Requirements

### STR-REQ-003 — Periodic Transmission

The Steering ECU shall periodically transmit Steering State through the Virtual
CAN Bus according to its configured transmission period.

**Verification Method:** Test
**Verification Test:** TC_CAN_007

---

### STR-REQ-004 — Steering CAN Identifier

The Steering ECU shall transmit Steering State using CAN identifier 0x120.

**Verification Method:** Test
**Verification Test:** TC_STR_001

---

## 4. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| STR-REQ-001 | Implemented | TC_STR_003 | VERIFIED / PASS |
| STR-REQ-002 | Implemented | TC_STR_004 | VERIFIED / PASS |
| STR-REQ-003 | Implemented | TC_CAN_007 | VERIFIED / PASS |
| STR-REQ-004 | Implemented | TC_STR_001 | VERIFIED / PASS |

## 5. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| STR-REQ-003 | TC_CAN_007 | Verify configured Steering transmission period | PASS |
| STR-REQ-004 | TC_STR_001 | Verify Steering CAN identifier is 0x120 | PASS |

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| STR-REQ-001 | TC_STR_003 | VERIFIED / PASS |
| STR-REQ-002 | TC_STR_004 | VERIFIED / PASS |
| STR-REQ-003 | TC_CAN_007 | VERIFIED / PASS |
| STR-REQ-004 | TC_STR_001 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
