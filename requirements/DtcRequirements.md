# VirtualVehicle — DTC Requirements

## Document Information

- Project: VirtualVehicle
- Document: DTC Requirements
- Baseline: v1.3
- Status: Baseline
- Verification Method: Automated Test unless otherwise specified

---

# 1. Purpose

This document defines the requirements for Diagnostic Trouble Code (DTC)
management within the VirtualVehicle simulation platform.

The DTC subsystem shall detect, store, update, recover, and clear diagnostic
fault information associated with simulated vehicle components.

The initial implementation covers the front-left and front-right wheel-speed
sensor diagnostic conditions used by the ABS subsystem.

---

# 2. Scope

The DTC subsystem includes:

- DTC definition and identification
- Fault detection
- Active fault status
- Stored fault information
- Fault recovery
- Individual DTC clearing
- Global DTC clearing
- Fault occurrence counting
- Detection timestamps
- Diagnostic state access
- Deterministic reset behavior

The following functionality is outside the scope of this document:

- UDS service implementation
- ISO-TP transport
- Diagnostic sessions
- Security access
- ECU reprogramming
- Physical diagnostic hardware
- Physical CAN transceiver validation

These features may be specified by separate requirements.

---

# 3. DTC Definitions

## DTC-REQ-001 — Front-Left Wheel-Speed Sensor DTC

The diagnostic subsystem shall define a DTC for an invalid front-left
wheel-speed sensor signal.

DTC code:

`0xC0031`

Verification Method: Automated Test

---

## DTC-REQ-002 — Front-Right Wheel-Speed Sensor DTC

The diagnostic subsystem shall define a DTC for an invalid front-right
wheel-speed sensor signal.

DTC code:

`0xC0034`

Verification Method: Automated Test

---

# 4. Fault Detection

## DTC-REQ-003 — Front-Left Fault Activation

When the front-left wheel-speed sensor produces an invalid reading, the
diagnostic subsystem shall set DTC `0xC0031` to Active.

Verification Method: Automated Test

---

## DTC-REQ-004 — Front-Right Fault Activation

When the front-right wheel-speed sensor produces an invalid reading, the
diagnostic subsystem shall set DTC `0xC0034` to Active.

Verification Method: Automated Test

---

# 5. Fault Recovery

## DTC-REQ-005 — Front-Left Fault Recovery

When DTC `0xC0031` is Active and the front-left wheel-speed sensor returns
to a valid state, the diagnostic subsystem shall change the current DTC
status from Active to Inactive.

The DTC record shall remain available until explicitly cleared or until
the simulation is reset.

Verification Method: Automated Test

---

## DTC-REQ-006 — Front-Right Fault Recovery

When DTC `0xC0034` is Active and the front-right wheel-speed sensor returns
to a valid state, the diagnostic subsystem shall change the current DTC
status from Active to Inactive.

The DTC record shall remain available until explicitly cleared or until
the simulation is reset.

Verification Method: Automated Test

---

# 6. DTC Clearing

## DTC-REQ-007 — Individual DTC Clear

The diagnostic subsystem shall provide the capability to clear an
individual DTC by its diagnostic code.

After an individual DTC is cleared:

- status shall be Cleared
- first-detected timestamp shall be 0.0 ms
- last-detected timestamp shall be 0.0 ms
- occurrence count shall be zero

Verification Method: Automated Test

---

## DTC-REQ-008 — Clear All DTCs

The diagnostic subsystem shall provide the capability to clear all
registered DTCs.

After all DTCs are cleared, each registered DTC shall have:

- status equal to Cleared
- first-detected timestamp equal to 0.0 ms
- last-detected timestamp equal to 0.0 ms
- occurrence count equal to zero

Verification Method: Automated Test

---

# 7. DTC Timing Information

## DTC-REQ-009 — First Detection Timestamp

When a DTC is detected for the first time after initialization or an
explicit clear operation, the diagnostic subsystem shall store the
simulation time at which the fault was first detected.

The first-detected timestamp shall remain unchanged while the same DTC
continues to be reported as faulty.

Verification Method: Automated Test

---

## DTC-REQ-010 — Last Detection Timestamp

While a DTC condition is present, the diagnostic subsystem shall update
the last-detected timestamp whenever the fault is evaluated and reported.

Verification Method: Automated Test

---

# 8. Occurrence Counter

## DTC-REQ-011 — DTC Occurrence Count

The diagnostic subsystem shall maintain an occurrence count for each DTC.

The occurrence count shall increase by one when the DTC transitions from
a non-Active state to Active.

Continuous evaluations of the same fault while the DTC remains Active
shall not increment the occurrence count.

If the fault recovers and later becomes Active again, the occurrence
count shall increase by one.

Verification Method: Automated Test

---

# 9. Diagnostic Access

## DTC-REQ-012 — DTC Lookup

The diagnostic subsystem shall provide the capability to retrieve a DTC
using its diagnostic code.

Verification Method: Automated Test

---

## DTC-REQ-013 — DTC Collection Access

The diagnostic subsystem shall provide read-only access to the collection
of registered DTCs.

Verification Method: Automated Test

---

## DTC-REQ-014 — Active DTC Query

The diagnostic subsystem shall provide the capability to determine
whether a specified DTC is currently Active.

Verification Method: Automated Test

---

# 10. Simulation Integration

## DTC-REQ-015 — Runtime Fault Detection

DTC detection shall operate during normal VirtualVehicle simulation
execution without requiring a simulation restart.

Verification Method: Automated Test

---

## DTC-REQ-016 — Runtime Fault Recovery

A DTC shall be capable of transitioning from Active to Inactive after
the corresponding sensor fault is removed without requiring a simulation
restart.

Verification Method: Automated Test

---

# 11. Reset Behavior

## DTC-REQ-017 — Diagnostic Reset

When the VirtualVehicle simulation is reset, the DTC subsystem shall
return to its initial diagnostic state.

For every registered DTC:

- status shall be Inactive
- first-detected timestamp shall be 0.0 ms
- last-detected timestamp shall be 0.0 ms
- occurrence count shall be zero

Verification Method: Automated Test

---

# 12. Determinism

## DTC-REQ-018 — Deterministic Diagnostic Behavior

For identical initial conditions, fault stimuli, and simulation timing,
the DTC subsystem shall produce identical diagnostic states, occurrence
counts, and timestamps.

Verification Method: Automated Test

---

# 13. Initial DTC Inventory

| DTC Code | Name | Monitored Condition |
|---|---|---|
| `0xC0031` | Front Left Wheel Speed Sensor | Invalid front-left wheel-speed sensor reading |
| `0xC0034` | Front Right Wheel Speed Sensor | Invalid front-right wheel-speed sensor reading |

---

# 14. Traceability

| Requirement | Verification Test | Result |
|---|---|---|
| DTC-REQ-001 | TC_DTC_001 | PASS |
| DTC-REQ-002 | TC_DTC_002 | PASS |
| DTC-REQ-003 | TC_DTC_003 | PASS |
| DTC-REQ-004 | TC_DTC_004 | PASS |
| DTC-REQ-005 | TC_DTC_005 | PASS |
| DTC-REQ-006 | TC_DTC_006 | PASS |
| DTC-REQ-007 | TC_DTC_007 | PASS |
| DTC-REQ-008 | TC_DTC_008 | PASS |
| DTC-REQ-009 | TC_DTC_009 | PASS |
| DTC-REQ-010 | TC_DTC_010 | PASS |
| DTC-REQ-011 | TC_DTC_011 | PASS |
| DTC-REQ-012 | TC_DTC_012 | PASS |
| DTC-REQ-013 | TC_DTC_013 | PASS |
| DTC-REQ-014 | TC_DTC_014 | PASS |
| DTC-REQ-015 | TC_DTC_015 | PASS |
| DTC-REQ-016 | TC_DTC_016 | PASS |
| DTC-REQ-017 | TC_DTC_017 | PASS |
| DTC-REQ-018 | TC_DTC_018 | PASS |

---

# 15. Baseline Status

Baseline v1.3 introduces DTC management as a verified diagnostic capability.

Verification result:

- DTC requirements: **18**
- Verified DTC requirements: **18**
- Automated DTC tests: **18**
- PASS: **18**
- FAIL: **0**

Current document status:

`VERIFIED / PASS`