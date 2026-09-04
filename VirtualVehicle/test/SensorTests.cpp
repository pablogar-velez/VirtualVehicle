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

TestCase TestRunner::runFrontLeftSensorDropoutTest()
{
    TestCase test;

    test.id =
        "TC_SNS_001";

    test.name =
        "Front Left Sensor Dropout";

    test.description =
        "Verify that a wheel-speed sensor configured "
        "with a Dropout fault produces an invalid reading.";

    test.requirementId =
        "SNS-REQ-005";

    test.precondition =
        "Front-left wheel-speed sensor shall have "
        "a valid physical wheel speed.";

    test.stimulus =
        "Inject SensorFault::Dropout.";

    test.expectedResult =
        "Sensor reading shall be invalid and report 0 km/h.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Sensor setup
    // ==================================================

    WheelSpeedSensor sensor;

    constexpr float physicalSpeedKmh =
        50.0f;

    sensor.setTrueSpeed(
        physicalSpeedKmh
    );

    const WheelSpeedReading normalReading =
        sensor.readSpeed();

    // ==================================================
    // Precondition
    // ==================================================

    if (
        !normalReading.valid ||
        normalReading.speedKmh !=
        physicalSpeedKmh
        )
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Valid sensor reading before fault";

        test.result.actual =
            "Invalid sensor precondition";

        test.result.message =
            "Sensor did not provide the expected "
            "valid reading before fault injection.";

        return test;
    }

    // ==================================================
    // Fault injection
    // ==================================================

    sensor.setFault(
        SensorFault::Dropout
    );

    const WheelSpeedReading dropoutReading =
        sensor.readSpeed();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.expected =
        "valid = false, speed = 0 km/h";

    test.result.actual =
        "valid = " +
        std::string(
            dropoutReading.valid
            ? "true"
            : "false"
        ) +
        ", speed = " +
        std::to_string(
            dropoutReading.speedKmh
        ) +
        " km/h";

    const bool faultConfigured =
        sensor.getFault() ==
        SensorFault::Dropout;

    const bool readingInvalid =
        !dropoutReading.valid;

    const bool speedZero =
        dropoutReading.speedKmh ==
        0.0f;

    if (
        faultConfigured &&
        readingInvalid &&
        speedZero
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Dropout fault produced the expected "
            "invalid wheel-speed measurement.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Dropout behavior did not match "
            "the sensor requirement.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runFrontRightSensorDropoutTest()
{
    TestCase test;

    test.id =
        "TC_SNS_002";

    test.name =
        "Front Right Sensor Dropout";

    test.description =
        "Verify that the front-right wheel-speed sensor "
        "supports Dropout fault injection and produces "
        "an invalid measurement.";

    test.requirementId =
        "SNS-REQ-004";

    test.precondition =
        "Front-right wheel-speed sensor shall provide "
        "a valid measurement before fault injection.";

    test.stimulus =
        "Inject SensorFault::Dropout.";

    test.expectedResult =
        "Sensor shall enter Dropout state and "
        "produce an invalid measurement.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Sensor setup
    // ==================================================

    WheelSpeedSensor sensor;

    constexpr float physicalSpeedKmh =
        65.0f;

    sensor.setTrueSpeed(
        physicalSpeedKmh
    );

    const WheelSpeedReading normalReading =
        sensor.readSpeed();

    // ==================================================
    // Precondition
    // ==================================================

    if (!normalReading.valid)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Valid reading before fault";

        test.result.actual =
            "Invalid reading before fault";

        test.result.message =
            "Sensor precondition failed.";

        return test;
    }

    // ==================================================
    // Fault injection
    // ==================================================

    sensor.setFault(
        SensorFault::Dropout
    );

    const WheelSpeedReading dropoutReading =
        sensor.readSpeed();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.expected =
        "Fault = Dropout, valid = false";

    test.result.actual =
        "Fault configured = " +
        std::string(
            sensor.getFault() ==
            SensorFault::Dropout
            ? "Dropout"
            : "Other"
        ) +
        ", valid = " +
        std::string(
            dropoutReading.valid
            ? "true"
            : "false"
        );

    if (
        sensor.getFault() ==
        SensorFault::Dropout &&
        !dropoutReading.valid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-right Dropout fault injection "
            "behaved as expected.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-right Dropout fault injection failed.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runSensorFaultRecoveryTest()
{
    TestCase test;

    test.id =
        "TC_SNS_003";

    test.name =
        "Sensor Fault Recovery";

    test.description =
        "Verify that clearing an injected wheel-speed "
        "sensor fault restores normal measurement behavior.";

    test.requirementId =
        "SNS-REQ-006";

    test.precondition =
        "Wheel-speed sensor shall contain an active "
        "Dropout fault.";

    test.stimulus =
        "Clear the injected sensor fault.";

    test.expectedResult =
        "Sensor shall return to SensorFault::None "
        "and provide a valid wheel-speed reading.";

    test.result.status =
        TestStatus::Running;

    // ==================================================
    // Sensor setup
    // ==================================================

    WheelSpeedSensor sensor;

    constexpr float physicalSpeedKmh =
        72.5f;

    sensor.setTrueSpeed(
        physicalSpeedKmh
    );

    sensor.readSpeed();

    sensor.setFault(
        SensorFault::Dropout
    );

    const WheelSpeedReading faultyReading =
        sensor.readSpeed();

    // ==================================================
    // Verify fault precondition
    // ==================================================

    if (
        faultyReading.valid ||
        sensor.getFault() !=
        SensorFault::Dropout
        )
    {
        test.result.status =
            TestStatus::Failed;

        test.result.expected =
            "Active Dropout fault";

        test.result.actual =
            "Dropout precondition not established";

        test.result.message =
            "Could not establish sensor fault "
            "before recovery verification.";

        return test;
    }

    // ==================================================
    // Clear fault
    // ==================================================

    sensor.clearFault();

    const WheelSpeedReading recoveredReading =
        sensor.readSpeed();

    // ==================================================
    // Evidence
    // ==================================================

    test.result.expected =
        "Fault = None, valid = true, speed = 72.5 km/h";

    test.result.actual =
        "Fault = " +
        std::string(
            sensor.getFault() ==
            SensorFault::None
            ? "None"
            : "Active"
        ) +
        ", valid = " +
        std::string(
            recoveredReading.valid
            ? "true"
            : "false"
        ) +
        ", speed = " +
        std::to_string(
            recoveredReading.speedKmh
        ) +
        " km/h";

    const bool faultCleared =
        sensor.getFault() ==
        SensorFault::None;

    const bool readingValid =
        recoveredReading.valid;

    const bool speedRecovered =
        recoveredReading.speedKmh ==
        physicalSpeedKmh;

    if (
        faultCleared &&
        readingValid &&
        speedRecovered
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Sensor returned to normal measurement "
            "behavior after fault clearing.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Sensor did not recover correctly "
            "after clearing the fault.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runFrontLeftWheelSpeedMeasurementTest()
{
    TestCase test;

    test.id = "TC_SNS_004";
    test.name = "Front-Left Wheel-Speed Measurement";
    test.description =
        "Verify that the simulated front-left wheel-speed sensor provides "
        "the front-left wheel speed to the ABS subsystem.";
    test.requirementId = "SNS-REQ-001";
    test.precondition =
        "Vehicle shall be moving with no front-left sensor fault.";
    test.stimulus =
        "Run Acceleration and compare VehicleModel front-left wheel speed "
        "with the value observed by the ABS state.";
    test.expectedResult =
        "ABS front-left wheel speed shall match the simulated front-left wheel speed.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        1000.0
    );

    const double expectedWheelSpeed =
        engine.getVehicleModel()
        .getFrontLeftWheelSpeedKmh();

    const AbsState& absState =
        engine.getAbsState();

    const double observedWheelSpeed =
        absState.wheelSpeedFrontLeft;

    constexpr double toleranceKmh = 0.1;

    const bool valid =
        absState.frontLeftSensorValid;

    const bool matches =
        std::fabs(
            observedWheelSpeed -
            expectedWheelSpeed
        ) <= toleranceKmh;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS FL speed=" +
        std::to_string(expectedWheelSpeed) +
        " km/h, valid=true";

    test.result.actual =
        "ABS FL speed=" +
        std::to_string(observedWheelSpeed) +
        " km/h, valid=" +
        std::string(valid ? "true" : "false");

    if (valid && matches)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-left wheel-speed measurement was provided to the ABS subsystem.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-left wheel-speed measurement did not match the simulated wheel speed.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runFrontRightWheelSpeedMeasurementTest()
{
    TestCase test;

    test.id = "TC_SNS_005";
    test.name = "Front-Right Wheel-Speed Measurement";
    test.description =
        "Verify that the simulated front-right wheel-speed sensor provides "
        "the front-right wheel speed to the ABS subsystem.";
    test.requirementId = "SNS-REQ-002";
    test.precondition =
        "Vehicle shall be moving with no front-right sensor fault.";
    test.stimulus =
        "Run Acceleration and compare VehicleModel front-right wheel speed "
        "with the value observed by the ABS state.";
    test.expectedResult =
        "ABS front-right wheel speed shall match the simulated front-right wheel speed.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        1000.0
    );

    const double expectedWheelSpeed =
        engine.getVehicleModel()
        .getFrontRightWheelSpeedKmh();

    const AbsState& absState =
        engine.getAbsState();

    const double observedWheelSpeed =
        absState.wheelSpeedFrontRight;

    constexpr double toleranceKmh = 0.1;

    const bool valid =
        absState.frontRightSensorValid;

    const bool matches =
        std::fabs(
            observedWheelSpeed -
            expectedWheelSpeed
        ) <= toleranceKmh;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS FR speed=" +
        std::to_string(expectedWheelSpeed) +
        " km/h, valid=true";

    test.result.actual =
        "ABS FR speed=" +
        std::to_string(observedWheelSpeed) +
        " km/h, valid=" +
        std::string(valid ? "true" : "false");

    if (valid && matches)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-right wheel-speed measurement was provided to the ABS subsystem.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-right wheel-speed measurement did not match the simulated wheel speed.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runFrontLeftDropoutInjectionTest()
{
    TestCase test;

    test.id = "TC_SNS_006";
    test.name = "Front-Left Dropout Injection";
    test.description =
        "Verify direct support for injecting a Dropout fault into the "
        "front-left wheel-speed sensor.";
    test.requirementId = "SNS-REQ-003";
    test.precondition =
        "Front-left wheel-speed sensor shall initially provide a valid measurement.";
    test.stimulus =
        "Inject SensorFault::Dropout into the front-left wheel-speed sensor during execution.";
    test.expectedResult =
        "The consuming ABS subsystem shall observe the front-left sensor as invalid.";
    test.result.status = TestStatus::Running;

    SimulationEngine engine(LogLevel::Quiet);
    engine.reset();
    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    const bool validBefore =
        engine.getAbsState()
        .frontLeftSensorValid;

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool validAfter =
        engine.getAbsState()
        .frontLeftSensorValid;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Valid before=true, valid after Dropout=false";

    test.result.actual =
        "Valid before=" +
        std::string(validBefore ? "true" : "false") +
        ", valid after=" +
        std::string(validAfter ? "true" : "false");

    if (validBefore && !validAfter)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Front-left Dropout fault injection was supported and observed by ABS.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Front-left Dropout fault injection did not produce the expected invalid input.";
    }

    return test;
}

