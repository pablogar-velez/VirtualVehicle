VirtualVehicle — Diagnostic Fault Expansion Requirements

Document Information

Project: VirtualVehicle

Document ID: VV-DFX-REQ-001

Baseline: v1.8

Status: VERIFICATION CLOSED

Parent Verified Baseline: v1.7

Parent Baseline Result: 254 PASS / 0 FAIL

Parent Baseline Requirements: 264 / 264 VERIFIED

Verification Method: Automated Test unless otherwise specified

1. Purpose

This document defines the High-Level Requirements for expanding VirtualVehicle
diagnostic fault injection beyond the two existing wheel-speed sensor faults.

Baseline v1.8 introduces deterministic runtime communication faults for the ABS,
Powertrain, Steering, and Automotive Ethernet subsystems while preserving the
complete verification-closed Baseline v1.7 behavior.

The fault expansion is intended for software-in-the-loop demonstration and
requirements-based verification. It does not claim physical electrical fault
modeling, network transceiver failure modeling, or hardware-in-the-loop fault
injection.

2. Parent Baseline

Baseline v1.7 is verification-closed with:

Automated Tests: 254 PASS / 0 FAIL

Requirements: 264 / 264 VERIFIED

Automotive Ethernet: 37 / 37 VERIFIED

Baseline v1.6 Regression: 217 PASS / 0 FAIL

The v1.8 fault-expansion requirements shall not be marked VERIFIED / PASS until
their dedicated automated tests and the complete Baseline v1.7 regression have
successfully executed.

3. ABS CAN Communication Fault

DFX-REQ-001 — ABS CAN Fault Injection

The SimulationEngine shall provide a runtime operation that injects an ABS CAN
communication fault and exposes the injected-fault state.

Verification Method: Automated Test
Verification Test: TC_DFX_001
Verification Status: VERIFIED / PASS

DFX-REQ-002 — ABS CAN Transmission Suppression

While the ABS CAN communication fault is active, the SimulationEngine shall
suppress periodic ABS_WHEEL_STATE CAN transmission.

The fault shall not disable simulation time progression or unrelated ECU CAN
traffic.

Verification Method: Automated Test
Verification Test: TC_DFX_002
Verification Status: VERIFIED / PASS

DFX-REQ-003 — ABS CAN Fault Recovery

When the ABS CAN communication fault is cleared, periodic ABS_WHEEL_STATE
transmission shall resume and the associated communication DTC shall no longer
be active.

Verification Method: Automated Test
Verification Test: TC_DFX_003
Verification Status: VERIFIED / PASS

4. Powertrain CAN Communication Fault

DFX-REQ-004 — Powertrain CAN Fault Injection

The SimulationEngine shall provide a runtime operation that injects a
Powertrain CAN communication fault and exposes the injected-fault state.

Verification Method: Automated Test
Verification Test: TC_DFX_004
Verification Status: VERIFIED / PASS

DFX-REQ-005 — Powertrain CAN Transmission Suppression

While the Powertrain CAN communication fault is active, the SimulationEngine
shall suppress periodic POWERTRAIN_STATE CAN transmission.

The fault shall not disable simulation time progression or unrelated ECU CAN
traffic.

Verification Method: Automated Test
Verification Test: TC_DFX_005
Verification Status: VERIFIED / PASS

DFX-REQ-006 — Powertrain CAN Fault Recovery

When the Powertrain CAN communication fault is cleared, periodic
POWERTRAIN_STATE transmission shall resume and the associated communication
DTC shall no longer be active.

Verification Method: Automated Test
Verification Test: TC_DFX_006
Verification Status: VERIFIED / PASS

5. Steering CAN Communication Fault

DFX-REQ-007 — Steering CAN Fault Injection

The SimulationEngine shall provide a runtime operation that injects a Steering
CAN communication fault and exposes the injected-fault state.

Verification Method: Automated Test
Verification Test: TC_DFX_007
Verification Status: VERIFIED / PASS

DFX-REQ-008 — Steering CAN Transmission Suppression

While the Steering CAN communication fault is active, the SimulationEngine
shall suppress periodic STEERING_STATE CAN transmission.

The fault shall not disable simulation time progression or unrelated ECU CAN
traffic.

Verification Method: Automated Test
Verification Test: TC_DFX_008
Verification Status: VERIFIED / PASS

DFX-REQ-009 — Steering CAN Fault Recovery

When the Steering CAN communication fault is cleared, periodic
STEERING_STATE transmission shall resume and the associated communication DTC
shall no longer be active.

Verification Method: Automated Test
Verification Test: TC_DFX_009
Verification Status: VERIFIED / PASS

6. Automotive Ethernet Communication Fault

DFX-REQ-010 — Ethernet Node A Communication Fault

The SimulationEngine shall provide a runtime operation that injects an
Automotive Ethernet Node A communication fault.

While the fault is active, an Ethernet frame whose source or destination is
Node A shall be rejected by the runtime submission path.

Verification Method: Automated Test
Verification Test: TC_DFX_010
Verification Status: VERIFIED / PASS

DFX-REQ-011 — Ethernet Node A Fault Recovery

When the Ethernet Node A communication fault is cleared, valid Node A Ethernet
communication shall be accepted again and the associated communication DTC
shall no longer be active.

Verification Method: Automated Test
Verification Test: TC_DFX_011
Verification Status: VERIFIED / PASS

7. DTC Integration

DFX-REQ-012 — Distinct Communication DTC Registration

The DTC Manager shall register distinct DTC definitions for:

ABS CAN Communication

Powertrain CAN Communication

Steering CAN Communication

Ethernet Node A Communication

Injecting each corresponding fault shall activate only its associated
communication DTC.

Verification Method: Automated Test
Verification Test: TC_DFX_012
Verification Status: VERIFIED / PASS

8. Reset Behavior

DFX-REQ-013 — Diagnostic Fault Reset

SimulationEngine reset shall clear all v1.8 injected communication-fault state,
restore normal communication behavior, and initialize the DTC Manager with no
active v1.8 communication DTCs.

Verification Method: Automated Test
Verification Test: TC_DFX_013
Verification Status: VERIFIED / PASS

9. Determinism

DFX-REQ-014 — Deterministic Fault Behavior

For two SimulationEngine instances with equivalent initial state and identical
fault injection, simulation update, fault-clear, and communication input
sequences, the v1.8 communication-fault behavior shall produce equivalent
fault-state, DTC-state, and relevant communication evidence.

Verification Method: Automated Test
Verification Test: TC_DFX_014
Verification Status: VERIFIED / PASS

10. Regression

DFX-REQ-015 — Complete Baseline v1.7 Regression Preservation

The complete Baseline v1.7 automated verification suite shall continue to pass
after integration of the diagnostic fault expansion.

Required regression result:

Previous Tests: 254

Required PASS: 254

Failed: 0

TC_DFX_015 provides a dedicated regression sentinel. Formal closure of this
requirement additionally requires successful execution of all 254 Baseline v1.7
tests within the complete v1.8 suite.

Verification Method: Automated Regression Test
Verification Test: TC_DFX_015 + complete 254-test Baseline v1.7 suite
Verification Status: VERIFIED / PASS

11. Verification Set

Test Case

Primary Requirement

Verification Objective

TC_DFX_001

DFX-REQ-001

ABS CAN fault injection state and DTC activation

TC_DFX_002

DFX-REQ-002

ABS CAN periodic transmission suppression

TC_DFX_003

DFX-REQ-003

ABS CAN communication recovery

TC_DFX_004

DFX-REQ-004

Powertrain CAN fault injection state and DTC activation

TC_DFX_005

DFX-REQ-005

Powertrain CAN periodic transmission suppression

TC_DFX_006

DFX-REQ-006

Powertrain CAN communication recovery

TC_DFX_007

DFX-REQ-007

Steering CAN fault injection state and DTC activation

TC_DFX_008

DFX-REQ-008

Steering CAN periodic transmission suppression

TC_DFX_009

DFX-REQ-009

Steering CAN communication recovery

TC_DFX_010

DFX-REQ-010

Ethernet Node A runtime communication blocking

TC_DFX_011

DFX-REQ-011

Ethernet Node A communication recovery

TC_DFX_012

DFX-REQ-012

Distinct communication DTC registration

TC_DFX_013

DFX-REQ-013

Runtime fault reset behavior

TC_DFX_014

DFX-REQ-014

Deterministic fault behavior

TC_DFX_015

DFX-REQ-015

Baseline v1.7 regression sentinel

Dedicated tests executed: 15

12. Baseline v1.8 Verification Result

Parent Baseline v1.7:

Requirements: 264

Verified: 264

Tests: 254

Passed: 254

Failed: 0

Baseline v1.8 executed result:

Previous Requirements: 264

New DFX Requirements: 15

Total Requirements: 279

Previous Tests: 254

New DFX Tests: 15

Total Tests: 269

Passed: 269

Failed: 0

The complete v1.8 automated suite was executed successfully.

Result: 269 PASS / 0 FAIL

The 15 DFX tests TC_DFX_001 through TC_DFX_015 completed with PASS status.
DFX-REQ-015 closure evidence includes the complete 254-test Baseline v1.7
regression remaining PASS within the 269-test v1.8 execution.

13. Verification Boundary

The following remain outside this fault-expansion milestone:

Physical sensor electrical fault modeling

Physical CAN transceiver failure

CAN bus short-to-ground / short-to-battery behavior

Bit-level CAN error frames

CAN error counters and bus-off state

Physical Ethernet PHY failure

Ethernet cable faults

Packet loss or corruption fault models

ECU power-supply faults

Hardware-in-the-loop fault injection

14. Current Status

Diagnostic Fault Expansion

Requirements Defined: 15

Implemented: 15

Verified: 15

Dedicated Tests Added: 15

Dedicated Tests Executed: 15 PASS / 0 FAIL

Parent Baseline: v1.7 — VERIFICATION CLOSED

Milestone Status:

VERIFICATION CLOSED