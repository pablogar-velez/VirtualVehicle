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

TestCase TestRunner::runIndividualTestExecutionVerification()
{
    TestCase test;

    test.id = "TC_VAL_001";
    test.name = "Individual Test Execution";
    test.description =
        "Verify that the validation framework can execute one selected automated test case.";
    test.requirementId = "VAL-REQ-001";
    test.precondition =
        "TC_SYS_001 shall be registered in the TestRunner dispatcher.";
    test.stimulus =
        "Execute only TC_SYS_001 through runTest().";
    test.expectedResult =
        "Exactly the requested TC_SYS_001 result shall be returned as executed.";
    test.result.status = TestStatus::Running;

    const TestCase selected =
        runTest(
            "TC_SYS_001"
        );

    const bool correctId =
        selected.id == "TC_SYS_001";

    const bool executed =
        selected.result.status !=
        TestStatus::NotRun &&
        selected.result.status !=
        TestStatus::Running;

    test.result.executionTimeMs =
        selected.result.executionTimeMs;

    test.result.expected =
        "Returned ID=TC_SYS_001 and final execution status";

    test.result.actual =
        "Returned ID=" +
        selected.id +
        ", executed=" +
        std::string(executed ? "true" : "false");

    if (correctId && executed)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Validation framework executed the individually selected test case.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Validation framework did not correctly execute the selected individual test.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runValidationTestIsolationVerification()
{
    TestCase test;

    test.id = "TC_VAL_002";
    test.name = "Validation Test Isolation";
    test.description =
        "Verify that independently created automated simulation test instances "
        "do not share simulation state.";
    test.requirementId = "VAL-REQ-003";
    test.precondition =
        "Two clean SimulationEngine instances shall exist.";
    test.stimulus =
        "Advance only the first instance and inject a sensor fault into it.";
    test.expectedResult =
        "The second instance shall remain at its initial time and healthy ABS state.";
    test.result.status = TestStatus::Running;

    SimulationEngine first(LogLevel::Quiet);
    SimulationEngine second(LogLevel::Quiet);

    first.reset();
    second.reset();

    first.startScenario(
        DrivingScenario::Acceleration
    );

    runUntil(
        first,
        100.0
    );

    first.setFrontLeftWheelSensorFault(
        SensorFault::Dropout
    );

    runUntil(
        first,
        120.0
    );

    const bool firstChanged =
        first.getCurrentTimeMs() > 0.0 &&
        first.getAbsState().healthStatus ==
        AbsHealthStatus::Degraded;

    const bool secondTimeClean =
        std::fabs(
            second.getCurrentTimeMs()
        ) <= 0.001;

    const bool secondHealthClean =
        second.getAbsState().healthStatus ==
        AbsHealthStatus::Healthy;

    const bool secondTraceClean =
        second.getCanTrace().empty();

    test.result.executionTimeMs =
        first.getCurrentTimeMs();

    test.result.expected =
        "First instance changed; second instance remains clean";

    test.result.actual =
        "FirstTime=" +
        std::to_string(first.getCurrentTimeMs()) +
        ", SecondTime=" +
        std::to_string(second.getCurrentTimeMs()) +
        ", SecondTrace=" +
        std::to_string(second.getCanTrace().size());

    if (
        firstChanged &&
        secondTimeClean &&
        secondHealthClean &&
        secondTraceClean
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Automated simulation instances remained isolated.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Automated simulation instance isolation was not maintained.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runTestStatusVerification()
{
    TestCase test;

    test.id = "TC_VAL_003";
    test.name = "Test Status";
    test.description =
        "Verify that an executed automated test produces a final PASS or FAIL status.";
    test.requirementId = "VAL-REQ-008";
    test.precondition =
        "A registered automated test shall be available.";
    test.stimulus =
        "Execute TC_CAN_002 through the validation dispatcher.";
    test.expectedResult =
        "The executed result status shall be either Passed or Failed.";
    test.result.status = TestStatus::Running;

    const TestCase executed =
        runTest(
            "TC_CAN_002"
        );

    const bool finalStatus =
        executed.result.status ==
        TestStatus::Passed ||
        executed.result.status ==
        TestStatus::Failed;

    test.result.executionTimeMs =
        executed.result.executionTimeMs;

    test.result.expected =
        "Status = PASS or FAIL";

    test.result.actual =
        finalStatus
        ? (
            executed.result.status == TestStatus::Passed
            ? "Status = PASS"
            : "Status = FAIL"
        )
        : "Status is not final";

    if (finalStatus)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Executed automated test produced a final PASS/FAIL result.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Executed automated test did not produce a final PASS/FAIL result.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runExecutionTimeVerification()
{
    TestCase test;

    test.id = "TC_VAL_004";
    test.name = "Execution-Time Evidence";
    test.description =
        "Verify that the validation framework records execution time for an executed test.";
    test.requirementId = "VAL-REQ-009";
    test.precondition =
        "A simulation-based automated test shall be available.";
    test.stimulus =
        "Execute TC_SYS_001 and inspect its executionTimeMs evidence.";
    test.expectedResult =
        "Execution time shall be recorded as a positive value.";
    test.result.status = TestStatus::Running;

    const TestCase executed =
        runTest(
            "TC_SYS_001"
        );

    const bool recorded =
        executed.result.executionTimeMs > 0.0;

    test.result.executionTimeMs =
        executed.result.executionTimeMs;

    test.result.expected =
        "executionTimeMs > 0";

    test.result.actual =
        "executionTimeMs=" +
        std::to_string(
            executed.result.executionTimeMs
        );

    if (recorded)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Execution-time evidence was recorded for the executed automated test.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Execution-time evidence was not recorded.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runResponseTimeEvidenceVerification()
{
    TestCase test;

    test.id = "TC_VAL_005";
    test.name = "Response-Time Evidence";
    test.description =
        "Verify that a test associated with a maximum response-time requirement "
        "records measured response time.";
    test.requirementId = "VAL-REQ-010";
    test.precondition =
        "TC_ABS_002 shall verify the 20 ms front-left sensor-fault response requirement.";
    test.stimulus =
        "Execute TC_ABS_002 and inspect responseTimeMs.";
    test.expectedResult =
        "Measured response time shall be recorded and the timing requirement shall be identified.";
    test.result.status = TestStatus::Running;

    const TestCase executed =
        runTest(
            "TC_ABS_002"
        );

    const bool timingRequirementPresent =
        executed.result.maximumAllowedResponseTimeMs > 0.0;

    const bool responseRecorded =
        executed.result.responseTimeMs > 0.0;

    test.result.executionTimeMs =
        executed.result.executionTimeMs;

    test.result.responseTimeMs =
        executed.result.responseTimeMs;

    test.result.maximumAllowedResponseTimeMs =
        executed.result.maximumAllowedResponseTimeMs;

    test.result.expected =
        "responseTimeMs > 0 and maximumAllowedResponseTimeMs = 20";

    test.result.actual =
        "responseTimeMs=" +
        std::to_string(executed.result.responseTimeMs) +
        ", maximumAllowedResponseTimeMs=" +
        std::to_string(
            executed.result.maximumAllowedResponseTimeMs
        );

    if (
        timingRequirementPresent &&
        responseRecorded
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Measured response-time evidence was recorded for the timed requirement.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Required response-time evidence was not recorded.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runTimingPassCriteriaVerification()
{
    TestCase test;

    test.id = "TC_VAL_006";
    test.name = "Timing Pass Criteria";
    test.description =
        "Verify validation timing pass criteria reject a response time "
        "that exceeds its maximum allowed limit.";
    test.requirementId = "VAL-REQ-012";
    test.precondition =
        "A TestResult shall define a 20 ms maximum response time.";
    test.stimulus =
        "Evaluate a synthetic measured response time of 21 ms against a 20 ms maximum.";
    test.expectedResult =
        "responseTimeWithinLimit() shall return false.";
    test.result.status = TestStatus::Running;

    TestResult timingResult{};

    timingResult.responseTimeMs =
        21.0;

    timingResult.maximumAllowedResponseTimeMs =
        20.0;

    const bool withinLimit =
        timingResult.responseTimeWithinLimit();

    test.result.expected =
        "21 ms <= 20 ms = false";

    test.result.actual =
        std::string("responseTimeWithinLimit=") +
        (withinLimit ? "true" : "false");

    if (!withinLimit)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Validation timing criteria correctly rejected an over-limit response time.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Validation timing criteria incorrectly accepted an over-limit response time.";
    }

    return test;
}

