# VirtualVehicle CAN Message Requirements

**Document ID:** VV-MSG-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

The requirements describe the identifiers and logical content of CAN messages exchanged between simulated ECUs.

---

## 2. CAN Message Requirements

### MSG-ABS-REQ-001 — CAN Identifier

ABS_WHEEL_STATE shall use CAN identifier 0x080.

**Verification Method:** Test

---

### MSG-ABS-REQ-002 — ABS State Information

ABS_WHEEL_STATE shall contain the information required to represent the defined simulated ABS and wheel state.

**Verification Method:** Test

---

### MSG-ABS-REQ-003 — Encode/Decode Consistency

Encoding and subsequent decoding of ABS_WHEEL_STATE shall preserve the defined signal values within the resolution supported by the message encoding.

**Verification Method:** Test
**Verification Test:** TC_MSG_001

---

### MSG-PT-REQ-001 — CAN Identifier

POWERTRAIN_STATE shall use CAN identifier 0x100.

**Verification Method:** Test

---

### MSG-PT-REQ-002 — Vehicle Speed

POWERTRAIN_STATE shall contain vehicle-speed information.

**Verification Method:** Test

---

### MSG-PT-REQ-003 — Engine RPM

POWERTRAIN_STATE shall contain engine-RPM information.

**Verification Method:** Test

---

### MSG-PT-REQ-004 — Gear State

POWERTRAIN_STATE shall contain gear-state information.

**Verification Method:** Test

---

### MSG-PT-REQ-005 — Brake State

POWERTRAIN_STATE shall contain brake-state information.

**Verification Method:** Test

---

### MSG-PT-REQ-006 — Encode/Decode Consistency

Encoding and subsequent decoding of POWERTRAIN_STATE shall preserve the defined signal values within the resolution supported by the message encoding.

**Verification Method:** Test
**Verification Test:** TC_PT_002

---

### MSG-STR-REQ-001 — CAN Identifier

STEERING_STATE shall use CAN identifier 0x120.

**Verification Method:** Test

---

### MSG-STR-REQ-002 — Steering Angle

STEERING_STATE shall contain steering-angle information.

**Verification Method:** Test

---

### MSG-STR-REQ-003 — Encode/Decode Consistency

Encoding and subsequent decoding of STEERING_STATE shall preserve the defined signal values within the resolution supported by the message encoding.

**Verification Method:** Test
**Verification Test:** TC_STR_002

---

## 3. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| MSG-ABS-REQ-001 | Implemented | TC_MSG_002 | VERIFIED / PASS |
| MSG-ABS-REQ-002 | Implemented | TC_MSG_003 | VERIFIED / PASS |
| MSG-ABS-REQ-003 | Implemented | TC_MSG_001 | VERIFIED / PASS |
| MSG-PT-REQ-001 | Implemented | TC_MSG_004 | VERIFIED / PASS |
| MSG-PT-REQ-002 | Implemented | TC_MSG_005 | VERIFIED / PASS |
| MSG-PT-REQ-003 | Implemented | TC_MSG_006 | VERIFIED / PASS |
| MSG-PT-REQ-004 | Implemented | TC_MSG_007 | VERIFIED / PASS |
| MSG-PT-REQ-005 | Implemented | TC_MSG_008 | VERIFIED / PASS |
| MSG-PT-REQ-006 | Implemented | TC_PT_002 | VERIFIED / PASS |
| MSG-STR-REQ-001 | Implemented | TC_MSG_009 | VERIFIED / PASS |
| MSG-STR-REQ-002 | Implemented | TC_MSG_010 | VERIFIED / PASS |
| MSG-STR-REQ-003 | Implemented | TC_STR_002 | VERIFIED / PASS |

## 4. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| MSG-ABS-REQ-003 | TC_MSG_001 | Verify ABS codec roundtrip consistency | PASS |
| MSG-PT-REQ-006 | TC_PT_002 | Verify Powertrain codec roundtrip consistency | PASS |
| MSG-STR-REQ-003 | TC_STR_002 | Verify Steering codec roundtrip consistency | PASS |

Detailed byte layout and scaling remain separate from this baseline.

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| MSG-ABS-REQ-001 | TC_MSG_002 | VERIFIED / PASS |
| MSG-ABS-REQ-002 | TC_MSG_003 | VERIFIED / PASS |
| MSG-ABS-REQ-003 | TC_MSG_001 | VERIFIED / PASS |
| MSG-PT-REQ-001 | TC_MSG_004 | VERIFIED / PASS |
| MSG-PT-REQ-002 | TC_MSG_005 | VERIFIED / PASS |
| MSG-PT-REQ-003 | TC_MSG_006 | VERIFIED / PASS |
| MSG-PT-REQ-004 | TC_MSG_007 | VERIFIED / PASS |
| MSG-PT-REQ-005 | TC_MSG_008 | VERIFIED / PASS |
| MSG-PT-REQ-006 | TC_PT_002 | VERIFIED / PASS |
| MSG-STR-REQ-001 | TC_MSG_009 | VERIFIED / PASS |
| MSG-STR-REQ-002 | TC_MSG_010 | VERIFIED / PASS |
| MSG-STR-REQ-003 | TC_STR_002 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
