VirtualVehicle Requirements Specification

Document ID: VV-SRS-001
Version: 1.7
Status: Baseline / Verification Closed
System: VirtualVehicle Platform

Verification Workflow

Requirement → Implementation → Test Case → Execution → Evidence → PASS / FAIL

Implementation alone does not constitute verification.

Verification states:

NOT IMPLEMENTED

IMPLEMENTED / UNVERIFIED

VERIFIED / PASS

VERIFICATION FAILED

A requirement shall only be classified as VERIFIED / PASS when the defined
verification evidence has been successfully executed or inspected.

Requirement Categories

Prefix

Domain

Specification

SYS

System

SystemRequirements.md

VEH

Vehicle Model

VehicleRequirements.md

ABS

ABS ECU

AbsRequirements.md

SNS

Sensors

SensorRequirements.md

PT

Powertrain ECU

PowertrainRequirements.md

STR

Steering ECU

SteeringRequirements.md

CAN

CAN Bus

CanRequirements.md

MSG

CAN Messages

MessageRequirements.md

DIA

Diagnostics / Fault Injection

DiagnosticsRequirements.md

DTC

Diagnostic Trouble Codes

DtcRequirements.md

UDS

Unified Diagnostic Services

UdsRequirements.md

ISOTP

ISO-TP Transport

IsotpRequirements.md

UDSTP

UDS / ISO-TP Integration

UdsTransportRequirements.md

UDSCAN

UDS Virtual CAN Transport

UdsCanTransportRequirements.md

UDSRT

UDS Runtime Integration

UdsRuntimeIntegrationRequirements.md

ETH

Automotive Ethernet

AutomotiveEthernetRequirements.md

VAL

Automated Validation

ValidationRequirements.md

Baseline v1.7 Scope

Baseline v1.6 extends the verified VirtualVehicle platform with runtime UDS
diagnostic traffic integrated into the SimulationEngine shared VirtualCanBus.
The previously verified v1.5 ISO-TP, UDS Transport, and isolated UDS CAN
Transport behavior is preserved.

The diagnostic stack now provides:

Diagnostic request representation

Diagnostic response representation

UDS server request processing

Positive diagnostic responses

Negative diagnostic responses

Diagnostic Session Control service 0x10

Default Diagnostic Session 0x01

Extended Diagnostic Session 0x03

Read DTC Information service 0x19

Active DTC reporting

Clear Diagnostic Information service 0x14

DTC clearing through the existing DTC Manager

Read Data By Identifier service 0x22

Vehicle Speed DID 0xF100

Engine RPM DID 0xF101

Steering Angle DID 0xF102

Runtime diagnostic processing

Current simulation-state access

Diagnostic reset behavior

Deterministic diagnostic responses

ISO-TP Single Frame transport

ISO-TP First Frame transport

ISO-TP Consecutive Frame transport

ISO-TP Flow Control frame representation

ISO-TP diagnostic payload segmentation

ISO-TP diagnostic payload reassembly

ISO-TP sequence-number validation

ISO-TP sequence-number rollover

UDS request serialization

UDS response serialization

UDS request transport through ISO-TP

UDS response transport through ISO-TP

Diagnostic request CAN ID 0x7E0

Diagnostic response CAN ID 0x7E8

UDS request transmission through VirtualCanBus

UDS response transmission through VirtualCanBus

Diagnostic CAN trace evidence

Diagnostic CAN timing evidence

Single-frame diagnostic transactions

Multi-frame diagnostic transactions

Positive-response preservation through the transport stack

Negative-response preservation through the transport stack

Deterministic diagnostic CAN transactions

Runtime UDS request submission through SimulationEngine

Runtime request routing through CAN ID 0x7E0

Runtime response routing through CAN ID 0x7E8

Shared arbitration with ABS, Powertrain, and Steering ECU traffic

Runtime ISO-TP request and response reassembly

Runtime CAN trace, timing, and statistics evidence

Runtime diagnostic transaction state and result access

Runtime diagnostic reset isolation

Full Baseline v1.5 regression preservation

The UDS subsystem remains integrated with the existing DTC Manager and
SimulationEngine.

The ISO-TP subsystem provides deterministic diagnostic payload segmentation
and reassembly using simulated Classic CAN frames.

The UDS Transport subsystem connects UDS request and response processing to
ISO-TP.

The UDS CAN Transport subsystem routes diagnostic ISO-TP frames through the
existing VirtualCanBus for isolated diagnostic transactions.

The UDS Runtime Integration subsystem routes diagnostic request and response
frames through the same SimulationEngine VirtualCanBus used by periodic ECU
traffic, preserving shared arbitration, timing, trace, and statistics behavior.

Baseline v1.6 implements a simplified UDS-over-ISO-TP diagnostic communication
model for software-in-the-loop simulation and verification.

It does not claim full ISO 14229 or ISO 15765-2 compliance.

Diagnostic Architecture

The verified Baseline v1.6 diagnostic runtime architecture is:

                 SimulationEngine
                      |
    +-----------------+-----------------+
    |                 |                 |
    v                 v                 v
 ABS ECU        Powertrain ECU     Steering ECU
 0x080              0x100              0x120
    |                 |                 |
    +-----------------+-----------------+
                      |
                      v
                 VirtualCanBus
                      |
                      v
             CAN Arbitration / Timing
                      |
                      v
                Runtime CAN Router
                 /             \
                /               \
               v                 v
      Vehicle CAN Frames    Diagnostic Frames
               |             0x7E0 / 0x7E8
               v                 |
         Dashboard ECU           v
                             ISO-TP / UDS
                                  |
                                  v
                              UdsServer
                                  |
                                  v
                          Diagnostic Response
                                  |
                                  v
                             VirtualCanBus

Diagnostic traffic participates in the same VirtualCanBus arbitration and timing
model as periodic ECU traffic.

The previously verified isolated UdsCanTransport remains part of the architecture,
but Baseline v1.6 additionally verifies the shared SimulationEngine runtime path.

Current Automated Test Coverage

Suite result: 254 Passed / 0 Failed

The Baseline v1.6 automated validation suite contains:

Domain

Test Range

Count

System

TC_SYS_001 – TC_SYS_004

4

Vehicle

TC_VEH_001 – TC_VEH_011

11

ABS

TC_ABS_001 – TC_ABS_007

7

Sensors

TC_SNS_001 – TC_SNS_006

6

Powertrain

TC_PT_001 – TC_PT_007

7

Steering

TC_STR_001 – TC_STR_004

4

Diagnostics / Fault Injection

TC_DIA_001 – TC_DIA_007

7

Diagnostic Trouble Codes

TC_DTC_001 – TC_DTC_018

18

Unified Diagnostic Services

TC_UDS_001 – TC_UDS_033

33

ISO-TP

TC_ISOTP_001 – TC_ISOTP_026

26

UDS Transport

TC_UDSTP_001 – TC_UDSTP_012

12

UDS CAN Transport

TC_UDSCAN_001 – TC_UDSCAN_015

15

UDS Runtime Integration

TC_UDSRT_001 – TC_UDSRT_029

29

CAN

TC_CAN_001 – TC_CAN_022

22

CAN Messages

TC_MSG_001 – TC_MSG_010

10

Validation Framework

TC_VAL_001 – TC_VAL_006

6

Total automated tests: 254

UDS Automated Verification

Test Case

Primary Requirement

Description

TC_UDS_001

UDS-REQ-001

UDS Request Representation

TC_UDS_002

UDS-REQ-002

UDS Response Representation

TC_UDS_003

UDS-REQ-003

UDS Server Request Processing

TC_UDS_004

UDS-REQ-004

Positive Response SID

TC_UDS_005

UDS-REQ-005

Negative Response Format

TC_UDS_006

UDS-REQ-006

Unsupported Service

TC_UDS_007

UDS-REQ-007

Diagnostic Session Control Support

TC_UDS_008

UDS-REQ-008

Default Diagnostic Session

TC_UDS_009

UDS-REQ-009

Extended Diagnostic Session

TC_UDS_010

UDS-REQ-010

Diagnostic Session State Update

TC_UDS_011

UDS-REQ-011

Unsupported Diagnostic Session

TC_UDS_012

UDS-REQ-012

Read DTC Information Support

TC_UDS_013

UDS-REQ-013

Read Active DTCs

TC_UDS_014

UDS-REQ-014

Front-Left DTC Reporting

TC_UDS_015

UDS-REQ-015

Front-Right DTC Reporting

TC_UDS_016

UDS-REQ-016

No Active DTC Response

TC_UDS_017

UDS-REQ-017

Clear Diagnostic Information Support

TC_UDS_018

UDS-REQ-018

Clear All DTCs Through UDS

TC_UDS_019

UDS-REQ-019

Clear Diagnostic Positive Response

TC_UDS_020

UDS-REQ-020

Read Data By Identifier Support

TC_UDS_021

UDS-REQ-021

Vehicle Speed DID Definition

TC_UDS_022

UDS-REQ-022

Vehicle Speed DID Readout

TC_UDS_023

UDS-REQ-023

Engine RPM DID Definition

TC_UDS_024

UDS-REQ-024

Engine RPM DID Readout

TC_UDS_025

UDS-REQ-025

Steering Angle DID Definition

TC_UDS_026

UDS-REQ-026

Steering Angle DID Readout

TC_UDS_027

UDS-REQ-027

Unsupported DID

TC_UDS_028

UDS-REQ-028

DTC Manager Access

TC_UDS_029

UDS-REQ-029

DTC Clear Integration

TC_UDS_030

UDS-REQ-030

Runtime Diagnostic Processing

TC_UDS_031

UDS-REQ-031

Current Vehicle Data

TC_UDS_032

UDS-REQ-032

UDS Reset State

TC_UDS_033

UDS-REQ-033

Deterministic Diagnostic Response

All UDS tests completed with PASS status.

Result: 33 PASS / 0 FAIL

ISO-TP Automated Verification

Test Range

Requirement Range

Result

TC_ISOTP_001 – TC_ISOTP_026

ISOTP-REQ-001 – ISOTP-REQ-026

26 PASS / 0 FAIL

The ISO-TP automated verification covers:

Transport component behavior

Classic CAN frame size

Standard 11-bit CAN identifiers

Maximum payload length

Empty-payload rejection

Single Frame generation

Single Frame payload capacity

Single Frame PCI encoding

First Frame generation

First Frame payload-length encoding

First Frame data capacity

Consecutive Frame generation

Consecutive Frame data capacity

Initial sequence number

Sequence-number increment

Sequence-number rollover

Flow Control frame representation

Flow Status representation

Flow Control parameters

Single Frame reassembly

Multi-frame reassembly

Sequence-number validation

CAN identifier validation

Incomplete-message detection

Deterministic segmentation

Deterministic reassembly

All ISO-TP tests completed with PASS status.

Result: 26 PASS / 0 FAIL

UDS Transport Automated Verification

Test Case

Primary Requirement

Description

TC_UDSTP_001

UDSTP-REQ-001

UDS Request Transport

TC_UDSTP_002

UDSTP-REQ-002

UDS Response Transport

TC_UDSTP_003

UDSTP-REQ-003

Request CAN Identifier

TC_UDSTP_004

UDSTP-REQ-004

Response CAN Identifier

TC_UDSTP_005

UDSTP-REQ-005

Request Serialization

TC_UDSTP_006

UDSTP-REQ-006

Response Serialization

TC_UDSTP_007

UDSTP-REQ-007

Positive Response Preservation

TC_UDSTP_008

UDSTP-REQ-008

Negative Response Preservation

TC_UDSTP_009

UDSTP-REQ-009

Request Identifier Validation

TC_UDSTP_010

UDSTP-REQ-010

Response Identifier Validation

TC_UDSTP_011

UDSTP-REQ-011

End-to-End UDS Processing

TC_UDSTP_012

UDSTP-REQ-012

Deterministic Transport

All UDS Transport tests completed with PASS status.

Result: 12 PASS / 0 FAIL

UDS CAN Transport Automated Verification

Test Case

Primary Requirement

Description

TC_UDSCAN_001

UDSCAN-REQ-001

UDS CAN Transport Component

TC_UDSCAN_002

UDSCAN-REQ-002

Request Transmission

TC_UDSCAN_003

UDSCAN-REQ-003

Response Transmission

TC_UDSCAN_004

UDSCAN-REQ-004

Request Identifier

TC_UDSCAN_005

UDSCAN-REQ-005

Response Identifier

TC_UDSCAN_006

UDSCAN-REQ-006

Request Reassembly

TC_UDSCAN_007

UDSCAN-REQ-007

Response Reassembly

TC_UDSCAN_008

UDSCAN-REQ-008

End-to-End Diagnostic Transaction

TC_UDSCAN_009

UDSCAN-REQ-009

Diagnostic CAN Trace

TC_UDSCAN_010

UDSCAN-REQ-010

Diagnostic CAN Timing

TC_UDSCAN_011

UDSCAN-REQ-011

Transaction Evidence

TC_UDSCAN_012

UDSCAN-REQ-012

Positive Response Preservation

TC_UDSCAN_013

UDSCAN-REQ-013

Negative Response Preservation

TC_UDSCAN_014

UDSCAN-REQ-014

Multi-Frame Diagnostic Transport

TC_UDSCAN_015

UDSCAN-REQ-015

Deterministic CAN Diagnostic Transaction

All UDS CAN Transport tests completed with PASS status.

Result: 15 PASS / 0 FAIL

UDS Runtime Integration Automated Verification

Test Case

Primary Requirement

Description

TC_UDSRT_001

UDSRT-REQ-001

Runtime Diagnostic Request Submission

TC_UDSRT_002

UDSRT-REQ-002

Runtime Request CAN Identifier

TC_UDSRT_003

UDSRT-REQ-003

Runtime Request ISO-TP Segmentation

TC_UDSRT_004

UDSRT-REQ-004

Shared VirtualCanBus Request Transmission

TC_UDSRT_005

UDSRT-REQ-005

Shared CAN Arbitration

TC_UDSRT_006

UDSRT-REQ-006

Existing CAN Priority Preservation

TC_UDSRT_007

UDSRT-REQ-007

Existing ECU Frame Preservation

TC_UDSRT_008

UDSRT-REQ-008

Central CAN Routing

TC_UDSRT_009

UDSRT-REQ-009

Vehicle CAN Routing Preservation

TC_UDSRT_010

UDSRT-REQ-010

Diagnostic Request Routing

TC_UDSRT_011

UDSRT-REQ-011

Diagnostic Response Routing

TC_UDSRT_012

UDSRT-REQ-012

Request Processing After Reassembly

TC_UDSRT_013

UDSRT-REQ-013

Current Runtime Diagnostic Data

TC_UDSRT_014

UDSRT-REQ-014

Runtime UDS Response Generation

TC_UDSRT_015

UDSRT-REQ-015

Runtime Response CAN Identifier

TC_UDSRT_016

UDSRT-REQ-016

Shared VirtualCanBus Response Transmission

TC_UDSRT_017

UDSRT-REQ-017

Runtime Response Reassembly

TC_UDSRT_018

UDSRT-REQ-018

Runtime Diagnostic CAN Trace

TC_UDSRT_019

UDSRT-REQ-019

Runtime Diagnostic CAN Statistics

TC_UDSRT_020

UDSRT-REQ-020

Runtime Diagnostic CAN Timing

TC_UDSRT_021

UDSRT-REQ-021

Pending Diagnostic Transaction State

TC_UDSRT_022

UDSRT-REQ-022

Diagnostic Transaction Completion

TC_UDSRT_023

UDSRT-REQ-023

Diagnostic Transaction Result Access

TC_UDSRT_024

UDSRT-REQ-024

Runtime Diagnostic Reset

TC_UDSRT_025

UDSRT-REQ-025

Reset CAN Isolation

TC_UDSRT_026

UDSRT-REQ-026

Existing ECU Runtime Regression

TC_UDSRT_027

UDSRT-REQ-027

Existing Diagnostic Regression

TC_UDSRT_028

UDSRT-REQ-028

Complete Baseline v1.5 Regression

TC_UDSRT_029

UDSRT-REQ-029

Deterministic Runtime Diagnostic Processing

All UDS Runtime Integration tests completed with PASS status.

Result: 29 PASS / 0 FAIL

The complete Baseline v1.5 regression also remained 188 PASS / 0 FAIL.

Traceability

Project-level traceability is maintained in TraceabilityMatrix.md.

The matrix distinguishes direct verification from supporting evidence so that
implemented behavior is not incorrectly reported as formally verified.

Verification Evidence

TraceabilityMatrix.md — complete requirement-to-evidence mapping.

InspectionVerification.md — formal inspection records for requirements whose
verification method is Inspection.

Automated Test Runner — executable requirements-based verification evidence.

VirtualCanBus trace — simulated CAN transmission evidence.

TestResult expected / actual fields — test execution evidence.

TestResult timing fields — execution and response timing evidence where
applicable.

Baseline Evolution

Baseline v1.2

Automated tests: 84

Verified requirements: 94

Baseline v1.3

Added Diagnostic Trouble Code management.

Automated tests: 102

Verified requirements: 112

Baseline v1.4

Added simplified Unified Diagnostic Services.

Automated tests: 135

Verified requirements: 145

Baseline v1.5

Added:

ISO-TP transport core

UDS / ISO-TP transport integration

UDS diagnostic transport through VirtualCanBus

Baseline v1.5:

Automated tests: 188

Verified requirements: 198

Baseline v1.6

Added:

Runtime UDS request submission through SimulationEngine

Shared VirtualCanBus diagnostic arbitration with periodic ECU traffic

Runtime CAN routing for diagnostic request and response frames

Runtime ISO-TP request and response reconstruction

Runtime diagnostic transaction state, reset, trace, timing, and statistics verification

Full Baseline v1.5 regression preservation

Baseline v1.6:

Automated tests: 217

Verified requirements: 227

Baseline v1.7 Verification Closure

Automated suite: 254 PASS / 0 FAIL

Baseline requirements: 264

Verified baseline requirements: 264

Unverified baseline requirements: 0

DTC requirements: 18

DTC requirements verified: 18

UDS requirements: 33

UDS requirements verified: 33

ISO-TP requirements: 26

ISO-TP requirements verified: 26

UDS Transport requirements: 12

UDS Transport requirements verified: 12

UDS CAN Transport requirements: 15

UDS CAN Transport requirements verified: 15

UDS Runtime Integration requirements: 29

UDS Runtime Integration requirements verified: 29

Previous Baseline v1.5 regression: 188 PASS / 0 FAIL

Inspection evidence: InspectionVerification.md

Baseline v1.6 is verification-closed for the currently defined VirtualVehicle
requirements.

Baseline Boundary

The following capabilities remain outside Baseline v1.6 and are not claimed as
requirements-verified:

Full ISO 14229 compliance

Full ISO 15765-2 compliance

Parallel diagnostic clients

ISO-TP runtime Flow Control scheduling

ISO-TP Block Size enforcement

ISO-TP STmin timing enforcement

CAN FD ISO-TP

Extended ISO-TP addressing

Functional diagnostic addressing

Security Access

ECU Reset service

Communication Control

Routine Control

Request Download

Transfer Data

ECU reprogramming / flashing

AUTOSAR runtime behavior

Automotive Ethernet

Physical diagnostic hardware

Physical CAN transceiver or hardware-bus validation

Hardware-in-the-loop CAN validation

Future requirements for these capabilities shall be introduced in a subsequent
baseline before implementation is claimed as requirements-verified.

Baseline Status

VirtualVehicle Baseline v1.6

Automated Tests:        217
Passed:                 217
Failed:                   0

Requirements:           227
Verified:               227
Unverified:               0

Status:
VERIFICATION CLOSED

Baseline v1.7 Automotive Ethernet Verification

Baseline v1.7 adds a deterministic virtual Automotive Ethernet communication subsystem while preserving the complete verified v1.6 CAN and diagnostic baseline.

Verified Ethernet capabilities include:

Ethernet frame representation

48-bit MAC addressing and deterministic node addressing

VirtualEthernetBus frame transmission and destination delivery

100 Mbit/s simulated link timing

Deterministic transmission duration and bus busy timing

Ethernet trace evidence for addresses, EtherType, payload, and timing

Frame, payload-byte, and per-node statistics

Reusable EthernetNode abstraction

SimulationEngine Ethernet ownership, runtime processing, and reset

CAN and Ethernet coexistence

Legacy CAN and diagnostic regression preservation

Complete Baseline v1.6 regression preservation

Deterministic Ethernet and system behavior

Ethernet verification result: 37 PASS / 0 FAIL

Complete Baseline v1.6 regression: 217 PASS / 0 FAIL

Baseline v1.7 total automated suite: 254 PASS / 0 FAIL

Baseline v1.7 requirements: 264 / 264 VERIFIED

Status: VERIFICATION CLOSED