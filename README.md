VirtualVehicle

C++20 Software-in-the-Loop Automotive Simulation Platform

VirtualVehicle is a deterministic automotive software simulation platform built in C++20 with Qt 6. It models vehicle ECUs, CAN communication, diagnostic services, ISO-TP transport, DTC handling, fault injection, and simplified Automotive Ethernet in a single desktop application.

The project is designed as a software-only portfolio environment for embedded systems, automotive communication, verification, and validation concepts.

Verification Status

Baseline: v1.7
Status: VERIFICATION CLOSED

Metric

Result

Automated tests

254 / 254 PASS

Requirements verified

264 / 264

Automotive Ethernet requirements

37 / 37 VERIFIED

v1.6 regression baseline

217 / 217 PASS

Key Features

Deterministic virtual CAN bus

CAN arbitration and timing

CAN message encoding and decoding

Powertrain, ABS, Steering, and Dashboard ECU simulation

Vehicle dynamics and wheel-speed sensor simulation

Sensor fault injection

Diagnostic Trouble Code management

UDS diagnostic services

ISO-TP segmentation and reassembly

UDS transport over the shared virtual CAN bus

Simplified Automotive Ethernet

Virtual Ethernet nodes and MAC addressing

Ethernet timing, trace, and statistics

CAN and Ethernet coexistence

Qt-based runtime GUI

Event logging, CAN monitoring, and runtime statistics

Integrated automated test runner

Requirements-based verification and traceability

Architecture

                         +----------------------+
                         |   SimulationEngine   |
                         +----------+-----------+
                                    |
             +----------------------+----------------------+
             |                                             |
             v                                             v
    +--------------------+                        +--------------------+
    |   Virtual CAN Bus  |                        | Virtual Ethernet   |
    +---------+----------+                        |        Bus         |
              |                                   +---------+----------+
      +-------+--------+                                    |
      |       |        |                              +-----+-----+
      v       v        v                              |           |
  +-------+ +------+ +----------+                 +-------+   +-------+
  |  ABS  | | P/T  | | Steering |                 |Node A |   |Node B |
  |  ECU  | | ECU  | |   ECU    |                 +-------+   +-------+
  +---+---+ +--+---+ +-----+----+
      |        |           |
      +--------+-----------+
               |
               v
       +---------------+
       | Dashboard ECU |
       +---------------+

               CAN Diagnostics
                     |
                     v
              +-------------+
              |     UDS     |
              +------+------+
                     |
                     v
              +-------------+
              |   ISO-TP    |
              +-------------+

The SimulationEngine coordinates the vehicle model, ECUs, virtual communication buses, diagnostics, scenarios, logging, and runtime processing.

Project Structure

VirtualVehicle/
├── requirements/
│
├── VirtualVehicle/
│   ├── can/
│   ├── diagnostics/
│   ├── ecu/
│   ├── ethernet/
│   ├── gui/
│   │   └── widgets/
│   ├── sensor/
│   ├── simulation/
│   ├── test/
│   ├── transport/
│   ├── CMakeLists.txt
│   ├── VirtualVehicle.cpp
│   └── VirtualVehicle.h
│
├── CMakeLists.txt
├── CMakePresets.json
├── .gitignore
└── .gitattributes

Technology Stack

Language: C++20

GUI: Qt 6 Widgets

Build system: CMake

IDE: Visual Studio

Compiler toolchain: MSVC

Current Qt configuration: Qt 6.11.2 / MSVC 2022 64-bit

Build Requirements

The current project configuration expects:

Windows

Visual Studio with C++ development tools

CMake 3.10 or later

Qt 6 Widgets

Qt MSVC 2022 64-bit kit

The root CMakeLists.txt currently defines the Qt installation path as:

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.2/msvc2022_64")

If Qt is installed somewhere else, update that path before configuring the project.

Build

From the repository root:

cmake -S . -B out/build
cmake --build out/build --config Debug

For a release build:

cmake -S . -B out/build
cmake --build out/build --config Release

The project can also be opened directly as a CMake project in Visual Studio.

Run

After building, launch the generated VirtualVehicle executable from the selected build configuration.

The Qt GUI provides runtime visibility into:

vehicle state

ABS state

scenario control

fault injection

CAN traffic

event logs

runtime statistics

automated verification tests

CAN Simulation

The virtual CAN subsystem includes:

CAN frame representation

deterministic arbitration

transmission timing

message registry

payload codecs

trace recording

statistics

shared-bus ECU communication

Lower arbitration IDs receive higher priority during arbitration.

Representative communication includes:

ABS ECU          -> CAN ID 0x080
Powertrain ECU   -> CAN ID 0x100
Steering ECU     -> CAN ID 0x120
UDS Request      -> CAN ID 0x7E0
UDS Response     -> CAN ID 0x7E8

Diagnostics

The diagnostic subsystem models software-level automotive diagnostic behavior including:

Diagnostic Trouble Codes

DTC status handling

UDS requests and responses

UDS server behavior

ISO-TP transport

UDS over the shared virtual CAN bus

runtime request/response processing

Diagnostic communication participates in the same CAN arbitration path as normal ECU traffic.

Automotive Ethernet

Baseline v1.7 adds a simplified deterministic Automotive Ethernet model.

Implemented concepts include:

48-bit MAC addressing

Ethernet frame representation

source and destination addressing

EtherType

payload preservation

virtual node registration

deterministic frame delivery

configurable link rate

simplified transmission timing

bus busy time

trace evidence

Ethernet statistics

per-node statistics

runtime integration

CAN/Ethernet coexistence

reset behavior

deterministic execution

The baseline link rate is 100 Mbit/s.

This is a software simulation model and is not intended to claim full IEEE 802.3 conformance.

Verification Approach

VirtualVehicle uses requirements-based automated verification.

Final v1.7 result:

Automated Tests:          254
Passed:                   254
Failed:                     0

Requirements:             264
Verified:                 264
Unverified:                 0

Automotive Ethernet:
Requirements:              37
Verified:                  37
Tests:                     37
Passed:                    37

v1.6 Regression:
Passed:                   217
Failed:                     0

Verification Documentation

The repository includes formal project documentation covering:

system requirements

Automotive Ethernet requirements

requirements-to-test traceability

inspection verification

verification closure evidence

The current verified baseline is v1.7.

Scope

VirtualVehicle focuses on software-level simulation and verification.

The project does not claim implementation of:

physical CAN transceivers

physical Ethernet PHY behavior

full IEEE 802.3 conformance

VLAN

TSN

AVB

TCP/IP

UDP/IP

SOME/IP

DoIP

AUTOSAR runtime

real automotive hardware

HIL execution

These technologies are intentionally outside the v1.7 baseline.

Project Goal

The purpose of VirtualVehicle is to demonstrate practical experience with:

embedded C++ architecture

automotive communication concepts

deterministic simulation

ECU interaction

diagnostics

verification and validation

requirements traceability

fault injection

automated testing

software integration

The project intentionally prioritizes clear architecture, deterministic behavior, verification evidence, and maintainability over physical hardware emulation.

Current Release

VirtualVehicle v1.7 — Automotive Ethernet

254 / 254 automated tests PASS
264 / 264 requirements VERIFIED

VERIFICATION CLOSED