#include "TestRunner.h"

#include <cmath>
#include <string>

#include "../diagnostics/Dtc.h"
#include "../diagnostics/DtcDefinitions.h"
#include "../diagnostics/DtcManager.h"

#include "../simulation/SimulationEngine.h"
#include "../simulation/LogLevel.h"

#include "../sensor/SensorFault.h"

// ==================================================
// TC_DTC_001
// Front-Left DTC Definition
// ==================================================

TestCase TestRunner::runFrontLeftDtcDefinitionTest()
{
    TestCase test;

    test.id = "TC_DTC_001";
    test.name = "Front-Left DTC Definition";
    test.description =
        "Verify definition and registration of the "
        "front-left wheel-speed sensor DTC.";

    test.requirementId = "DTC-REQ-001";

    test.precondition =
        "DtcManager shall be initialized.";

    test.stimulus =
        "Query DTC code 0xC0031.";

    test.expectedResult =
        "DTC 0xC0031 shall exist.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        DtcDefinitions::
        FrontLeftWheelSpeedSensor ==
        0xC0031 &&
        dtc != nullptr &&
        dtc->code == 0xC0031;

    test.result.expected =
        "Registered DTC = 0xC0031";

    test.result.actual =
        valid
        ? "Registered DTC = 0xC0031"
        : "Front-left DTC definition mismatch";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Front-left wheel-speed sensor DTC is correctly defined."
        : "Front-left wheel-speed sensor DTC is not correctly defined.";

    return test;
}

// ==================================================
// TC_DTC_002
// Front-Right DTC Definition
// ==================================================

TestCase TestRunner::runFrontRightDtcDefinitionTest()
{
    TestCase test;

    test.id = "TC_DTC_002";
    test.name = "Front-Right DTC Definition";
    test.description =
        "Verify definition and registration of the "
        "front-right wheel-speed sensor DTC.";

    test.requirementId = "DTC-REQ-002";

    test.precondition =
        "DtcManager shall be initialized.";

    test.stimulus =
        "Query DTC code 0xC0034.";

    test.expectedResult =
        "DTC 0xC0034 shall exist.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::
            FrontRightWheelSpeedSensor
        );

    const bool valid =
        DtcDefinitions::
        FrontRightWheelSpeedSensor ==
        0xC0034 &&
        dtc != nullptr &&
        dtc->code == 0xC0034;

    test.result.expected =
        "Registered DTC = 0xC0034";

    test.result.actual =
        valid
        ? "Registered DTC = 0xC0034"
        : "Front-right DTC definition mismatch";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Front-right wheel-speed sensor DTC is correctly defined."
        : "Front-right wheel-speed sensor DTC is not correctly defined.";

    return test;
}

// ==================================================
// TC_DTC_003
// Front-Left DTC Activation
// ==================================================

TestCase TestRunner::runFrontLeftDtcActivationTest()
{
    TestCase test;

    test.id = "TC_DTC_003";
    test.name = "Front-Left DTC Activation";
    test.description =
        "Verify that a front-left wheel-speed sensor "
        "Dropout activates DTC 0xC0031.";

    test.requirementId = "DTC-REQ-003";

    test.precondition =
        "Simulation shall be running without active DTCs.";

    test.stimulus =
        "Inject front-left SensorFault::Dropout.";

    test.expectedResult =
        "DTC 0xC0031 shall become Active.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool active =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "C0031 Active = true";

    test.result.actual =
        std::string("C0031 Active = ") +
        (active ? "true" : "false");

    test.result.status =
        active
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        active
        ? "Front-left sensor Dropout activated DTC C0031."
        : "Front-left sensor Dropout did not activate DTC C0031.";

    return test;
}

// ==================================================
// TC_DTC_004
// Front-Right DTC Activation
// ==================================================

TestCase TestRunner::runFrontRightDtcActivationTest()
{
    TestCase test;

    test.id = "TC_DTC_004";
    test.name = "Front-Right DTC Activation";
    test.description =
        "Verify that a front-right wheel-speed sensor "
        "Dropout activates DTC 0xC0034.";

    test.requirementId = "DTC-REQ-004";

    test.precondition =
        "Simulation shall be running without active DTCs.";

    test.stimulus =
        "Inject front-right SensorFault::Dropout.";

    test.expectedResult =
        "DTC 0xC0034 shall become Active.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    engine.setFrontRightWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool active =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontRightWheelSpeedSensor
        );

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "C0034 Active = true";

    test.result.actual =
        std::string("C0034 Active = ") +
        (active ? "true" : "false");

    test.result.status =
        active
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        active
        ? "Front-right sensor Dropout activated DTC C0034."
        : "Front-right sensor Dropout did not activate DTC C0034.";

    return test;
}

// ==================================================
// TC_DTC_005
// Front-Left DTC Recovery
// ==================================================

TestCase TestRunner::runFrontLeftDtcRecoveryTest()
{
    TestCase test;

    test.id = "TC_DTC_005";
    test.name = "Front-Left DTC Recovery";
    test.description =
        "Verify that C0031 transitions from Active to "
        "Inactive when the sensor becomes valid.";

    test.requirementId = "DTC-REQ-005";

    test.precondition =
        "C0031 shall be Active.";

    test.stimulus =
        "Clear the front-left wheel sensor fault.";

    test.expectedResult =
        "C0031 shall remain stored with Inactive status.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool wasActive =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    engine.clearFrontLeftWheelSensorFault();

    runUntil(
        engine,
        140.0
    );

    const Dtc* dtc =
        engine
        .getDtcManager()
        .findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool recovered =
        wasActive &&
        dtc != nullptr &&
        dtc->status ==
        DtcStatus::Inactive &&
        dtc->occurrenceCount >= 1;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Active -> Inactive, record retained";

    test.result.actual =
        recovered
        ? "Active -> Inactive, record retained"
        : "Unexpected DTC recovery state";

    test.result.status =
        recovered
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        recovered
        ? "C0031 recovered while retaining diagnostic history."
        : "C0031 did not recover as required.";

    return test;
}

// ==================================================
// TC_DTC_006
// Front-Right DTC Recovery
// ==================================================

TestCase TestRunner::runFrontRightDtcRecoveryTest()
{
    TestCase test;

    test.id = "TC_DTC_006";
    test.name = "Front-Right DTC Recovery";
    test.description =
        "Verify that C0034 transitions from Active to "
        "Inactive when the sensor becomes valid.";

    test.requirementId = "DTC-REQ-006";

    test.precondition =
        "C0034 shall be Active.";

    test.stimulus =
        "Clear the front-right wheel sensor fault.";

    test.expectedResult =
        "C0034 shall remain stored with Inactive status.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    engine.setFrontRightWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool wasActive =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontRightWheelSpeedSensor
        );

    engine.clearFrontRightWheelSensorFault();

    runUntil(
        engine,
        140.0
    );

    const Dtc* dtc =
        engine
        .getDtcManager()
        .findDtc(
            DtcDefinitions::
            FrontRightWheelSpeedSensor
        );

    const bool recovered =
        wasActive &&
        dtc != nullptr &&
        dtc->status ==
        DtcStatus::Inactive &&
        dtc->occurrenceCount >= 1;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Active -> Inactive, record retained";

    test.result.actual =
        recovered
        ? "Active -> Inactive, record retained"
        : "Unexpected DTC recovery state";

    test.result.status =
        recovered
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        recovered
        ? "C0034 recovered while retaining diagnostic history."
        : "C0034 did not recover as required.";

    return test;
}

// ==================================================
// TC_DTC_007
// Individual DTC Clear
// ==================================================

TestCase TestRunner::runIndividualDtcClearTest()
{
    TestCase test;

    test.id = "TC_DTC_007";
    test.name = "Individual DTC Clear";
    test.description =
        "Verify explicit clearing of one registered DTC.";

    test.requirementId = "DTC-REQ-007";

    test.precondition =
        "C0031 shall contain diagnostic history.";

    test.stimulus =
        "Execute clearDtc(C0031).";

    test.expectedResult =
        "Status Cleared, timestamps zero, occurrence count zero.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        100.0
    );

    manager.reportHealthy(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        120.0
    );

    const bool clearResult =
        manager.clearDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        clearResult &&
        dtc != nullptr &&
        dtc->status ==
        DtcStatus::Cleared &&
        dtc->firstDetectedTimeMs == 0.0 &&
        dtc->lastDetectedTimeMs == 0.0 &&
        dtc->occurrenceCount == 0;

    test.result.expected =
        "Cleared / 0.0 / 0.0 / 0";

    test.result.actual =
        valid
        ? "Cleared / 0.0 / 0.0 / 0"
        : "Individual DTC clear state mismatch";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Individual DTC was cleared correctly."
        : "Individual DTC clear did not reset all required data.";

    return test;
}

// ==================================================
// TC_DTC_008
// Clear All DTCs
// ==================================================

TestCase TestRunner::runClearAllDtcsTest()
{
    TestCase test;

    test.id = "TC_DTC_008";
    test.name = "Clear All DTCs";
    test.description =
        "Verify explicit clearing of all registered DTCs.";

    test.requirementId = "DTC-REQ-008";

    test.precondition =
        "Multiple DTCs shall contain diagnostic history.";

    test.stimulus =
        "Execute clearAll().";

    test.expectedResult =
        "Every registered DTC shall be Cleared with zero history.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        100.0
    );

    manager.reportFault(
        DtcDefinitions::
        FrontRightWheelSpeedSensor,
        110.0
    );

    manager.clearAll();

    bool valid =
        true;

    for (const Dtc& dtc :
        manager.getDtcs())
    {
        if (
            dtc.status !=
            DtcStatus::Cleared ||
            dtc.firstDetectedTimeMs !=
            0.0 ||
            dtc.lastDetectedTimeMs !=
            0.0 ||
            dtc.occurrenceCount !=
            0
            )
        {
            valid =
                false;

            break;
        }
    }

    test.result.expected =
        "All registered DTCs cleared";

    test.result.actual =
        valid
        ? "All registered DTCs cleared"
        : "One or more DTCs retained diagnostic data";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "All DTC records were cleared correctly."
        : "clearAll() did not reset every DTC record.";

    return test;
}

// ==================================================
// TC_DTC_009
// First Detection Timestamp
// ==================================================

TestCase TestRunner::runDtcFirstDetectionTimestampTest()
{
    TestCase test;

    test.id = "TC_DTC_009";
    test.name = "DTC First Detection Timestamp";
    test.description =
        "Verify that the first detection time is retained "
        "while a fault remains active.";

    test.requirementId = "DTC-REQ-009";

    test.precondition =
        "C0031 shall have no previous history.";

    test.stimulus =
        "Report C0031 at 100 ms and again at 110 ms.";

    test.expectedResult =
        "firstDetectedTimeMs shall remain 100 ms.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        100.0
    );

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        110.0
    );

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        dtc != nullptr &&
        std::fabs(
            dtc->firstDetectedTimeMs -
            100.0
        ) < 0.001;

    test.result.expected =
        "firstDetectedTimeMs = 100.0";

    test.result.actual =
        dtc != nullptr
        ? "firstDetectedTimeMs = " +
        std::to_string(
            dtc->firstDetectedTimeMs
        )
        : "DTC not found";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "First detection timestamp remained unchanged."
        : "First detection timestamp was not retained.";

    return test;
}

// ==================================================
// TC_DTC_010
// Last Detection Timestamp
// ==================================================

TestCase TestRunner::runDtcLastDetectionTimestampTest()
{
    TestCase test;

    test.id = "TC_DTC_010";
    test.name = "DTC Last Detection Timestamp";
    test.description =
        "Verify that the last detection timestamp updates "
        "while a fault remains active.";

    test.requirementId = "DTC-REQ-010";

    test.precondition =
        "C0031 shall be available.";

    test.stimulus =
        "Report C0031 at 100 ms and 110 ms.";

    test.expectedResult =
        "lastDetectedTimeMs shall equal 110 ms.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        100.0
    );

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        110.0
    );

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        dtc != nullptr &&
        std::fabs(
            dtc->lastDetectedTimeMs -
            110.0
        ) < 0.001;

    test.result.expected =
        "lastDetectedTimeMs = 110.0";

    test.result.actual =
        dtc != nullptr
        ? "lastDetectedTimeMs = " +
        std::to_string(
            dtc->lastDetectedTimeMs
        )
        : "DTC not found";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Last detection timestamp updated correctly."
        : "Last detection timestamp did not update correctly.";

    return test;
}

// ==================================================
// TC_DTC_011
// DTC Occurrence Count
// ==================================================

TestCase TestRunner::runDtcOccurrenceCountTest()
{
    TestCase test;

    test.id = "TC_DTC_011";
    test.name = "DTC Occurrence Count";
    test.description =
        "Verify occurrence count increments only when "
        "a DTC transitions into Active.";

    test.requirementId = "DTC-REQ-011";

    test.precondition =
        "C0031 shall have zero occurrences.";

    test.stimulus =
        "Activate repeatedly, recover, then activate again.";

    test.expectedResult =
        "Continuous active reports count once; reactivation counts twice.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        100.0
    );

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        110.0
    );

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        120.0
    );

    const Dtc* firstState =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool firstOccurrenceValid =
        firstState != nullptr &&
        firstState->occurrenceCount ==
        1;

    manager.reportHealthy(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        130.0
    );

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        140.0
    );

    const Dtc* secondState =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool secondOccurrenceValid =
        secondState != nullptr &&
        secondState->occurrenceCount ==
        2;

    const bool valid =
        firstOccurrenceValid &&
        secondOccurrenceValid;

    test.result.expected =
        "First continuous fault=1, second occurrence=2";

    test.result.actual =
        secondState != nullptr
        ? "Final occurrenceCount=" +
        std::to_string(
            secondState->occurrenceCount
        )
        : "DTC not found";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "DTC occurrence counter followed transition semantics."
        : "DTC occurrence counter incremented incorrectly.";

    return test;
}

// ==================================================
// TC_DTC_012
// DTC Lookup
// ==================================================

TestCase TestRunner::runDtcLookupTest()
{
    TestCase test;

    test.id = "TC_DTC_012";
    test.name = "DTC Lookup";
    test.description =
        "Verify retrieval of a registered DTC using its code.";

    test.requirementId = "DTC-REQ-012";

    test.precondition =
        "DtcManager shall contain registered DTCs.";

    test.stimulus =
        "Lookup C0031.";

    test.expectedResult =
        "findDtc() shall return the matching DTC.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        dtc != nullptr &&
        dtc->code ==
        DtcDefinitions::
        FrontLeftWheelSpeedSensor;

    test.result.expected =
        "C0031 lookup successful";

    test.result.actual =
        valid
        ? "C0031 lookup successful"
        : "C0031 lookup failed";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "DTC lookup returned the requested record."
        : "DTC lookup failed.";

    return test;
}

// ==================================================
// TC_DTC_013
// DTC Collection Access
// ==================================================

TestCase TestRunner::runDtcCollectionAccessTest()
{
    TestCase test;

    test.id = "TC_DTC_013";
    test.name = "DTC Collection Access";
    test.description =
        "Verify read-only access to the registered DTC collection.";

    test.requirementId = "DTC-REQ-013";

    test.precondition =
        "DtcManager shall be initialized.";

    test.stimulus =
        "Read getDtcs().";

    test.expectedResult =
        "Collection shall contain C0031 and C0034.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    const auto& dtcs =
        manager.getDtcs();

    bool leftFound =
        false;

    bool rightFound =
        false;

    for (const Dtc& dtc : dtcs)
    {
        if (
            dtc.code ==
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
            )
        {
            leftFound =
                true;
        }

        if (
            dtc.code ==
            DtcDefinitions::
            FrontRightWheelSpeedSensor
            )
        {
            rightFound =
                true;
        }
    }

    const bool valid =
        leftFound &&
        rightFound &&
        dtcs.size() >= 2;

    test.result.expected =
        "C0031 and C0034 available";

    test.result.actual =
        "Collection size=" +
        std::to_string(
            dtcs.size()
        );

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Registered DTC collection is accessible."
        : "Registered DTC collection is incomplete.";

    return test;
}

// ==================================================
// TC_DTC_014
// Active DTC Query
// ==================================================

TestCase TestRunner::runActiveDtcQueryTest()
{
    TestCase test;

    test.id = "TC_DTC_014";
    test.name = "Active DTC Query";
    test.description =
        "Verify querying the current Active status of a DTC.";

    test.requirementId = "DTC-REQ-014";

    test.precondition =
        "C0031 shall initially be inactive.";

    test.stimulus =
        "Report C0031 as faulty and query its status.";

    test.expectedResult =
        "isDtcActive(C0031) shall return true.";

    test.result.status =
        TestStatus::Running;

    DtcManager manager;

    const bool before =
        manager.isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    manager.reportFault(
        DtcDefinitions::
        FrontLeftWheelSpeedSensor,
        100.0
    );

    const bool after =
        manager.isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        !before &&
        after;

    test.result.expected =
        "Before=false, After=true";

    test.result.actual =
        "Before=" +
        std::string(
            before ? "true" : "false"
        ) +
        ", After=" +
        std::string(
            after ? "true" : "false"
        );

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Active DTC query returned the correct state."
        : "Active DTC query returned an incorrect state.";

    return test;
}

// ==================================================
// TC_DTC_015
// Runtime Fault Detection
// ==================================================

TestCase TestRunner::runRuntimeDtcDetectionTest()
{
    TestCase test;

    test.id = "TC_DTC_015";
    test.name = "Runtime DTC Detection";
    test.description =
        "Verify DTC detection during an already-running simulation.";

    test.requirementId = "DTC-REQ-015";

    test.precondition =
        "Simulation shall already have progressed.";

    test.stimulus =
        "Inject front-left Dropout without restarting.";

    test.expectedResult =
        "C0031 shall activate in the same simulation instance.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    const double beforeTime =
        engine.getCurrentTimeMs();

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        130.0
    );

    const bool active =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool continued =
        engine.getCurrentTimeMs() >
        beforeTime;

    const bool valid =
        active &&
        continued;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Same instance continues and C0031 becomes Active";

    test.result.actual =
        "Time=" +
        std::to_string(
            engine.getCurrentTimeMs()
        ) +
        ", Active=" +
        std::string(
            active ? "true" : "false"
        );

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Runtime fault detection operated without restart."
        : "Runtime fault detection failed.";

    return test;
}

// ==================================================
// TC_DTC_016
// Runtime Fault Recovery
// ==================================================

TestCase TestRunner::runRuntimeDtcRecoveryTest()
{
    TestCase test;

    test.id = "TC_DTC_016";
    test.name = "Runtime DTC Recovery";
    test.description =
        "Verify DTC recovery during the same running simulation.";

    test.requirementId = "DTC-REQ-016";

    test.precondition =
        "C0031 shall be Active during simulation.";

    test.stimulus =
        "Clear the sensor Dropout without restarting.";

    test.expectedResult =
        "C0031 shall transition to Inactive.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool active =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    engine.clearFrontLeftWheelSensorFault();

    runUntil(
        engine,
        150.0
    );

    const Dtc* dtc =
        engine
        .getDtcManager()
        .findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool recovered =
        dtc != nullptr &&
        dtc->status ==
        DtcStatus::Inactive;

    const bool valid =
        active &&
        recovered;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Active -> Inactive without restart";

    test.result.actual =
        valid
        ? "Active -> Inactive without restart"
        : "Runtime recovery failed";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Runtime DTC recovery operated correctly."
        : "Runtime DTC recovery failed.";

    return test;
}

// ==================================================
// TC_DTC_017
// Diagnostic Reset
// ==================================================

TestCase TestRunner::runDtcResetTest()
{
    TestCase test;

    test.id = "TC_DTC_017";
    test.name = "Diagnostic Reset";
    test.description =
        "Verify simulation reset restores the initial DTC state.";

    test.requirementId = "DTC-REQ-017";

    test.precondition =
        "At least one DTC shall contain active history.";

    test.stimulus =
        "Execute SimulationEngine::reset().";

    test.expectedResult =
        "All DTCs shall return to initial Inactive state.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        engine,
        100.0
    );

    engine.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        engine,
        120.0
    );

    const bool activeBeforeReset =
        engine
        .getDtcManager()
        .isDtcActive(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    engine.reset();

    bool clean =
        true;

    for (const Dtc& dtc :
        engine
        .getDtcManager()
        .getDtcs())
    {
        if (
            dtc.status !=
            DtcStatus::Inactive ||
            dtc.firstDetectedTimeMs !=
            0.0 ||
            dtc.lastDetectedTimeMs !=
            0.0 ||
            dtc.occurrenceCount !=
            0
            )
        {
            clean =
                false;

            break;
        }
    }

    const bool valid =
        activeBeforeReset &&
        clean;

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "All DTCs Inactive with zero diagnostic history";

    test.result.actual =
        clean
        ? "All DTCs restored to initial state"
        : "Diagnostic state remained after reset";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Simulation reset restored initial diagnostic state."
        : "Simulation reset did not restore diagnostic state.";

    return test;
}

// ==================================================
// TC_DTC_018
// Deterministic Diagnostic Behavior
// ==================================================

TestCase TestRunner::runDtcDeterminismTest()
{
    TestCase test;

    test.id = "TC_DTC_018";
    test.name = "Deterministic Diagnostic Behavior";
    test.description =
        "Verify identical simulations produce identical DTC results.";

    test.requirementId = "DTC-REQ-018";

    test.precondition =
        "Two clean simulation instances shall exist.";

    test.stimulus =
        "Apply identical timing and fault stimulus to both.";

    test.expectedResult =
        "Diagnostic states, counters, and timestamps shall match.";

    test.result.status =
        TestStatus::Running;

    SimulationEngine first(
        LogLevel::Quiet
    );

    SimulationEngine second(
        LogLevel::Quiet
    );

    first.reset();
    second.reset();

    first.startScenario(
        DrivingScenario::Acceleration
    );

    second.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        first,
        100.0
    );

    runUntil(
        second,
        100.0
    );

    first.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    second.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        first,
        150.0
    );

    runUntil(
        second,
        150.0
    );

    const Dtc* firstDtc =
        first
        .getDtcManager()
        .findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const Dtc* secondDtc =
        second
        .getDtcManager()
        .findDtc(
            DtcDefinitions::
            FrontLeftWheelSpeedSensor
        );

    const bool valid =
        firstDtc != nullptr &&
        secondDtc != nullptr &&
        firstDtc->status ==
        secondDtc->status &&
        firstDtc->occurrenceCount ==
        secondDtc->occurrenceCount &&
        std::fabs(
            firstDtc->firstDetectedTimeMs -
            secondDtc->firstDetectedTimeMs
        ) < 0.001 &&
        std::fabs(
            firstDtc->lastDetectedTimeMs -
            secondDtc->lastDetectedTimeMs
        ) < 0.001;

    test.result.executionTimeMs =
        first.getCurrentTimeMs();

    test.result.expected =
        "Identical DTC state, count and timestamps";

    test.result.actual =
        valid
        ? "Diagnostic results are identical"
        : "Diagnostic results differ";

    test.result.status =
        valid
        ? TestStatus::Passed
        : TestStatus::Failed;

    test.result.message =
        valid
        ? "Diagnostic behavior was deterministic."
        : "Diagnostic behavior was not deterministic.";

    return test;
}