# VirtualVehicle Diagnostics and Fault Injection Requirements

**Document ID:** VV-DIA-REQ  
**Baseline:** 1.2  
**Status:** Baseline  

---

## 1. Purpose

Baseline v1.2 diagnostics covers simulated fault injection and ECU health observation. UDS and DTCs are outside this baseline.

---

## 2. Diagnostics and Fault Injection Requirements

### DIA-REQ-001 — Front-Left Sensor Fault Injection

The VirtualVehicle platform shall allow a Dropout fault to be injected into the front-left wheel-speed sensor during simulation execution.

**Verification Method:** Test
**Verification Test:** TC_DIA_001

---

### DIA-REQ-002 — Front-Right Sensor Fault Injection

The VirtualVehicle platform shall allow a Dropout fault to be injected into the front-right wheel-speed sensor during simulation execution.

**Verification Method:** Test
**Verification Test:** TC_DIA_002

---

### DIA-REQ-003 — Front-Left Fault Clearing

The VirtualVehicle platform shall allow an injected front-left wheel-speed sensor fault to be cleared.

**Verification Method:** Test
**Verification Test:** TC_DIA_003

---

### DIA-REQ-004 — Front-Right Fault Clearing

The VirtualVehicle platform shall allow an injected front-right wheel-speed sensor fault to be cleared.

**Verification Method:** Test
**Verification Test:** TC_DIA_004

---

### DIA-REQ-005 — ECU Health Observability

The effect of a supported sensor fault shall be observable through the health state of the affected ECU when the fault is monitored by that ECU.

**Verification Method:** Test
**Verification Test:** TC_DIA_005

---

### DIA-REQ-006 — Runtime Fault Injection

Supported sensor faults shall be injectable without restarting the simulation.

**Verification Method:** Test
**Verification Test:** TC_DIA_006

---

### DIA-REQ-007 — Fault Recovery

Supported injected sensor faults shall be clearable without restarting the simulation.

**Verification Method:** Test
**Verification Test:** TC_DIA_007

---

## 3. Verification Status

| Requirement | Implementation | Verification | Status |
|---|---|---|---|
| DIA-REQ-001 | Implemented | TC_DIA_001 | VERIFIED / PASS |
| DIA-REQ-002 | Implemented | TC_DIA_002 | VERIFIED / PASS |
| DIA-REQ-003 | Implemented | TC_DIA_003 | VERIFIED / PASS |
| DIA-REQ-004 | Implemented | TC_DIA_004 | VERIFIED / PASS |
| DIA-REQ-005 | Implemented | TC_DIA_005 | VERIFIED / PASS |
| DIA-REQ-006 | Implemented | TC_DIA_006 | VERIFIED / PASS |
| DIA-REQ-007 | Implemented | TC_DIA_007 | VERIFIED / PASS |

## 4. Traceability

| Requirement | Test Case | Verification Objective | Result |
|---|---|---|---|
| DIA-REQ-001 | TC_DIA_001 | Verify front-left Dropout injection during execution | PASS |
| DIA-REQ-002 | TC_DIA_002 | Verify front-right Dropout injection during execution | PASS |
| DIA-REQ-003 | TC_DIA_003 | Verify front-left injected fault can be cleared | PASS |
| DIA-REQ-004 | TC_DIA_004 | Verify front-right injected fault can be cleared | PASS |
| DIA-REQ-005 | TC_DIA_005 | Verify sensor-fault effect is observable through ABS ECU health | PASS |
| DIA-REQ-006 | TC_DIA_006 | Verify runtime fault injection without simulation restart | PASS |
| DIA-REQ-007 | TC_DIA_007 | Verify runtime fault recovery without simulation restart | PASS |

Baseline v1.2 does not define UDS services, diagnostic sessions, DTCs,
diagnostic communication over CAN, security access, or ECU reprogramming.

---

## Baseline v1.2 Verification Record

| Requirement | Verification Evidence | Status |
|---|---|---|
| DIA-REQ-001 | TC_DIA_001 | VERIFIED / PASS |
| DIA-REQ-002 | TC_DIA_002 | VERIFIED / PASS |
| DIA-REQ-003 | TC_DIA_003 | VERIFIED / PASS |
| DIA-REQ-004 | TC_DIA_004 | VERIFIED / PASS |
| DIA-REQ-005 | TC_DIA_005 | VERIFIED / PASS |
| DIA-REQ-006 | TC_DIA_006 | VERIFIED / PASS |
| DIA-REQ-007 | TC_DIA_007 | VERIFIED / PASS |

**Latest automated suite result:** 84 PASS / 0 FAIL.
