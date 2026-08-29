# VirtualVehicle Powertrain ECU Requirements

**Document ID:** VV-PT-REQ  
**Baseline:** 1.0  
**Status:** Baseline  

---

## 1. Purpose

This document defines the functional requirements for the VirtualVehicle
Powertrain ECU.

The Powertrain ECU provides vehicle powertrain state information and
periodically communicates that information through the Virtual CAN Bus.

---

## 2. Powertrain State Requirements

### PT-REQ-001 — Powertrain State

The Powertrain ECU shall provide the simulated vehicle powertrain state.

**Verification Method:** Test

---

### PT-REQ-002 — Vehicle Speed

The Powertrain state shall include vehicle speed information.

**Verification Method:** Test

---

### PT-REQ-003 — Engine Speed

The Powertrain state shall include engine RPM information.

**Verification Method:** Test

---

### PT-REQ-004 — Gear State

The Powertrain state shall include vehicle gear information.

**Verification Method:** Test

---

### PT-REQ-005 — Brake State

The Powertrain state shall include vehicle brake state information.

**Verification Method:** Test

---

## 3. CAN Communication Requirements

### PT-REQ-006 — Periodic Transmission

The Powertrain ECU shall periodically transmit Powertrain State through the
Virtual CAN Bus according to its configured transmission period.

**Verification Method:** Test

---

### PT-REQ-007 — Powertrain CAN Identifier

The Powertrain ECU shall transmit Powertrain State using CAN identifier 0x100.

**Verification Method:** Test

---

## 4. Verification Status

| Requirement | Implementation | Verification |
|---|---|---|
| PT-REQ-001 | Implemented | Unverified |
| PT-REQ-002 | Implemented | Unverified |
| PT-REQ-003 | Implemented | Unverified |
| PT-REQ-004 | Implemented | Unverified |
| PT-REQ-005 | Implemented | Unverified |
| PT-REQ-006 | Implemented | Unverified |
| PT-REQ-007 | Implemented | Unverified |

---

## 5. Traceability

Dedicated Powertrain ECU verification test cases will be added to the automated
validation suite.

Potential verification areas include:

- Powertrain state generation
- Vehicle-speed transmission
- Engine-RPM transmission
- Gear-state transmission
- Brake-state transmission
- CAN identifier verification
- Periodic transmission verification