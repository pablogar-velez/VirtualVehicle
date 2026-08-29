# VirtualVehicle CAN Message Requirements

**Document ID:** VV-MSG-REQ  
**Baseline:** 1.0  
**Status:** Baseline  

---

## 1. Purpose

This document defines the CAN message-level requirements for the
VirtualVehicle platform.

The requirements describe the identifiers and logical content of messages
exchanged between simulated ECUs.

---

## 2. ABS Wheel State Message

### MSG-ABS-REQ-001 — CAN Identifier

ABS_WHEEL_STATE shall use CAN identifier 0x080.

**Verification Method:** Test

---

### MSG-ABS-REQ-002 — ABS State Information

ABS_WHEEL_STATE shall contain the information required to represent the
defined simulated ABS and wheel state.

**Verification Method:** Test

---

### MSG-ABS-REQ-003 — Encode/Decode Consistency

Encoding and subsequent decoding of ABS_WHEEL_STATE shall preserve the defined
signal values within the resolution supported by the message encoding.

**Verification Method:** Test

---

## 3. Powertrain State Message

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

Encoding and subsequent decoding of POWERTRAIN_STATE shall preserve the
defined signal values within the resolution supported by the message encoding.

**Verification Method:** Test

---

## 4. Steering State Message

### MSG-STR-REQ-001 — CAN Identifier

STEERING_STATE shall use CAN identifier 0x120.

**Verification Method:** Test

---

### MSG-STR-REQ-002 — Steering Angle

STEERING_STATE shall contain steering-angle information.

**Verification Method:** Test

---

### MSG-STR-REQ-003 — Encode/Decode Consistency

Encoding and subsequent decoding of STEERING_STATE shall preserve the defined
signal values within the resolution supported by the message encoding.

**Verification Method:** Test

---

## 5. Verification Status

| Requirement | Implementation | Verification |
|---|---|---|
| MSG-ABS-REQ-001 | Implemented | Unverified |
| MSG-ABS-REQ-002 | Implemented | Unverified |
| MSG-ABS-REQ-003 | Implemented | Unverified |
| MSG-PT-REQ-001 | Implemented | Unverified |
| MSG-PT-REQ-002 | Implemented | Unverified |
| MSG-PT-REQ-003 | Implemented | Unverified |
| MSG-PT-REQ-004 | Implemented | Unverified |
| MSG-PT-REQ-005 | Implemented | Unverified |
| MSG-PT-REQ-006 | Implemented | Unverified |
| MSG-STR-REQ-001 | Implemented | Unverified |
| MSG-STR-REQ-002 | Implemented | Unverified |
| MSG-STR-REQ-003 | Implemented | Unverified |

---

## 6. Signal Definition

Detailed signal layout, byte position, scaling, offset, resolution, and valid
range shall be documented separately from this baseline after verification
against the implemented CAN codecs.