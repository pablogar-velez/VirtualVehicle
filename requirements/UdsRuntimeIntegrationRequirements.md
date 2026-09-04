VirtualVehicle — UDS Runtime Integration Requirements

Document Information

Project: VirtualVehicle

Document: UDS Runtime Integration Requirements

Baseline: v1.6

Status: Baseline / Verification Closed

Verification Method: Automated Test unless otherwise specified

Parent Verified Baseline: v1.5

Parent Baseline Result: 188 PASS / 0 FAIL

Parent Baseline Requirements: 198 / 198 VERIFIED

1. Purpose

This document defines the requirements for integrating UDS diagnostic CAN
traffic into the normal VirtualVehicle SimulationEngine runtime.

The objective of this milestone is to allow diagnostic request and response
frames to participate in the same VirtualCanBus arbitration and timing model
used by periodic ECU traffic.

The runtime diagnostic path shall coexist with:

ABS ECU traffic

Powertrain ECU traffic

Steering ECU traffic

Dashboard ECU reception

Existing CAN arbitration

Existing CAN timing

Existing CAN trace

Existing CAN statistics

The implementation shall preserve the verified behavior of Baseline v1.5.

2. Baseline Relationship

VirtualVehicle Baseline v1.5 already verifies the following diagnostic layers:

UdsServer
    ^
    |
UdsTransport
    ^
    |
ISO-TP
    ^
    |
UdsCanTransport
    ^
    |
VirtualCanBus

However, the verified UdsCanTransport behavior applies to isolated
synchronous diagnostic transactions.

Baseline v1.5 does not verify diagnostic traffic concurrently participating
with the normal SimulationEngine periodic ECU traffic.

This requirement set introduces that missing runtime integration.

3. Runtime Architecture

The target runtime architecture shall be:

                     SimulationEngine
                           |
                           |
        +------------------+------------------+
        |                  |                  |
        v                  v                  v
     ABS ECU          Powertrain ECU      Steering ECU
     0x080                0x100              0x120
        |                  |                  |
        +------------------+------------------+
                           |
                           v
                     VirtualCanBus
                           |
                           |
                 CAN Arbitration / Timing
                           |
                           v
                    Runtime CAN Router
                           |
          +----------------+----------------+
          |                                 |
          v                                 v
    Vehicle CAN Frames                Diagnostic Frames
          |                                 |
          v                                 v
    Dashboard ECU                     ISO-TP / UDS
                                            |
                                            v
                                        UdsServer
                                            |
                                            v
                                    Diagnostic Response
                                            |
                                            v
                                      VirtualCanBus
                                          0x7E8

Diagnostic traffic shall not bypass normal VirtualCanBus arbitration.

4. Runtime Diagnostic Request Requirements

UDSRT-REQ-001 — Runtime Diagnostic Request Submission

The SimulationEngine shall provide a mechanism for submitting a UDS diagnostic
request during an active simulation.

The submitted request shall not be processed directly by UdsServer.

The request shall first be converted into diagnostic CAN traffic.

Verification Method: Automated Test
Verification Test: TC_UDSRT_001
Verification Status: VERIFIED / PASS

UDSRT-REQ-002 — Runtime Request CAN Identifier

Runtime physical diagnostic requests shall use CAN arbitration identifier
0x7E0.

Verification Method: Automated Test
Verification Test: TC_UDSRT_002
Verification Status: VERIFIED / PASS

UDSRT-REQ-003 — Runtime Request ISO-TP Segmentation

The SimulationEngine diagnostic runtime shall use the existing ISO-TP
implementation to segment a submitted UDS request into one or more CAN frames.

Verification Method: Automated Test
Verification Test: TC_UDSRT_003
Verification Status: VERIFIED / PASS

UDSRT-REQ-004 — Shared CAN Bus Request Transmission

Runtime diagnostic request frames shall be queued through the same
VirtualCanBus instance used by periodic ECU traffic.

The implementation shall not use a separate diagnostic VirtualCanBus instance
for runtime diagnostic communication.

Verification Method: Automated Test
Verification Test: TC_UDSRT_004
Verification Status: VERIFIED / PASS

5. Shared-Bus Arbitration Requirements

UDSRT-REQ-005 — Shared Arbitration

Diagnostic frames and periodic ECU frames pending at the same arbitration
opportunity shall participate in the existing VirtualCanBus arbitration
algorithm.

The diagnostic subsystem shall not manually override the arbitration winner.

Verification Method: Automated Test
Verification Test: TC_UDSRT_005
Verification Status: VERIFIED / PASS

UDSRT-REQ-006 — CAN Priority Preservation

The existing lower-arbitration-identifier priority behavior shall remain
unchanged when diagnostic traffic is present.

For simultaneous pending frames, identifiers such as:

ABS         0x080
Powertrain  0x100
Steering    0x120
UDS Request 0x7E0

shall continue to arbitrate according to the existing CAN arbitration model.

Verification Method: Automated Test
Verification Test: TC_UDSRT_006
Verification Status: VERIFIED / PASS

UDSRT-REQ-007 — No Existing ECU Traffic Loss

The presence of runtime diagnostic traffic shall not cause pending ABS,
Powertrain, or Steering CAN frames to be discarded.

Verification Method: Automated Test
Verification Test: TC_UDSRT_007
Verification Status: VERIFIED / PASS

6. Runtime CAN Routing Requirements

UDSRT-REQ-008 — Central Runtime CAN Routing

Each CAN frame transmitted by VirtualCanBus during SimulationEngine runtime
shall be routed according to its arbitration identifier.

The runtime CAN routing mechanism shall distinguish vehicle communication
frames from diagnostic communication frames.

Verification Method: Automated Test
Verification Test: TC_UDSRT_008
Verification Status: VERIFIED / PASS

UDSRT-REQ-009 — Vehicle Frame Routing

Existing vehicle CAN frames shall continue to be delivered to the appropriate
existing vehicle consumer behavior.

The integration of diagnostic routing shall not remove existing Dashboard ECU
reception of supported vehicle CAN messages.

Verification Method: Automated Test
Verification Test: TC_UDSRT_009
Verification Status: VERIFIED / PASS

UDSRT-REQ-010 — Diagnostic Request Routing

CAN frames using the configured UDS request identifier 0x7E0 shall be routed
to the diagnostic ISO-TP request-reassembly path.

Diagnostic request frames shall not be interpreted as normal Dashboard ECU
vehicle-state messages.

Verification Method: Automated Test
Verification Test: TC_UDSRT_010
Verification Status: VERIFIED / PASS

UDSRT-REQ-011 — Diagnostic Response Routing

CAN frames using the configured UDS response identifier 0x7E8 shall be routed
to the diagnostic response-reassembly path.

Verification Method: Automated Test
Verification Test: TC_UDSRT_011
Verification Status: VERIFIED / PASS

7. UDS Processing Requirements

UDSRT-REQ-012 — Request Processing After Reassembly

UdsServer shall process a runtime diagnostic request only after the complete
UDS request payload has been successfully reassembled from the required ISO-TP
CAN frames.

Verification Method: Automated Test
Verification Test: TC_UDSRT_012
Verification Status: VERIFIED / PASS

UDSRT-REQ-013 — Current Runtime Vehicle Data

Runtime UDS requests shall operate on the current SimulationEngine diagnostic
vehicle data.

Read Data By Identifier requests shall use the current runtime values made
available to UdsServer by SimulationEngine.

Verification Method: Automated Test
Verification Test: TC_UDSRT_013
Verification Status: VERIFIED / PASS

8. Runtime Diagnostic Response Requirements

UDSRT-REQ-014 — Runtime Response Generation

After UdsServer processes a complete runtime diagnostic request, the resulting
UDS response shall be converted into ISO-TP response CAN frames.

Verification Method: Automated Test
Verification Test: TC_UDSRT_014
Verification Status: VERIFIED / PASS

UDSRT-REQ-015 — Runtime Response CAN Identifier

Runtime physical diagnostic response frames shall use CAN arbitration
identifier 0x7E8.

Verification Method: Automated Test
Verification Test: TC_UDSRT_015
Verification Status: VERIFIED / PASS

UDSRT-REQ-016 — Shared CAN Bus Response Transmission

Diagnostic response CAN frames shall be transmitted through the same
VirtualCanBus instance used by the SimulationEngine periodic ECU traffic.

Verification Method: Automated Test
Verification Test: TC_UDSRT_016
Verification Status: VERIFIED / PASS

UDSRT-REQ-017 — Runtime Response Reassembly

The diagnostic runtime shall reconstruct the final UDS response from the
runtime CAN response frames transmitted through VirtualCanBus.

Verification Method: Automated Test
Verification Test: TC_UDSRT_017
Verification Status: VERIFIED / PASS

9. CAN Evidence Requirements

UDSRT-REQ-018 — Runtime Diagnostic CAN Trace

Runtime diagnostic request and response frames shall appear in the existing
VirtualCanBus CAN trace.

The trace shall preserve the same timing and payload evidence used for
non-diagnostic CAN traffic.

Verification Method: Automated Test
Verification Test: TC_UDSRT_018
Verification Status: VERIFIED / PASS

UDSRT-REQ-019 — Runtime Diagnostic CAN Statistics

Runtime diagnostic request and response transmissions shall contribute to the
existing VirtualCanBus transmission statistics.

Verification Method: Automated Test
Verification Test: TC_UDSRT_019
Verification Status: VERIFIED / PASS

UDSRT-REQ-020 — Runtime Diagnostic Timing

Runtime diagnostic CAN frames shall participate in the existing VirtualCanBus
transmission-time and waiting-time model.

Diagnostic frames shall not bypass the CAN busy-until timing state.

Verification Method: Automated Test
Verification Test: TC_UDSRT_020
Verification Status: VERIFIED / PASS

10. Transaction State Requirements

UDSRT-REQ-021 — Pending Diagnostic Transaction State

The runtime diagnostic subsystem shall maintain sufficient state to represent a
diagnostic transaction that spans multiple CAN transmissions and multiple
SimulationEngine processing events.

Verification Method: Automated Test
Verification Test: TC_UDSRT_021
Verification Status: VERIFIED / PASS

UDSRT-REQ-022 — Diagnostic Transaction Completion

A runtime diagnostic transaction shall be considered complete only after the
final UDS response has been reconstructed from the diagnostic response CAN
traffic.

Verification Method: Automated Test
Verification Test: TC_UDSRT_022
Verification Status: VERIFIED / PASS

UDSRT-REQ-023 — Diagnostic Transaction Result Access

The SimulationEngine shall provide a mechanism for determining whether a
submitted runtime diagnostic transaction has completed and for obtaining the
resulting UDS response.

Verification Method: Automated Test
Verification Test: TC_UDSRT_023
Verification Status: VERIFIED / PASS

11. Reset Requirements

UDSRT-REQ-024 — Runtime Diagnostic Reset

SimulationEngine reset shall clear any incomplete runtime diagnostic transport
state.

The reset shall preserve the existing UdsServer reset behavior defined by the
verified UDS baseline.

Verification Method: Automated Test
Verification Test: TC_UDSRT_024
Verification Status: VERIFIED / PASS

UDSRT-REQ-025 — Reset CAN Isolation

After SimulationEngine reset, no diagnostic CAN frame belonging to a
transaction initiated before the reset shall remain pending for transmission.

Verification Method: Automated Test
Verification Test: TC_UDSRT_025
Verification Status: VERIFIED / PASS

12. Regression Requirements

UDSRT-REQ-026 — Existing ECU Runtime Regression

The runtime diagnostic integration shall preserve the previously verified ABS,
Powertrain, Steering, vehicle-model, sensor, DTC, and CAN runtime behavior.

Verification Method: Automated Test
Verification Test: TC_UDSRT_026
Verification Status: VERIFIED / PASS

UDSRT-REQ-027 — Existing Diagnostic Regression

The runtime diagnostic integration shall preserve the existing verified UDS,
ISO-TP, UDS Transport, and isolated UDS CAN Transport behavior.

Verification Method: Automated Test
Verification Test: TC_UDSRT_027
Verification Status: VERIFIED / PASS

UDSRT-REQ-028 — Previous Baseline Regression

The complete Baseline v1.5 automated validation suite shall continue to pass
after runtime diagnostic integration.

The required regression result shall remain:

Previous Tests: 188
Required PASS:  188
Required FAIL:    0

Verification Method: Automated Regression Test
Verification Test: TC_UDSRT_028 plus complete Baseline v1.5 suite
Verification Status: VERIFIED / PASS

13. Determinism Requirements

UDSRT-REQ-029 — Deterministic Runtime Diagnostic Processing

For identical initial SimulationEngine state, identical CAN state, identical
diagnostic request, identical request time, and identical simulation update
sequence, the runtime diagnostic subsystem shall produce equivalent diagnostic
responses and CAN transaction ordering.

Verification Method: Automated Test
Verification Test: TC_UDSRT_029
Verification Status: VERIFIED / PASS

14. Verification Set

The dedicated automated verification set is:

Test Case

Primary Requirement

Verification Objective

TC_UDSRT_001

UDSRT-REQ-001

Runtime diagnostic request submission

TC_UDSRT_002

UDSRT-REQ-002

Runtime request CAN identifier

TC_UDSRT_003

UDSRT-REQ-003

Runtime request ISO-TP segmentation

TC_UDSRT_004

UDSRT-REQ-004

Shared VirtualCanBus request transmission

TC_UDSRT_005

UDSRT-REQ-005

Shared CAN arbitration

TC_UDSRT_006

UDSRT-REQ-006

Existing CAN priority preservation

TC_UDSRT_007

UDSRT-REQ-007

Existing ECU frame preservation

TC_UDSRT_008

UDSRT-REQ-008

Central CAN routing

TC_UDSRT_009

UDSRT-REQ-009

Vehicle CAN routing preservation

TC_UDSRT_010

UDSRT-REQ-010

Diagnostic request routing

TC_UDSRT_011

UDSRT-REQ-011

Diagnostic response routing

TC_UDSRT_012

UDSRT-REQ-012

Request processing after reassembly

TC_UDSRT_013

UDSRT-REQ-013

Current runtime diagnostic data

TC_UDSRT_014

UDSRT-REQ-014

Runtime UDS response generation

TC_UDSRT_015

UDSRT-REQ-015

Runtime response CAN identifier

TC_UDSRT_016

UDSRT-REQ-016

Shared VirtualCanBus response transmission

TC_UDSRT_017

UDSRT-REQ-017

Runtime response reassembly

TC_UDSRT_018

UDSRT-REQ-018

Runtime diagnostic CAN trace

TC_UDSRT_019

UDSRT-REQ-019

Runtime diagnostic CAN statistics

TC_UDSRT_020

UDSRT-REQ-020

Runtime diagnostic CAN timing

TC_UDSRT_021

UDSRT-REQ-021

Pending transaction state

TC_UDSRT_022

UDSRT-REQ-022

Transaction completion

TC_UDSRT_023

UDSRT-REQ-023

Transaction result access

TC_UDSRT_024

UDSRT-REQ-024

Runtime diagnostic reset

TC_UDSRT_025

UDSRT-REQ-025

Reset pending-CAN isolation

TC_UDSRT_026

UDSRT-REQ-026

Existing ECU runtime regression

TC_UDSRT_027

UDSRT-REQ-027

Existing diagnostic regression

TC_UDSRT_028

UDSRT-REQ-028

Full Baseline v1.5 regression

TC_UDSRT_029

UDSRT-REQ-029

Deterministic runtime diagnostics

Dedicated tests: 29

15. Baseline v1.6 Verification Closure

Baseline v1.5 remains preserved as the parent verification baseline:

Baseline v1.5

Requirements: 198
Verified:     198

Tests:        188
Passed:       188
Failed:         0

Baseline v1.6 adds 29 runtime integration requirements and 29 dedicated
verification tests.

Previous Requirements: 198
New Requirements:       29
Total Requirements:    227

Previous Tests:         188
New Tests:               29
Total Tests:            217

Executed verification result:

Automated Tests: 217
Passed:          217
Failed:            0

Requirements:    227
Verified:        227
Unverified:        0

All 29 UDS Runtime Integration requirements are VERIFIED / PASS.

The complete Baseline v1.5 automated suite also remained 188 PASS / 0 FAIL
after runtime integration.

Baseline v1.6 is therefore VERIFICATION CLOSED.

16. Verification Boundary

The following capabilities remain outside this runtime integration milestone:

Full ISO 14229 compliance

Full ISO 15765-2 compliance

ISO-TP real-time Flow Control scheduling

ISO-TP Block Size enforcement

ISO-TP STmin enforcement

Parallel diagnostic clients

Functional diagnostic addressing

Extended diagnostic addressing

CAN FD diagnostics

Security Access

ECU Reset UDS service

Communication Control

Routine Control

Request Download

Transfer Data

ECU programming / flashing

AUTOSAR runtime

Automotive Ethernet

Physical CAN hardware

Physical diagnostic hardware

Hardware-in-the-loop diagnostic transport

Future requirement sets shall be created before any of these capabilities are
claimed as formally verified.

17. Current Status

UDS Runtime Integration

Requirements Defined: 29
Implemented:          29
Verified:             29

Dedicated Tests:
29 PASS / 0 FAIL

Parent Baseline:
v1.5 — 188 PASS / 0 FAIL

Baseline v1.6:
217 PASS / 0 FAIL
227 / 227 requirements VERIFIED

Milestone Status:
VERIFICATION CLOSED