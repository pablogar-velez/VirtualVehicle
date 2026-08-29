# VirtualVehicle Steering ECU Requirements

**Document ID:** VV-STR-REQ  
**Baseline:** 1.0  
**Status:** Baseline  

---

## 1. Purpose

This document defines the functional requirements for the VirtualVehicle
Steering ECU.

The Steering ECU obtains steering information from the simulated vehicle and
periodically communicates steering state through the Virtual CAN Bus.

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

The Steering ECU shall periodically transmit Steering State through the
Virtual CAN Bus according to its configured transmission period.

**Verification Method:** Test

---

### STR-REQ-004 — Steering CAN Identifier

The Steering ECU shall transmit Steering State using CAN identifier 0x120.

**Verification Method:** Test

---

## 4. Verification Status

| Requirement | Implementation | Verification |
|---|---|---|
| STR-REQ-001 | Implemented | Unverified |
| STR-REQ-002 | Implemented | Unverified |
| STR-REQ-003 | Implemented | Unverified |
| STR-REQ-004 | Implemented | Unverified |

---

## 5. Traceability

Dedicated Steering ECU verification test cases will be added to the automated
validation suite.

Potential verification areas include:

- Steering-state generation
- Steering-angle transmission
- CAN identifier verification
- Periodic transmission verification