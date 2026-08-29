# VirtualVehicle Diagnostics and Fault Injection Requirements

**Document ID:** VV-DIA-REQ  
**Baseline:** 1.0  
**Status:** Baseline  

---

## 1. Purpose

This document defines the fault-injection and diagnostic-observability
requirements for the VirtualVehicle platform.

The term diagnostics in Requirements Baseline v1.0 refers to simulated fault
injection and ECU health observation.

UDS diagnostic services and Diagnostic Trouble Codes are not part of this
baseline.

---

## 2. Fault Injection Requirements

### DIA-REQ-001 — Front-Left Sensor Fault Injection

The VirtualVehicle platform shall allow a Dropout fault to be injected into the
front-left wheel-speed sensor during simulation execution.

**Verification Method:** Test

---

### DIA-REQ-002 — Front-Right Sensor Fault Injection

The VirtualVehicle platform shall allow a Dropout fault to be injected into the
front-right wheel-speed sensor during simulation execution.

**Verification Method:** Test

---

### DIA-REQ-003 — Front-Left Fault Clearing

The VirtualVehicle platform shall allow an injected front-left wheel-speed
sensor fault to be cleared.

**Verification Method:** Test

---

### DIA-REQ-004 — Front-Right Fault Clearing

The VirtualVehicle platform shall allow an injected front-right wheel-speed
sensor fault to be cleared.

**Verification Method:** Test

---

## 3. Diagnostic Observability Requirements

### DIA-REQ-005 — ECU Health Observability

The effect of a supported sensor fault shall be observable through the health
state of the affected ECU when the fault is monitored by that ECU.

**Verification Method:** Test

---

### DIA-REQ-006 — Runtime Fault Injection

Supported sensor faults shall be injectable without restarting the simulation.

**Verification Method:** Test

---

### DIA-REQ-007 — Fault Recovery

Supported injected sensor faults shall be clearable without restarting the
simulation.

**Verification Method:** Test

---

## 4. Verification Status

| Requirement | Implementation | Verification |
|---|---|---|
| DIA-REQ-001 | Implemented | Unverified |
| DIA-REQ-002 | Implemented | Unverified |
| DIA-REQ-003 | Implemented | Unverified |
| DIA-REQ-004 | Implemented | Unverified |
| DIA-REQ-005 | Implemented | Partial ABS test coverage |
| DIA-REQ-006 | Implemented | Unverified |
| DIA-REQ-007 | Implemented | Unverified |

---

## 5. Scope Limitation

Requirements Baseline v1.0 does not define:

- UDS services
- Diagnostic sessions
- Diagnostic Trouble Codes
- Diagnostic communication over CAN
- Security access
- ECU reprogramming

These capabilities may be introduced in future requirements baselines.