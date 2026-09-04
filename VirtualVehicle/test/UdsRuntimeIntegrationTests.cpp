#include "TestRunner.h"

#include <chrono>
#include <cstdint>
#include <functional>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "../diagnostics/UdsRequest.h"
#include "../diagnostics/UdsResponse.h"
#include "../diagnostics/UdsService.h"

#include "../simulation/SimulationEngine.h"

namespace
{
    using Clock =
        std::chrono::steady_clock;

    constexpr std::uint32_t AbsCanId =
        0x080;

    constexpr std::uint32_t PowertrainCanId =
        0x100;

    constexpr std::uint32_t SteeringCanId =
        0x120;

    constexpr std::uint32_t UdsRequestCanId =
        0x7E0;

    constexpr std::uint32_t UdsResponseCanId =
        0x7E8;

    double elapsedMilliseconds(
        const Clock::time_point& start,
        const Clock::time_point& end)
    {
        return std::chrono::duration<
            double,
            std::milli
        >(
            end - start
        ).count();
    }

    TestCase createRuntimeTestCase(
        const std::string& id,
        const std::string& name,
        const std::string& requirementId,
        const std::string& description,
        const std::string& stimulus,
        const std::string& expectedResult)
    {
        TestCase test;

        test.id =
            id;

        test.name =
            name;

        test.requirementId =
            requirementId;

        test.description =
            description;

        test.precondition =
            "SimulationEngine is initialized in a deterministic reset state.";

        test.stimulus =
            stimulus;

        test.expectedResult =
            expectedResult;

        return test;
    }

    TestCase executeRuntimeTest(
        const std::string& id,
        const std::string& name,
        const std::string& requirementId,
        const std::string& description,
        const std::string& stimulus,
        const std::string& expectedResult,
        const std::function<bool(std::string&)>& body)
    {
        TestCase test =
            createRuntimeTestCase(
                id,
                name,
                requirementId,
                description,
                stimulus,
                expectedResult
            );

        const auto start =
            Clock::now();

        bool passed =
            false;

        std::string actual;

        try
        {
            passed =
                body(
                    actual
                );
        }
        catch (const std::exception& exception)
        {
            actual =
                std::string(
                    "Exception: "
                ) +
                exception.what();

            passed =
                false;
        }
        catch (...)
        {
            actual =
                "Unknown exception.";

            passed =
                false;
        }

        const auto end =
            Clock::now();

        test.result.status =
            passed
            ? TestStatus::Passed
            : TestStatus::Failed;

        test.result.expected =
            expectedResult;

        test.result.actual =
            actual;

        test.result.message =
            passed
            ? name + " verified."
            : name + " verification failed.";

        test.result.executionTimeMs =
            elapsedMilliseconds(
                start,
                end
            );

        return test;
    }

    UdsRequest createSessionRequest()
    {
        UdsRequest request;

        request.serviceId =
            UdsService::
            DiagnosticSessionControl;

        request.payload = {
            UdsService::ExtendedSession
        };

        return request;
    }

    UdsRequest createUnsupportedMultiFrameRequest()
    {
        UdsRequest request;

        request.serviceId =
            0x99;

        request.payload = {
            0x01,
            0x02,
            0x03,
            0x04,
            0x05,
            0x06,
            0x07,
            0x08,
            0x09,
            0x0A,
            0x0B,
            0x0C,
            0x0D,
            0x0E,
            0x0F,
            0x10,
            0x11,
            0x12,
            0x13,
            0x14
        };

        return request;
    }

    UdsRequest createVehicleSpeedDidRequest()
    {
        UdsRequest request;

        request.serviceId =
            0x22;

        request.payload = {
            0xF1,
            0x00
        };

        return request;
    }

    bool completeRuntimeTransaction(
        SimulationEngine& engine,
        const UdsRequest& request,
        int maximumSteps = 20)
    {
        engine.submitUdsRequest(
            request
        );

        for (
            int step = 0;
            step < maximumSteps &&
            engine.hasPendingUdsTransaction();
            ++step
            )
        {
            engine.update(
                10.0
            );
        }

        return
            !engine.hasPendingUdsTransaction()
            &&
            engine.hasCompletedUdsResponse();
    }

    std::size_t countTraceIdentifier(
        const SimulationEngine& engine,
        std::uint32_t arbitrationId)
    {
        std::size_t count =
            0;

        for (
            const auto& entry :
            engine.getCanTrace()
            )
        {
            if (
                entry.arbitrationId ==
                arbitrationId
                )
            {
                ++count;
            }
        }

        return count;
    }

    bool traceContainsIdentifier(
        const SimulationEngine& engine,
        std::uint32_t arbitrationId)
    {
        return
            countTraceIdentifier(
                engine,
                arbitrationId
            ) >
            0;
    }

    std::size_t firstTraceIndex(
        const SimulationEngine& engine,
        std::uint32_t arbitrationId)
    {
        const auto& trace =
            engine.getCanTrace();

        for (
            std::size_t index = 0;
            index < trace.size();
            ++index
            )
        {
            if (
                trace[index].arbitrationId ==
                arbitrationId
                )
            {
                return index;
            }
        }

        return
            std::numeric_limits<
            std::size_t
            >::max();
    }

    bool traceContainsPeriodicVehicleTraffic(
        const SimulationEngine& engine)
    {
        return
            traceContainsIdentifier(
                engine,
                AbsCanId
            )
            &&
            traceContainsIdentifier(
                engine,
                PowertrainCanId
            )
            &&
            traceContainsIdentifier(
                engine,
                SteeringCanId
            );
    }

    bool responseEquals(
        const UdsResponse& lhs,
        const UdsResponse& rhs)
    {
        return
            lhs.positive ==
            rhs.positive
            &&
            lhs.serviceId ==
            rhs.serviceId
            &&
            lhs.payload ==
            rhs.payload;
    }

    bool diagnosticTraceTimingValid(
        const SimulationEngine& engine)
    {
        bool foundDiagnosticFrame =
            false;

        for (
            const auto& entry :
            engine.getCanTrace()
            )
        {
            if (
                entry.arbitrationId !=
                UdsRequestCanId
                &&
                entry.arbitrationId !=
                UdsResponseCanId
                )
            {
                continue;
            }

            foundDiagnosticFrame =
                true;

            if (
                entry.txEndTimeMs <=
                entry.txStartTimeMs
                ||
                entry.transmissionTimeMs <=
                0.0
                ||
                entry.waitingTimeMs <
                0.0
                )
            {
                return false;
            }
        }

        return foundDiagnosticFrame;
    }

    bool tracesEqual(
        const SimulationEngine& lhs,
        const SimulationEngine& rhs)
    {
        const auto& first =
            lhs.getCanTrace();

        const auto& second =
            rhs.getCanTrace();

        if (
            first.size() !=
            second.size()
            )
        {
            return false;
        }

        for (
            std::size_t index = 0;
            index < first.size();
            ++index
            )
        {
            if (
                first[index].requestTimeMs !=
                second[index].requestTimeMs
                ||
                first[index].txStartTimeMs !=
                second[index].txStartTimeMs
                ||
                first[index].txEndTimeMs !=
                second[index].txEndTimeMs
                ||
                first[index].waitingTimeMs !=
                second[index].waitingTimeMs
                ||
                first[index].transmissionTimeMs !=
                second[index].transmissionTimeMs
                ||
                first[index].arbitrationId !=
                second[index].arbitrationId
                ||
                first[index].dlc !=
                second[index].dlc
                ||
                first[index].data !=
                second[index].data
                )
            {
                return false;
            }
        }

        return true;
    }
}

// ============================================================
// UDSRT-REQ-001
// ============================================================

TestCase TestRunner::runUdsRuntimeRequestSubmissionTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_001",
        "UDS Runtime Request Submission",
        "UDSRT-REQ-001",
        "Verify a UDS request can be submitted to the active SimulationEngine runtime.",
        "Submit an Extended Session request before advancing simulation time.",
        "The diagnostic transaction shall enter the pending state.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            engine.submitUdsRequest(
                createSessionRequest()
            );

            const bool passed =
                engine.hasPendingUdsTransaction()
                &&
                !engine.hasCompletedUdsResponse();

            actual =
                passed
                ? "Runtime UDS transaction is pending."
                : "Runtime UDS transaction did not enter pending state.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-002
// ============================================================

TestCase TestRunner::runUdsRuntimeRequestIdentifierTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_002",
        "UDS Runtime Request CAN Identifier",
        "UDSRT-REQ-002",
        "Verify runtime diagnostic request traffic uses CAN identifier 0x7E0.",
        "Complete one runtime Extended Session transaction.",
        "The CAN trace shall contain request traffic on 0x7E0.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                );

            actual =
                passed
                ? "CAN trace contains UDS request ID 0x7E0."
                : "UDS request ID 0x7E0 was not observed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-003
// ============================================================

TestCase TestRunner::runUdsRuntimeRequestSegmentationTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_003",
        "UDS Runtime Request ISO-TP Segmentation",
        "UDSRT-REQ-003",
        "Verify a request larger than one ISO-TP Single Frame is segmented during runtime.",
        "Submit an unsupported request containing 20 payload bytes.",
        "More than one 0x7E0 CAN frame shall be transmitted and the transaction shall complete.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createUnsupportedMultiFrameRequest()
                );

            const std::size_t requestFrames =
                countTraceIdentifier(
                    engine,
                    UdsRequestCanId
                );

            const bool passed =
                completed
                &&
                requestFrames >
                1;

            actual =
                "Runtime request frame count = " +
                std::to_string(
                    requestFrames
                );

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-004
// ============================================================

TestCase TestRunner::runUdsRuntimeSharedBusRequestTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_004",
        "UDS Runtime Shared CAN Request",
        "UDSRT-REQ-004",
        "Verify diagnostic request traffic uses the same runtime CAN trace as periodic ECU traffic.",
        "Submit a diagnostic request at t=0 and advance the simulation.",
        "The same trace shall contain periodic ECU traffic and UDS request traffic.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsPeriodicVehicleTraffic(
                    engine
                )
                &&
                traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                );

            actual =
                passed
                ? "Periodic ECU and UDS request traffic share the runtime CAN trace."
                : "Shared-bus request evidence is incomplete.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-005
// ============================================================

TestCase TestRunner::runUdsRuntimeSharedArbitrationTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_005",
        "UDS Runtime Shared Arbitration",
        "UDSRT-REQ-005",
        "Verify diagnostic and periodic ECU frames participate in the same arbitration opportunity.",
        "Queue a UDS request at t=0, then allow initial ECU transmissions at t=0.",
        "Lower-ID periodic ECU frames shall transmit before diagnostic request 0x7E0.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            completeRuntimeTransaction(
                engine,
                createSessionRequest()
            );

            const std::size_t absIndex =
                firstTraceIndex(
                    engine,
                    AbsCanId
                );

            const std::size_t powertrainIndex =
                firstTraceIndex(
                    engine,
                    PowertrainCanId
                );

            const std::size_t steeringIndex =
                firstTraceIndex(
                    engine,
                    SteeringCanId
                );

            const std::size_t udsIndex =
                firstTraceIndex(
                    engine,
                    UdsRequestCanId
                );

            const bool passed =
                absIndex <
                powertrainIndex
                &&
                powertrainIndex <
                steeringIndex
                &&
                steeringIndex <
                udsIndex;

            actual =
                passed
                ? "Arbitration order: 0x080, 0x100, 0x120, 0x7E0."
                : "Expected shared arbitration ordering was not observed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-006
// ============================================================

TestCase TestRunner::runUdsRuntimePriorityPreservationTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_006",
        "UDS Runtime CAN Priority Preservation",
        "UDSRT-REQ-006",
        "Verify lower CAN identifiers retain priority when diagnostic traffic is pending.",
        "Submit a UDS request concurrently with the initial periodic ECU transmissions.",
        "0x080, 0x100, and 0x120 shall precede 0x7E0.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            completeRuntimeTransaction(
                engine,
                createSessionRequest()
            );

            const std::size_t diagnosticIndex =
                firstTraceIndex(
                    engine,
                    UdsRequestCanId
                );

            const bool passed =
                firstTraceIndex(
                    engine,
                    AbsCanId
                ) <
                diagnosticIndex
                &&
                firstTraceIndex(
                    engine,
                    PowertrainCanId
                ) <
                diagnosticIndex
                &&
                firstTraceIndex(
                    engine,
                    SteeringCanId
                ) <
                diagnosticIndex;

            actual =
                passed
                ? "All periodic ECU identifiers retained priority over 0x7E0."
                : "CAN priority preservation failed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-007
// ============================================================

TestCase TestRunner::runUdsRuntimeNoTrafficLossTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_007",
        "UDS Runtime No Existing ECU Traffic Loss",
        "UDSRT-REQ-007",
        "Verify runtime diagnostics do not discard periodic ECU CAN frames.",
        "Execute a runtime diagnostic transaction while periodic ECU traffic is active.",
        "ABS, Powertrain, and Steering frames shall all remain present.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsPeriodicVehicleTraffic(
                    engine
                );

            actual =
                passed
                ? "ABS, Powertrain, and Steering traffic remained present."
                : "One or more periodic ECU frame types were lost.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-008
// ============================================================

TestCase TestRunner::runUdsRuntimeCentralRoutingTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_008",
        "UDS Runtime Central CAN Routing",
        "UDSRT-REQ-008",
        "Verify runtime routing separates vehicle and diagnostic traffic after bus transmission.",
        "Complete an Extended Session request while periodic ECU frames are transmitted.",
        "Vehicle traffic shall remain present and the UDS server session shall change.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsPeriodicVehicleTraffic(
                    engine
                )
                &&
                engine
                .getUdsServer()
                .getCurrentSession() ==
                UdsService::ExtendedSession;

            actual =
                passed
                ? "Vehicle frames and UDS request were routed to their intended consumers."
                : "Runtime CAN routing evidence is incomplete.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-009
// ============================================================

TestCase TestRunner::runUdsRuntimeVehicleRoutingTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_009",
        "UDS Runtime Vehicle Routing Preservation",
        "UDSRT-REQ-009",
        "Verify vehicle CAN processing remains operational when diagnostic routing is enabled.",
        "Run one update containing both vehicle and diagnostic CAN traffic.",
        "Vehicle state shall be updated and all periodic message identifiers shall be transmitted.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsPeriodicVehicleTraffic(
                    engine
                )
                &&
                engine
                .getVehicleState()
                .engineRpm >=
                800;

            actual =
                passed
                ? "Vehicle runtime processing remained active."
                : "Vehicle routing regression detected.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-010
// ============================================================

TestCase TestRunner::runUdsRuntimeRequestRoutingTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_010",
        "UDS Runtime Request Routing",
        "UDSRT-REQ-010",
        "Verify 0x7E0 runtime frames reach the UDS request processing path.",
        "Request Extended Diagnostic Session through SimulationEngine.",
        "The UDS server shall enter Extended Session.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                )
                &&
                engine
                .getUdsServer()
                .getCurrentSession() ==
                UdsService::ExtendedSession;

            actual =
                passed
                ? "0x7E0 request reached UdsServer and changed session."
                : "UDS request routing failed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-011
// ============================================================

TestCase TestRunner::runUdsRuntimeResponseRoutingTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_011",
        "UDS Runtime Response Routing",
        "UDSRT-REQ-011",
        "Verify 0x7E8 runtime frames are routed to diagnostic response reconstruction.",
        "Complete one runtime Extended Session request.",
        "A completed response shall be available and 0x7E8 shall appear in trace.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                )
                &&
                engine.hasCompletedUdsResponse();

            actual =
                passed
                ? "0x7E8 response was routed and reconstructed."
                : "UDS response routing failed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-012
// ============================================================

TestCase TestRunner::runUdsRuntimeReassemblyProcessingTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_012",
        "UDS Runtime Request Reassembly Processing",
        "UDSRT-REQ-012",
        "Verify UdsServer processing occurs after complete multi-frame request reconstruction.",
        "Send a 20-byte unsupported-service payload requiring multiple request frames.",
        "The complete request shall reach UdsServer and return NRC ServiceNotSupported.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createUnsupportedMultiFrameRequest()
                );

            if (!completed)
            {
                actual =
                    "Multi-frame runtime transaction did not complete.";

                return false;
            }

            const UdsResponse& response =
                engine.getCompletedUdsResponse();

            const bool passed =
                countTraceIdentifier(
                    engine,
                    UdsRequestCanId
                ) >
                1
                &&
                response.isNegative()
                &&
                response.serviceId ==
                UdsService::NegativeResponse
                &&
                response.payload.size() ==
                2
                &&
                response.payload[0] ==
                0x99;

            actual =
                passed
                ? "Complete multi-frame request produced a negative UDS response."
                : "Multi-frame request was not processed as expected.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-013
// ============================================================

TestCase TestRunner::runUdsRuntimeCurrentVehicleDataTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_013",
        "UDS Runtime Current Vehicle Data",
        "UDSRT-REQ-013",
        "Verify runtime ReadDataByIdentifier uses current SimulationEngine diagnostic data.",
        "Accelerate the vehicle, then request DID F100 through runtime CAN diagnostics.",
        "A positive 0x62 response containing DID F100 shall be returned.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            engine.startScenario(
                DrivingScenario::Acceleration
            );

            engine.update(
                100.0
            );

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createVehicleSpeedDidRequest()
                );

            if (!completed)
            {
                actual =
                    "Runtime DID transaction did not complete.";

                return false;
            }

            const UdsResponse& response =
                engine.getCompletedUdsResponse();

            const bool passed =
                response.isPositive()
                &&
                response.serviceId ==
                0x62
                &&
                response.payload.size() >=
                2
                &&
                response.payload[0] ==
                0xF1
                &&
                response.payload[1] ==
                0x00;

            actual =
                passed
                ? "Positive DID F100 response returned from current runtime data."
                : "Current vehicle data DID response was invalid.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-014
// ============================================================

TestCase TestRunner::runUdsRuntimeResponseGenerationTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_014",
        "UDS Runtime Response Generation",
        "UDSRT-REQ-014",
        "Verify UdsServer output is converted into runtime diagnostic response traffic.",
        "Submit a supported Extended Session request.",
        "A positive completed UDS response shall be generated.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                engine
                .getCompletedUdsResponse()
                .isPositive();

            actual =
                passed
                ? "Positive runtime UDS response generated."
                : "Runtime response generation failed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-015
// ============================================================

TestCase TestRunner::runUdsRuntimeResponseIdentifierTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_015",
        "UDS Runtime Response CAN Identifier",
        "UDSRT-REQ-015",
        "Verify runtime physical diagnostic responses use CAN identifier 0x7E8.",
        "Complete one runtime diagnostic transaction.",
        "The CAN trace shall contain response traffic on 0x7E8.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                );

            actual =
                passed
                ? "CAN trace contains response ID 0x7E8."
                : "Response ID 0x7E8 was not observed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-016
// ============================================================

TestCase TestRunner::runUdsRuntimeSharedBusResponseTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_016",
        "UDS Runtime Shared CAN Response",
        "UDSRT-REQ-016",
        "Verify diagnostic responses use the same runtime CAN bus as periodic ECU traffic.",
        "Complete one runtime UDS transaction during normal ECU operation.",
        "The trace shall contain 0x7E8 and periodic ECU traffic.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                )
                &&
                traceContainsPeriodicVehicleTraffic(
                    engine
                );

            actual =
                passed
                ? "UDS response and periodic ECU traffic share the runtime trace."
                : "Shared-bus response evidence is incomplete.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-017
// ============================================================

TestCase TestRunner::runUdsRuntimeResponseReassemblyTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_017",
        "UDS Runtime Response Reassembly",
        "UDSRT-REQ-017",
        "Verify the final UDS response is reconstructed from runtime response CAN traffic.",
        "Complete an Extended Session transaction.",
        "The reconstructed response shall be positive 0x50 with session 0x03.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            if (!completed)
            {
                actual =
                    "Runtime transaction did not complete.";

                return false;
            }

            const UdsResponse& response =
                engine.getCompletedUdsResponse();

            const bool passed =
                response.isPositive()
                &&
                response.serviceId ==
                0x50
                &&
                response.payload.size() ==
                1
                &&
                response.payload[0] ==
                UdsService::ExtendedSession;

            actual =
                passed
                ? "Reconstructed response = 0x50 0x03."
                : "Reconstructed response did not match expected data.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-018
// ============================================================

TestCase TestRunner::runUdsRuntimeTraceTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_018",
        "UDS Runtime CAN Trace Evidence",
        "UDSRT-REQ-018",
        "Verify runtime diagnostic request and response frames are recorded in CAN trace.",
        "Complete one runtime diagnostic transaction.",
        "Trace shall contain both 0x7E0 and 0x7E8.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            completeRuntimeTransaction(
                engine,
                createSessionRequest()
            );

            const bool passed =
                traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                )
                &&
                traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                );

            actual =
                passed
                ? "Trace contains both diagnostic identifiers."
                : "Diagnostic trace evidence is incomplete.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-019
// ============================================================

TestCase TestRunner::runUdsRuntimeStatisticsTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_019",
        "UDS Runtime CAN Statistics",
        "UDSRT-REQ-019",
        "Verify runtime diagnostic traffic contributes to CAN transmission statistics.",
        "Record frame count, execute a runtime diagnostic transaction, then read frame count again.",
        "Frames transmitted shall increase and diagnostic trace entries shall be present.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const auto before =
                engine
                .getCanStatistics()
                .getFramesTransmitted();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const auto after =
                engine
                .getCanStatistics()
                .getFramesTransmitted();

            const bool passed =
                completed
                &&
                after >
                before
                &&
                traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                )
                &&
                traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                );

            actual =
                "Frames transmitted before = " +
                std::to_string(
                    before
                ) +
                ", after = " +
                std::to_string(
                    after
                );

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-020
// ============================================================

TestCase TestRunner::runUdsRuntimeTimingTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_020",
        "UDS Runtime CAN Timing",
        "UDSRT-REQ-020",
        "Verify runtime diagnostic frames use VirtualCanBus transmission and waiting-time evidence.",
        "Complete one diagnostic transaction while periodic traffic is present.",
        "Each diagnostic trace entry shall have positive transmission duration and non-negative wait.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                diagnosticTraceTimingValid(
                    engine
                );

            actual =
                passed
                ? "Diagnostic CAN timing evidence is valid."
                : "Diagnostic CAN timing evidence is invalid.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-021
// ============================================================

TestCase TestRunner::runUdsRuntimePendingStateTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_021",
        "UDS Runtime Pending Transaction State",
        "UDSRT-REQ-021",
        "Verify runtime state represents a diagnostic transaction before completion.",
        "Submit a diagnostic request without advancing the simulation.",
        "Pending shall be true and completed response shall be unavailable.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            engine.submitUdsRequest(
                createSessionRequest()
            );

            const bool passed =
                engine.hasPendingUdsTransaction()
                &&
                !engine.hasCompletedUdsResponse();

            actual =
                passed
                ? "Pending state retained before CAN processing."
                : "Pending transaction state is invalid.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-022
// ============================================================

TestCase TestRunner::runUdsRuntimeCompletionTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_022",
        "UDS Runtime Transaction Completion",
        "UDSRT-REQ-022",
        "Verify a runtime transaction completes only with a reconstructed response available.",
        "Submit and process an Extended Session request.",
        "Pending shall clear and completed response shall become available.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                !engine.hasPendingUdsTransaction()
                &&
                engine.hasCompletedUdsResponse();

            actual =
                passed
                ? "Transaction completed with response available."
                : "Transaction completion state is invalid.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-023
// ============================================================

TestCase TestRunner::runUdsRuntimeResultAccessTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_023",
        "UDS Runtime Result Access",
        "UDSRT-REQ-023",
        "Verify completed runtime UDS response can be read and explicitly cleared.",
        "Complete a runtime transaction, access its response, then clear it.",
        "Response data shall be accessible before clear and unavailable afterward.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            if (
                !completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                )
                )
            {
                actual =
                    "Runtime transaction did not complete.";

                return false;
            }

            const UdsResponse response =
                engine.getCompletedUdsResponse();

            const bool validBeforeClear =
                response.isPositive()
                &&
                response.serviceId ==
                0x50;

            engine.clearCompletedUdsResponse();

            const bool passed =
                validBeforeClear
                &&
                !engine.hasCompletedUdsResponse();

            actual =
                passed
                ? "Completed response accessed and cleared successfully."
                : "Completed response access/clear behavior failed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-024
// ============================================================

TestCase TestRunner::runUdsRuntimeResetStateTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_024",
        "UDS Runtime Diagnostic Reset",
        "UDSRT-REQ-024",
        "Verify SimulationEngine reset clears incomplete runtime diagnostic state.",
        "Submit a request, confirm pending state, then reset.",
        "Pending and completed-response state shall both be cleared.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            engine.submitUdsRequest(
                createSessionRequest()
            );

            const bool pendingBeforeReset =
                engine.hasPendingUdsTransaction();

            engine.reset();

            const bool passed =
                pendingBeforeReset
                &&
                !engine.hasPendingUdsTransaction()
                &&
                !engine.hasCompletedUdsResponse();

            actual =
                passed
                ? "Runtime diagnostic transaction state cleared by reset."
                : "Runtime diagnostic reset state is invalid.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-025
// ============================================================

TestCase TestRunner::runUdsRuntimeResetCanIsolationTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_025",
        "UDS Runtime Reset CAN Isolation",
        "UDSRT-REQ-025",
        "Verify pre-reset diagnostic CAN frames do not survive SimulationEngine reset.",
        "Submit a multi-frame diagnostic request, reset before processing, then run one update.",
        "Post-reset CAN trace shall contain no 0x7E0 or 0x7E8 traffic from the old transaction.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            engine.submitUdsRequest(
                createUnsupportedMultiFrameRequest()
            );

            engine.reset();

            engine.update(
                10.0
            );

            const bool passed =
                !traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                )
                &&
                !traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                );

            actual =
                passed
                ? "No pre-reset diagnostic CAN traffic survived reset."
                : "Diagnostic CAN traffic remained after reset.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-026
// ============================================================

TestCase TestRunner::runUdsRuntimeEcuRegressionTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_026",
        "UDS Runtime Existing ECU Regression",
        "UDSRT-REQ-026",
        "Verify adding runtime diagnostics does not alter the same-step vehicle and ECU state.",
        "Run one reference engine and one engine with a diagnostic transaction for 10 ms.",
        "Vehicle and ABS runtime state shall remain equivalent.",
        [](
            std::string& actual)
        {
            SimulationEngine reference(
                LogLevel::Quiet
            );

            SimulationEngine diagnostic(
                LogLevel::Quiet
            );

            reference.reset();
            diagnostic.reset();

            reference.update(
                10.0
            );

            diagnostic.submitUdsRequest(
                createSessionRequest()
            );

            diagnostic.update(
                10.0
            );

            const VehicleState& firstVehicle =
                reference.getVehicleState();

            const VehicleState& secondVehicle =
                diagnostic.getVehicleState();

            const AbsState& firstAbs =
                reference.getAbsState();

            const AbsState& secondAbs =
                diagnostic.getAbsState();

            const bool passed =
                firstVehicle.speedKmh ==
                secondVehicle.speedKmh
                &&
                firstVehicle.engineRpm ==
                secondVehicle.engineRpm
                &&
                firstVehicle.gear ==
                secondVehicle.gear
                &&
                firstVehicle.brakeApplied ==
                secondVehicle.brakeApplied
                &&
                firstAbs.absActive ==
                secondAbs.absActive
                &&
                firstAbs.healthStatus ==
                secondAbs.healthStatus;

            actual =
                passed
                ? "Vehicle and ABS state match the non-diagnostic reference engine."
                : "Existing ECU runtime state changed when diagnostics were active.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-027
// ============================================================

TestCase TestRunner::runUdsRuntimeDiagnosticRegressionTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_027",
        "UDS Runtime Existing Diagnostic Regression",
        "UDSRT-REQ-027",
        "Verify runtime CAN diagnostics preserve the UdsServer response produced by direct processing.",
        "Process the same Extended Session request directly and through runtime CAN on independent engines.",
        "The final UDS responses shall be equivalent.",
        [](
            std::string& actual)
        {
            SimulationEngine directEngine(
                LogLevel::Quiet
            );

            SimulationEngine runtimeEngine(
                LogLevel::Quiet
            );

            directEngine.reset();
            runtimeEngine.reset();

            const UdsRequest request =
                createSessionRequest();

            const UdsResponse directResponse =
                directEngine
                .getUdsServer()
                .processRequest(
                    request
                );

            const bool completed =
                completeRuntimeTransaction(
                    runtimeEngine,
                    request
                );

            if (!completed)
            {
                actual =
                    "Runtime diagnostic transaction did not complete.";

                return false;
            }

            const bool passed =
                responseEquals(
                    directResponse,
                    runtimeEngine
                    .getCompletedUdsResponse()
                );

            actual =
                passed
                ? "Direct and runtime UDS responses are equivalent."
                : "Runtime diagnostic response regressed from direct UdsServer behavior.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-028
// ============================================================

TestCase TestRunner::runUdsRuntimeBaselineRegressionTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_028",
        "UDS Runtime Baseline Regression Sentinel",
        "UDSRT-REQ-028",
        "Provide a runtime integration regression sentinel while the complete runAll suite executes all 188 legacy tests.",
        "Execute normal vehicle traffic and one runtime diagnostic transaction.",
        "Core vehicle CAN, diagnostic CAN, statistics, and response behavior shall remain operational.",
        [](
            std::string& actual)
        {
            SimulationEngine engine(
                LogLevel::Quiet
            );

            engine.reset();

            const bool completed =
                completeRuntimeTransaction(
                    engine,
                    createSessionRequest()
                );

            const bool passed =
                completed
                &&
                traceContainsPeriodicVehicleTraffic(
                    engine
                )
                &&
                traceContainsIdentifier(
                    engine,
                    UdsRequestCanId
                )
                &&
                traceContainsIdentifier(
                    engine,
                    UdsResponseCanId
                )
                &&
                engine
                .getCanStatistics()
                .getFramesTransmitted() >
                0;

            actual =
                passed
                ? "Runtime regression sentinel passed; legacy regression evidence is supplied by the full suite."
                : "Runtime baseline regression sentinel failed.";

            return passed;
        }
    );
}

// ============================================================
// UDSRT-REQ-029
// ============================================================

TestCase TestRunner::runUdsRuntimeDeterminismTest()
{
    return executeRuntimeTest(
        "TC_UDSRT_029",
        "UDS Runtime Determinism",
        "UDSRT-REQ-029",
        "Verify identical SimulationEngine state and request produce equivalent runtime diagnostic behavior.",
        "Run identical Extended Session transactions on two independent reset engines.",
        "Responses and complete CAN traces shall match.",
        [](
            std::string& actual)
        {
            SimulationEngine first(
                LogLevel::Quiet
            );

            SimulationEngine second(
                LogLevel::Quiet
            );

            first.reset();
            second.reset();

            const UdsRequest request =
                createSessionRequest();

            const bool firstCompleted =
                completeRuntimeTransaction(
                    first,
                    request
                );

            const bool secondCompleted =
                completeRuntimeTransaction(
                    second,
                    request
                );

            if (
                !firstCompleted
                ||
                !secondCompleted
                )
            {
                actual =
                    "One or both deterministic transactions did not complete.";

                return false;
            }

            const bool passed =
                responseEquals(
                    first.getCompletedUdsResponse(),
                    second.getCompletedUdsResponse()
                )
                &&
                tracesEqual(
                    first,
                    second
                );

            actual =
                passed
                ? "Equivalent response and CAN trace produced by both engines."
                : "Runtime diagnostic behavior was not deterministic.";

            return passed;
        }
    );
}
