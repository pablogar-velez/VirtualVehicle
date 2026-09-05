VirtualVehicle — Automotive Ethernet Requirements

Document Information

Project: VirtualVehicle

Document: Automotive Ethernet Requirements

Baseline: v1.7

Status: Verification Closed

Verification Method: Automated Test unless otherwise specified

Parent Verified Baseline: v1.6

Parent Baseline Result: 217 PASS / 0 FAIL

Parent Baseline Requirements: 227 / 227 VERIFIED

1. Purpose

This document defines the requirements for introducing a simplified Automotive
Ethernet communication subsystem into the VirtualVehicle software-in-the-loop
simulation platform.

The objective of this milestone is to extend the existing virtual vehicle
communication architecture beyond CAN while preserving all behavior verified in
Baseline v1.6.

The Ethernet subsystem shall provide a deterministic software simulation of
Ethernet communication concepts suitable for VirtualVehicle verification and
portfolio demonstration.

The implementation shall not claim full IEEE 802.3, TCP/IP, UDP, SOME/IP, or
AUTOSAR compliance.

2. Baseline Relationship

VirtualVehicle Baseline v1.6 verifies the following major communication and
diagnostic architecture:

SimulationEngine
      |
      +-----------------------------+
      |                             |
      v                             v
Periodic ECUs                 Diagnostic Client
      |                             |
      v                             v
 VirtualCanBus                  UDS / ISO-TP
      |                             |
      +-------------+---------------+
                    |
                    v
              CAN Arbitration
              Timing / Trace
              Statistics

Baseline v1.7 shall preserve this complete architecture.

The Automotive Ethernet subsystem shall be introduced as an additional virtual
communication channel.

The existing VirtualCanBus implementation shall not be replaced by Ethernet.

3. Target Architecture

The target Baseline v1.7 communication architecture shall be:

                         SimulationEngine
                              |
               +--------------+--------------+
               |                             |
               v                             v
        CAN Communication             Ethernet Communication
               |                             |
               v                             v
         VirtualCanBus               VirtualEthernetBus
               |                             |
      +--------+--------+             +------+------+
      |        |        |             |             |
      v        v        v             v             v
    ABS     Powertrain Steering   Ethernet ECU   Ethernet ECU
   0x080      0x100     0x120        Node A         Node B
      |
      v
 UDS / ISO-TP
 0x7E0 / 0x7E8

CAN and Ethernet communication shall coexist within the VirtualVehicle
simulation.

4. Ethernet Frame Requirements
ETH-REQ-001 — Ethernet Frame Representation

The system shall provide a software representation of an Ethernet frame.

The frame representation shall contain sufficient information to support
deterministic virtual Ethernet communication.

Verification Method: Automated Test
Verification Test: TC_ETH_001
Verification Status: VERIFIED / PASS

ETH-REQ-002 — Source MAC Address

Each virtual Ethernet frame shall contain a source MAC address.

Verification Method: Automated Test
Verification Test: TC_ETH_002
Verification Status: VERIFIED / PASS

ETH-REQ-003 — Destination MAC Address

Each virtual Ethernet frame shall contain a destination MAC address.

Verification Method: Automated Test
Verification Test: TC_ETH_003
Verification Status: VERIFIED / PASS

ETH-REQ-004 — EtherType Field

Each virtual Ethernet frame shall contain an EtherType field identifying the
simulated upper-layer protocol.

Verification Method: Automated Test
Verification Test: TC_ETH_004
Verification Status: VERIFIED / PASS

ETH-REQ-005 — Ethernet Payload

Each virtual Ethernet frame shall support a payload represented as a sequence of
bytes.

Verification Method: Automated Test
Verification Test: TC_ETH_005
Verification Status: VERIFIED / PASS

ETH-REQ-006 — Payload Preservation

The virtual Ethernet subsystem shall preserve the transmitted payload from
sender to receiver.

Verification Method: Automated Test
Verification Test: TC_ETH_006
Verification Status: VERIFIED / PASS

5. MAC Address Requirements
ETH-REQ-007 — MAC Address Representation

The system shall provide a deterministic representation of a 48-bit Ethernet MAC
address.

Verification Method: Automated Test
Verification Test: TC_ETH_007
Verification Status: VERIFIED / PASS

ETH-REQ-008 — MAC Address Equality

The system shall support comparison of MAC addresses for Ethernet frame routing.

Verification Method: Automated Test
Verification Test: TC_ETH_008
Verification Status: VERIFIED / PASS

ETH-REQ-009 — Deterministic Node Addressing

Each configured virtual Ethernet node shall use a deterministic MAC address.

Verification Method: Automated Test
Verification Test: TC_ETH_009
Verification Status: VERIFIED / PASS

6. Virtual Ethernet Bus Requirements
ETH-REQ-010 — Virtual Ethernet Bus Component

The system shall provide a VirtualEthernetBus component for transmitting
Ethernet frames between virtual nodes.

Verification Method: Automated Test
Verification Test: TC_ETH_010
Verification Status: VERIFIED / PASS

ETH-REQ-011 — Frame Transmission Request

Virtual Ethernet nodes shall be able to submit Ethernet frames to
VirtualEthernetBus for transmission.

Verification Method: Automated Test
Verification Test: TC_ETH_011
Verification Status: VERIFIED / PASS

ETH-REQ-012 — Frame Delivery

VirtualEthernetBus shall deliver transmitted Ethernet frames to the destination
defined by the frame destination MAC address.

Verification Method: Automated Test
Verification Test: TC_ETH_012
Verification Status: VERIFIED / PASS

ETH-REQ-013 — Unknown Destination Behavior

A frame addressed to an unregistered destination MAC address shall not be
delivered to an unrelated virtual Ethernet node.

Verification Method: Automated Test
Verification Test: TC_ETH_013
Verification Status: VERIFIED / PASS

ETH-REQ-014 — Multiple Ethernet Nodes

VirtualEthernetBus shall support more than one registered virtual Ethernet node.

Verification Method: Automated Test
Verification Test: TC_ETH_014
Verification Status: VERIFIED / PASS

7. Ethernet Timing Requirements
ETH-REQ-015 — Configurable Ethernet Link Rate

The VirtualEthernetBus shall use a configured simulated link rate for
transmission-time calculation.

The initial Baseline v1.7 implementation shall use:

100 Mbit/s

Verification Method: Automated Test
Verification Test: TC_ETH_015
Verification Status: VERIFIED / PASS

ETH-REQ-016 — Ethernet Transmission Duration

The subsystem shall calculate a deterministic simulated transmission duration
for each transmitted Ethernet frame.

Verification Method: Automated Test
Verification Test: TC_ETH_016
Verification Status: VERIFIED / PASS

ETH-REQ-017 — Ethernet Bus Time Progression

Ethernet frame transmission shall advance the VirtualEthernetBus transmission
time according to the calculated frame duration.

Verification Method: Automated Test
Verification Test: TC_ETH_017
Verification Status: VERIFIED / PASS

ETH-REQ-018 — Ethernet Busy State

VirtualEthernetBus shall expose sufficient timing state to determine when the
current simulated Ethernet transmission completes.

Verification Method: Automated Test
Verification Test: TC_ETH_018
Verification Status: VERIFIED / PASS

8. Ethernet Trace Requirements
ETH-REQ-019 — Ethernet Trace Recording

VirtualEthernetBus shall maintain a trace of transmitted Ethernet frames.

Verification Method: Automated Test
Verification Test: TC_ETH_019
Verification Status: VERIFIED / PASS

ETH-REQ-020 — Ethernet Trace Address Evidence

Each Ethernet trace entry shall contain source and destination MAC address
evidence.

Verification Method: Automated Test
Verification Test: TC_ETH_020
Verification Status: VERIFIED / PASS

ETH-REQ-021 — Ethernet Trace EtherType Evidence

Each Ethernet trace entry shall contain the transmitted EtherType.

Verification Method: Automated Test
Verification Test: TC_ETH_021
Verification Status: VERIFIED / PASS

ETH-REQ-022 — Ethernet Trace Payload Evidence

Each Ethernet trace entry shall preserve sufficient payload evidence for
verification of the transmitted data.

Verification Method: Automated Test
Verification Test: TC_ETH_022
Verification Status: VERIFIED / PASS

ETH-REQ-023 — Ethernet Trace Timing Evidence

Each Ethernet trace entry shall contain timing information sufficient to
determine transmission start and completion.

Verification Method: Automated Test
Verification Test: TC_ETH_023
Verification Status: VERIFIED / PASS

9. Ethernet Statistics Requirements
ETH-REQ-024 — Ethernet Frame Count

The subsystem shall maintain a count of successfully transmitted Ethernet
frames.

Verification Method: Automated Test
Verification Test: TC_ETH_024
Verification Status: VERIFIED / PASS

ETH-REQ-025 — Ethernet Transmitted Byte Count

The subsystem shall maintain a count of Ethernet payload bytes transmitted
through VirtualEthernetBus.

Verification Method: Automated Test
Verification Test: TC_ETH_025
Verification Status: VERIFIED / PASS

ETH-REQ-026 — Per-Node Ethernet Statistics

The subsystem shall provide sufficient statistics to determine Ethernet
transmission activity associated with individual configured nodes.

Verification Method: Automated Test
Verification Test: TC_ETH_026
Verification Status: VERIFIED / PASS

10. Ethernet ECU Requirements
ETH-REQ-027 — Virtual Ethernet Node

The system shall provide a reusable virtual Ethernet node or ECU abstraction
capable of transmitting and receiving Ethernet frames.

Verification Method: Automated Test
Verification Test: TC_ETH_027
Verification Status: VERIFIED / PASS

ETH-REQ-028 — Ethernet Node Receive Filtering

A virtual Ethernet node shall accept frames addressed to its configured MAC
address.

Verification Method: Automated Test
Verification Test: TC_ETH_028
Verification Status: VERIFIED / PASS

ETH-REQ-029 — Ethernet Node Transmission

A virtual Ethernet node shall be able to construct and transmit a frame using
its configured source MAC address.

Verification Method: Automated Test
Verification Test: TC_ETH_029
Verification Status: VERIFIED / PASS

11. SimulationEngine Integration Requirements
ETH-REQ-030 — SimulationEngine Ethernet Ownership

SimulationEngine shall own or control the VirtualEthernetBus used by the active
simulation.

Verification Method: Automated Test
Verification Test: TC_ETH_030
Verification Status: VERIFIED / PASS

ETH-REQ-031 — CAN and Ethernet Coexistence

SimulationEngine shall support CAN and Ethernet communication within the same
simulation instance.

Ethernet integration shall not replace or disable the existing VirtualCanBus.

Verification Method: Automated Test
Verification Test: TC_ETH_031
Verification Status: VERIFIED / PASS

ETH-REQ-032 — Ethernet Runtime Processing

SimulationEngine shall provide runtime processing for pending virtual Ethernet
communication.

Verification Method: Automated Test
Verification Test: TC_ETH_032
Verification Status: VERIFIED / PASS

ETH-REQ-033 — Ethernet Runtime Reset

SimulationEngine reset shall clear the runtime state of the virtual Ethernet
subsystem.

Verification Method: Automated Test
Verification Test: TC_ETH_033
Verification Status: VERIFIED / PASS

12. Regression Requirements
ETH-REQ-034 — Existing CAN Regression

Automotive Ethernet integration shall preserve the existing verified
VirtualCanBus behavior.

Verification Method: Automated Test
Verification Test: TC_ETH_034
Verification Status: VERIFIED / PASS

ETH-REQ-035 — Existing Diagnostic Regression

Automotive Ethernet integration shall preserve the existing verified UDS,
ISO-TP, UDS Transport, UDS CAN Transport, and UDS Runtime Integration behavior.

Verification Method: Automated Test
Verification Test: TC_ETH_035
Verification Status: VERIFIED / PASS

ETH-REQ-036 — Baseline v1.6 Regression

The complete Baseline v1.6 automated validation suite shall continue to pass
after Automotive Ethernet integration.

The required regression result shall remain:

Previous Tests: 217
Required PASS:  217
Required FAIL:    0

Verification Method: Automated Regression Test
Verification Test: Complete Baseline v1.6 suite
Verification Status: VERIFIED / PASS

13. Determinism Requirement
ETH-REQ-037 — Deterministic Ethernet Communication

For identical initial Ethernet state, node configuration, frame contents,
simulation time, and transmission sequence, the Ethernet subsystem shall produce
equivalent frame delivery, timing, trace, and statistics results.

Verification Method: Automated Test
Verification Test: TC_ETH_037
Verification Status: VERIFIED / PASS

14. Verification Set
Test Case	Primary Requirement	Verification Objective
TC_ETH_001	ETH-REQ-001	Ethernet frame representation
TC_ETH_002	ETH-REQ-002	Source MAC address
TC_ETH_003	ETH-REQ-003	Destination MAC address
TC_ETH_004	ETH-REQ-004	EtherType representation
TC_ETH_005	ETH-REQ-005	Ethernet payload
TC_ETH_006	ETH-REQ-006	Payload preservation
TC_ETH_007	ETH-REQ-007	MAC address representation
TC_ETH_008	ETH-REQ-008	MAC address comparison
TC_ETH_009	ETH-REQ-009	Deterministic node addressing
TC_ETH_010	ETH-REQ-010	VirtualEthernetBus component
TC_ETH_011	ETH-REQ-011	Frame transmission request
TC_ETH_012	ETH-REQ-012	Frame delivery
TC_ETH_013	ETH-REQ-013	Unknown destination behavior
TC_ETH_014	ETH-REQ-014	Multiple Ethernet nodes
TC_ETH_015	ETH-REQ-015	Configurable link rate
TC_ETH_016	ETH-REQ-016	Ethernet transmission duration
TC_ETH_017	ETH-REQ-017	Ethernet bus time progression
TC_ETH_018	ETH-REQ-018	Ethernet busy state
TC_ETH_019	ETH-REQ-019	Ethernet trace recording
TC_ETH_020	ETH-REQ-020	Ethernet trace address evidence
TC_ETH_021	ETH-REQ-021	Ethernet trace EtherType evidence
TC_ETH_022	ETH-REQ-022	Ethernet trace payload evidence
TC_ETH_023	ETH-REQ-023	Ethernet trace timing evidence
TC_ETH_024	ETH-REQ-024	Ethernet frame count
TC_ETH_025	ETH-REQ-025	Ethernet byte count
TC_ETH_026	ETH-REQ-026	Per-node Ethernet statistics
TC_ETH_027	ETH-REQ-027	Virtual Ethernet node
TC_ETH_028	ETH-REQ-028	Ethernet receive filtering
TC_ETH_029	ETH-REQ-029	Ethernet node transmission
TC_ETH_030	ETH-REQ-030	SimulationEngine Ethernet ownership
TC_ETH_031	ETH-REQ-031	CAN and Ethernet coexistence
TC_ETH_032	ETH-REQ-032	Ethernet runtime processing
TC_ETH_033	ETH-REQ-033	Ethernet runtime reset
TC_ETH_034	ETH-REQ-034	Existing CAN regression
TC_ETH_035	ETH-REQ-035	Existing diagnostic regression
TC_ETH_036	ETH-REQ-036	Full Baseline v1.6 regression
TC_ETH_037	ETH-REQ-037	Deterministic Ethernet communication

Dedicated tests executed: 37

Result: 37 PASS / 0 FAIL

15. Baseline v1.7 Closure

Baseline v1.6 remains unchanged and verification-closed:

Baseline v1.6

Requirements: 227
Verified:     227

Tests:        217
Passed:       217
Failed:         0

The Ethernet requirements defined in this document shall not be added to the
verified baseline total until implementation and verification are complete.

All 37 Ethernet requirements were implemented and successfully verified.
The Baseline v1.7 totals are:

Previous Requirements: 227
New Requirements:       37
Total Requirements:    264

Previous Tests:         217
New Tests:               37
Total Tests:            254

The Baseline v1.7 verification target will therefore be:

Automated Tests: 254
Required PASS:   254
Required FAIL:     0

Requirements:    264
Required Verified:
                 264

These totals were achieved by successful execution of the complete Baseline v1.7 automated verification suite.

16. Verification Boundary

The following capabilities remain outside this Automotive Ethernet milestone:

Full IEEE 802.3 compliance
Physical Ethernet PHY behavior
Ethernet preamble modeling
Start Frame Delimiter modeling
Ethernet FCS / CRC hardware behavior
Inter-packet gap hardware timing accuracy
Ethernet switching hardware
VLAN / IEEE 802.1Q
ARP
IPv4
IPv6
ICMP
TCP
Full UDP protocol implementation
Full SOME/IP implementation
SOME/IP Service Discovery
DoIP
Time-Sensitive Networking
Precision Time Protocol
AUTOSAR Ethernet stack
Ethernet security
Physical Ethernet hardware
Hardware-in-the-loop Ethernet validation

Future requirement sets shall be created before any of these capabilities are
claimed as formally verified.

17. Current Status
Automotive Ethernet

Requirements Defined: 37
Implemented:          37
Verified:             37

Dedicated Tests Executed:
37 PASS / 0 FAIL

Parent Baseline:
v1.6 — VERIFICATION CLOSED

Milestone Status:
VERIFICATION CLOSED