# VirtualVehicle Powertrain ECU Requirements

**Document ID:** VV-PT-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

The Powertrain ECU provides vehicle powertrain state information and periodically communicates that information through the Virtual CAN Bus.

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
**Verification Test:** TC_CAN_006

---

### PT-REQ-007 — Powertrain CAN Identifier

The Powertrain ECU shall transmit Powertrain State using CAN identifier 0x100.

**Verification Method:** Test
**Verification Test:** TC_PT_001

---

## 4. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| PT-REQ-001 | Implemented | TC_PT_003 | VERIFIED / PASS |
| PT-REQ-002 | Implemented | TC_PT_004 | VERIFIED / PASS |
| PT-REQ-003 | Implemented | TC_PT_005 | VERIFIED / PASS |
| PT-REQ-004 | Implemented | TC_PT_006 | VERIFIED / PASS |
| PT-REQ-005 | Implemented | TC_PT_007 | VERIFIED / PASS |
| PT-REQ-006 | Implemented | TC_CAN_006 | VERIFIED / PASS |
| PT-REQ-007 | Implemented | TC_PT_001 | VERIFIED / PASS |

## 5. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| PT-REQ-006 | TC_CAN_006 | Verify configured Powertrain transmission period | PASS |
| PT-REQ-007 | TC_PT_001 | Verify Powertrain CAN identifier is 0x100 | PASS |

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| PT-REQ-001 | TC_PT_003 | VERIFIED / PASS |
| PT-REQ-002 | TC_PT_004 | VERIFIED / PASS |
| PT-REQ-003 | TC_PT_005 | VERIFIED / PASS |
| PT-REQ-004 | TC_PT_006 | VERIFIED / PASS |
| PT-REQ-005 | TC_PT_007 | VERIFIED / PASS |
| PT-REQ-006 | TC_CAN_006 | VERIFIED / PASS |
| PT-REQ-007 | TC_PT_001 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
