#include "TestRunner.h"
#include "TestConfig.h"

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include "../simulation/SimulationEngine.h"
#include "../sensor/WheelSpeedSensor.h"
#include "../can/VirtualCanBus.h"
#include "../can/CanMessageDefinitions.h"
#include "../can/CanTiming.h"
#include "../can/VehicleStateCodec.h"
#include "../can/SteeringStateCodec.h"
#include "../can/AbsStateCodec.h"

#include "../ecu/PowertrainEcu.h"
#include "../ecu/SteeringEcu.h"

using namespace TestConfig;

// ==================================================

TestCase TestRunner::runAbsCodecRoundtripTest()
{
    TestCase test;

    test.id =
        "TC_MSG_001";

    test.name =
        "ABS Codec Roundtrip";

    test.description =
        "Verify that ABS_WHEEL_STATE encode/decode "
        "preserves wheel speeds and ABS status.";

    test.requirementId =
        "MSG-ABS-REQ-003";

    test.precondition =
        "ABS state shall contain valid signal values.";

    test.stimulus =
        "Encode and decode ABS state.";

    test.expectedResult =
        "Decoded ABS state shall match encoded state.";

    test.result.status =
        TestStatus::Running;

    AbsState original{};

    original.wheelSpeedFrontLeft =
        58.4f;

    original.wheelSpeedFrontRight =
        59.1f;

    original.absActive =
        true;

    original.frontLeftSensorValid =
        true;

    original.frontRightSensorValid =
        false;

    original.healthStatus =
        AbsHealthStatus::Degraded;

    const CanFrame frame =
        AbsStateCodec::encode(
            original
        );

    const AbsState decoded =
        AbsStateCodec::decode(
            frame
        );

    const bool frontLeftMatches =
        decoded.wheelSpeedFrontLeft ==
        original.wheelSpeedFrontLeft;

    const bool frontRightMatches =
        decoded.wheelSpeedFrontRight ==
        original.wheelSpeedFrontRight;

    const bool absMatches =
        decoded.absActive ==
        original.absActive;

    const bool flValidMatches =
        decoded.frontLeftSensorValid ==
        original.frontLeftSensorValid;

    const bool frValidMatches =
        decoded.frontRightSensorValid ==
        original.frontRightSensorValid;

    const bool healthMatches =
        decoded.healthStatus ==
        original.healthStatus;

    test.result.expected =
        "ABS signals preserved";

    test.result.actual =
        "FL=" +
        std::to_string(
            decoded.wheelSpeedFrontLeft
        ) +
        ", FR=" +
        std::to_string(
            decoded.wheelSpeedFrontRight
        ) +
        ", ABS=" +
        std::string(
            decoded.absActive
            ? "true"
            : "false"
        );

    if (
        frontLeftMatches &&
        frontRightMatches &&
        absMatches &&
        flValidMatches &&
        frValidMatches &&
        healthMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "ABS_WHEEL_STATE codec preserved "
            "all defined signals.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "ABS_WHEEL_STATE codec roundtrip mismatch.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runAbsMessageIdentifierTest()
{
    TestCase test;

    test.id = "TC_MSG_002";
    test.name = "ABS Message CAN Identifier";
    test.description =
        "Verify that ABS_WHEEL_STATE encoding uses CAN identifier 0x080.";
    test.requirementId = "MSG-ABS-REQ-001";
    test.precondition = "A valid ABS state shall be available.";
    test.stimulus = "Encode an ABS_WHEEL_STATE CAN frame.";
    test.expectedResult = "Encoded frame shall use CAN ID 0x080.";
    test.result.status = TestStatus::Running;

    AbsState state{};
    state.wheelSpeedFrontLeft = 50.0f;
    state.wheelSpeedFrontRight = 50.5f;
    state.absActive = false;
    state.frontLeftSensorValid = true;
    state.frontRightSensorValid = true;
    state.healthStatus = AbsHealthStatus::Healthy;

    const CanFrame frame =
        AbsStateCodec::encode(state);

    test.result.expected = "CAN ID = 0x080";
    test.result.actual =
        frame.arbitrationId == CanMessageDefinitions::ABS_WHEEL_STATE.id
        ? "CAN ID = 0x080"
        : "Unexpected CAN ID";

    if (frame.arbitrationId == CanMessageDefinitions::ABS_WHEEL_STATE.id)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS_WHEEL_STATE encoding used the required CAN identifier.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "ABS_WHEEL_STATE encoding used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runAbsMessageStateInformationTest()
{
    TestCase test;

    test.id = "TC_MSG_003";
    test.name = "ABS Message State Information";
    test.description =
        "Verify that ABS_WHEEL_STATE contains the defined simulated "
        "wheel-speed, intervention, validity, and health information.";
    test.requirementId = "MSG-ABS-REQ-002";
    test.precondition = "ABS state shall contain known test values.";
    test.stimulus = "Encode and decode ABS_WHEEL_STATE and inspect defined state information.";
    test.expectedResult =
        "Decoded ABS state information shall match the values represented in the message.";
    test.result.status = TestStatus::Running;

    AbsState original{};
    original.wheelSpeedFrontLeft = 62.4f;
    original.wheelSpeedFrontRight = 61.8f;
    original.absActive = true;
    original.frontLeftSensorValid = true;
    original.frontRightSensorValid = false;
    original.healthStatus = AbsHealthStatus::Degraded;

    const CanFrame frame =
        AbsStateCodec::encode(original);

    const AbsState decoded =
        AbsStateCodec::decode(frame);

    constexpr double wheelSpeedResolutionKmh = 0.1;

    const bool frontLeftPresent =
        std::fabs(
            decoded.wheelSpeedFrontLeft -
            original.wheelSpeedFrontLeft
        ) <= wheelSpeedResolutionKmh;

    const bool frontRightPresent =
        std::fabs(
            decoded.wheelSpeedFrontRight -
            original.wheelSpeedFrontRight
        ) <= wheelSpeedResolutionKmh;

    const bool activePresent =
        decoded.absActive == original.absActive;

    const bool frontLeftValidityPresent =
        decoded.frontLeftSensorValid == original.frontLeftSensorValid;

    const bool frontRightValidityPresent =
        decoded.frontRightSensorValid == original.frontRightSensorValid;

    const bool healthPresent =
        decoded.healthStatus == original.healthStatus;

    test.result.expected =
        "FL=62.4, FR=61.8, ABS=true, FLValid=true, FRValid=false, Health=DEGRADED";

    test.result.actual =
        "FL=" + std::to_string(decoded.wheelSpeedFrontLeft) +
        ", FR=" + std::to_string(decoded.wheelSpeedFrontRight) +
        ", ABS=" + std::string(decoded.absActive ? "true" : "false") +
        ", FLValid=" + std::string(decoded.frontLeftSensorValid ? "true" : "false") +
        ", FRValid=" + std::string(decoded.frontRightSensorValid ? "true" : "false") +
        ", Health=" +
        std::string(
            decoded.healthStatus == AbsHealthStatus::Degraded
            ? "DEGRADED"
            : "HEALTHY"
        );

    if (
        frontLeftPresent &&
        frontRightPresent &&
        activePresent &&
        frontLeftValidityPresent &&
        frontRightValidityPresent &&
        healthPresent
        )
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "ABS_WHEEL_STATE contained all defined simulated ABS and wheel-state information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "ABS_WHEEL_STATE did not preserve all required state information.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runPowertrainMessageIdentifierTest()
{
    TestCase test;

    test.id = "TC_MSG_004";
    test.name = "Powertrain Message CAN Identifier";
    test.description =
        "Verify that POWERTRAIN_STATE encoding uses CAN identifier 0x100.";
    test.requirementId = "MSG-PT-REQ-001";
    test.precondition = "A valid vehicle state shall be available.";
    test.stimulus = "Encode a POWERTRAIN_STATE CAN frame.";
    test.expectedResult = "Encoded frame shall use CAN ID 0x100.";
    test.result.status = TestStatus::Running;

    VehicleState state{};
    state.speedKmh = 72.0f;
    state.engineRpm = 2200;
    state.gear = 3;
    state.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(state);

    test.result.expected = "CAN ID = 0x100";
    test.result.actual =
        frame.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id
        ? "CAN ID = 0x100"
        : "Unexpected CAN ID";

    if (frame.arbitrationId == CanMessageDefinitions::POWERTRAIN_STATE.id)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE encoding used the required CAN identifier.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE encoding used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runPowertrainMessageVehicleSpeedTest()
{
    TestCase test;

    test.id = "TC_MSG_005";
    test.name = "Powertrain Vehicle-Speed Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains vehicle-speed information.";
    test.requirementId = "MSG-PT-REQ-002";
    test.precondition = "Vehicle state shall contain a known vehicle speed.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect vehicle speed.";
    test.expectedResult =
        "Decoded vehicle speed shall match the represented value within message resolution.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 82.5f;
    original.engineRpm = 2140;
    original.gear = 3;
    original.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    constexpr double speedResolutionKmh = 0.1;

    const bool matches =
        std::fabs(decoded.speedKmh - original.speedKmh) <=
        speedResolutionKmh;

    test.result.expected = "Vehicle speed = 82.5 km/h";
    test.result.actual =
        "Vehicle speed = " + std::to_string(decoded.speedKmh) + " km/h";

    if (matches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required vehicle-speed information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE vehicle-speed information was not preserved.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runPowertrainMessageEngineRpmTest()
{
    TestCase test;

    test.id = "TC_MSG_006";
    test.name = "Powertrain Engine-RPM Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains engine-RPM information.";
    test.requirementId = "MSG-PT-REQ-003";
    test.precondition = "Vehicle state shall contain a known engine RPM.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect engine RPM.";
    test.expectedResult = "Decoded engine RPM shall equal the represented value.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 82.5f;
    original.engineRpm = 2140;
    original.gear = 3;
    original.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    test.result.expected = "Engine RPM = 2140";
    test.result.actual =
        "Engine RPM = " + std::to_string(decoded.engineRpm);

    if (decoded.engineRpm == original.engineRpm)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required engine-RPM information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE engine-RPM information was not preserved.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runPowertrainMessageGearStateTest()
{
    TestCase test;

    test.id = "TC_MSG_007";
    test.name = "Powertrain Gear-State Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains gear-state information.";
    test.requirementId = "MSG-PT-REQ-004";
    test.precondition = "Vehicle state shall contain a known gear.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect gear state.";
    test.expectedResult = "Decoded gear shall equal the represented gear.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 82.5f;
    original.engineRpm = 2140;
    original.gear = 4;
    original.brakeApplied = false;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    test.result.expected = "Gear = 4";
    test.result.actual =
        "Gear = " +
        std::to_string(
            static_cast<int>(decoded.gear)
        );

    if (decoded.gear == original.gear)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required gear-state information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE gear-state information was not preserved.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runPowertrainMessageBrakeStateTest()
{
    TestCase test;

    test.id = "TC_MSG_008";
    test.name = "Powertrain Brake-State Information";
    test.description =
        "Verify that POWERTRAIN_STATE contains brake-state information.";
    test.requirementId = "MSG-PT-REQ-005";
    test.precondition = "Vehicle state shall contain brakeApplied=true.";
    test.stimulus = "Encode and decode POWERTRAIN_STATE and inspect brake state.";
    test.expectedResult = "Decoded brake state shall remain applied.";
    test.result.status = TestStatus::Running;

    VehicleState original{};
    original.speedKmh = 45.0f;
    original.engineRpm = 1600;
    original.gear = 2;
    original.brakeApplied = true;

    const CanFrame frame =
        VehicleStateCodec::encode(original);

    const VehicleState decoded =
        VehicleStateCodec::decode(frame);

    test.result.expected = "Brake applied = true";
    test.result.actual =
        std::string("Brake applied = ") +
        (decoded.brakeApplied ? "true" : "false");

    if (decoded.brakeApplied == original.brakeApplied)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "POWERTRAIN_STATE contained the required brake-state information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "POWERTRAIN_STATE brake-state information was not preserved.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runSteeringMessageIdentifierTest()
{
    TestCase test;

    test.id = "TC_MSG_009";
    test.name = "Steering Message CAN Identifier";
    test.description =
        "Verify that STEERING_STATE encoding uses CAN identifier 0x120.";
    test.requirementId = "MSG-STR-REQ-001";
    test.precondition = "A valid steering state shall be available.";
    test.stimulus = "Encode a STEERING_STATE CAN frame.";
    test.expectedResult = "Encoded frame shall use CAN ID 0x120.";
    test.result.status = TestStatus::Running;

    SteeringState state{};
    state.steeringAngleDeg = 3.2f;
    state.steeringTorqueNm = 1.45f;

    const CanFrame frame =
        SteeringStateCodec::encode(state);

    test.result.expected = "CAN ID = 0x120";
    test.result.actual =
        frame.arbitrationId == CanMessageDefinitions::STEERING_STATE.id
        ? "CAN ID = 0x120"
        : "Unexpected CAN ID";

    if (frame.arbitrationId == CanMessageDefinitions::STEERING_STATE.id)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "STEERING_STATE encoding used the required CAN identifier.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "STEERING_STATE encoding used an unexpected CAN identifier.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runSteeringMessageAngleTest()
{
    TestCase test;

    test.id = "TC_MSG_010";
    test.name = "Steering-Angle Information";
    test.description =
        "Verify that STEERING_STATE contains steering-angle information.";
    test.requirementId = "MSG-STR-REQ-002";
    test.precondition = "Steering state shall contain a known steering angle.";
    test.stimulus = "Encode and decode STEERING_STATE and inspect steering angle.";
    test.expectedResult =
        "Decoded steering angle shall match the represented value within message resolution.";
    test.result.status = TestStatus::Running;

    SteeringState original{};
    original.steeringAngleDeg = -12.7f;
    original.steeringTorqueNm = 2.25f;

    const CanFrame frame =
        SteeringStateCodec::encode(original);

    const SteeringState decoded =
        SteeringStateCodec::decode(frame);

    constexpr double angleResolutionDeg = 0.1;

    const bool matches =
        std::fabs(
            decoded.steeringAngleDeg -
            original.steeringAngleDeg
        ) <= angleResolutionDeg;

    test.result.expected = "Steering angle = -12.7 deg";
    test.result.actual =
        "Steering angle = " +
        std::to_string(decoded.steeringAngleDeg) +
        " deg";

    if (matches)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "STEERING_STATE contained the required steering-angle information.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "STEERING_STATE steering-angle information was not preserved.";
    }

    return test;
}

