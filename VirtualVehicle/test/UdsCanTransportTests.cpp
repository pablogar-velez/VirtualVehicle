#include "TestRunner.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "../can/CanFrame.h"
#include "../can/VirtualCanBus.h"

#include "../diagnostics/DtcManager.h"
#include "../diagnostics/UdsCanTransport.h"
#include "../diagnostics/UdsRequest.h"
#include "../diagnostics/UdsResponse.h"
#include "../diagnostics/UdsServer.h"
#include "../diagnostics/UdsService.h"

namespace
{
    using Clock =
        std::chrono::steady_clock;

    TestCase createUdsCanTransportTestCase(
        const std::string& id,
        const std::string& name,
        const std::string& requirementId,
        const std::string& description,
        const std::string& precondition,
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
            precondition;

        test.stimulus =
            stimulus;

        test.expectedResult =
            expectedResult;

        return test;
    }

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

    void setResult(
        TestCase& test,
        bool passed,
        const std::string& expected,
        const std::string& actual,
        const std::string& message,
        double executionTimeMs)
    {
        test.result.status =
            passed
            ? TestStatus::Passed
            : TestStatus::Failed;

        test.result.expected =
            expected;

        test.result.actual =
            actual;

        test.result.message =
            message;

        test.result.executionTimeMs =
            executionTimeMs;
    }

    bool allFramesUseIdentifier(
        const std::vector<CanFrame>& frames,
        std::uint32_t arbitrationId)
    {
        if (frames.empty())
        {
            return false;
        }

        for (const CanFrame& frame : frames)
        {
            if (
                frame.arbitrationId !=
                arbitrationId
                )
            {
                return false;
            }
        }

        return true;
    }

    bool traceContainsIdentifier(
        const VirtualCanBus& canBus,
        std::uint32_t arbitrationId)
    {
        const auto& trace =
            canBus.getTrace();

        for (const auto& entry : trace)
        {
            if (
                entry.arbitrationId ==
                arbitrationId
                )
            {
                return true;
            }
        }

        return false;
    }

    bool framesEqual(
        const CanFrame& lhs,
        const CanFrame& rhs)
    {
        return
            lhs.arbitrationId ==
            rhs.arbitrationId
            &&
            lhs.format ==
            rhs.format
            &&
            lhs.type ==
            rhs.type
            &&
            lhs.dlc ==
            rhs.dlc
            &&
            lhs.data ==
            rhs.data;
    }

    bool frameSequencesEqual(
        const std::vector<CanFrame>& lhs,
        const std::vector<CanFrame>& rhs)
    {
        if (
            lhs.size() !=
            rhs.size()
            )
        {
            return false;
        }

        for (
            std::size_t index = 0;
            index < lhs.size();
            ++index
            )
        {
            if (
                !framesEqual(
                    lhs[index],
                    rhs[index]
                )
                )
            {
                return false;
            }
        }

        return true;
    }

    bool responsesEqual(
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
}

// ============================================================
// UDSCAN-REQ-001
// ============================================================

TestCase TestRunner::runUdsCanTransportComponentTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_001",
            "UDS CAN Transport Component",
            "UDSCAN-REQ-001",
            "Verify that UDS diagnostic traffic can be processed through VirtualCanBus.",
            "VirtualCanBus, UdsServer, and UdsCanTransport are initialized.",
            "Process a Diagnostic Session Control request.",
            "A valid UDS response shall be produced through the CAN transport."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const UdsRequest request =
        createSessionRequest();

    const auto result =
        transport.processRequest(
            request
        );

    const bool passed =
        result.response.isPositive()
        &&
        !result.requestFrames.empty()
        &&
        !result.responseFrames.empty();

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Valid request and response transported through VirtualCanBus",
        passed
        ? "UDS CAN transaction completed"
        : "UDS CAN transaction failed",
        passed
        ? "UDS CAN transport component verified."
        : "UDS CAN transport component verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-002
// ============================================================

TestCase TestRunner::runUdsCanRequestTransmissionTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_002",
            "UDS CAN Request Transmission",
            "UDSCAN-REQ-002",
            "Verify ISO-TP request frames are transmitted through VirtualCanBus.",
            "UDS CAN transport is initialized.",
            "Process a UDS request.",
            "VirtualCanBus trace shall contain request traffic."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const bool passed =
        !result.requestFrames.empty()
        &&
        traceContainsIdentifier(
            canBus,
            0x7E0
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Request frames transmitted through VirtualCanBus",
        passed
        ? "Request CAN traffic recorded"
        : "Request CAN traffic not recorded",
        passed
        ? "UDS CAN request transmission verified."
        : "UDS CAN request transmission failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-003
// ============================================================

TestCase TestRunner::runUdsCanResponseTransmissionTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_003",
            "UDS CAN Response Transmission",
            "UDSCAN-REQ-003",
            "Verify ISO-TP response frames are transmitted through VirtualCanBus.",
            "UDS CAN transport is initialized.",
            "Process a UDS request.",
            "VirtualCanBus trace shall contain response traffic."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const bool passed =
        !result.responseFrames.empty()
        &&
        traceContainsIdentifier(
            canBus,
            0x7E8
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Response frames transmitted through VirtualCanBus",
        passed
        ? "Response CAN traffic recorded"
        : "Response CAN traffic not recorded",
        passed
        ? "UDS CAN response transmission verified."
        : "UDS CAN response transmission failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-004
// ============================================================

TestCase TestRunner::runUdsCanRequestIdentifierTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_004",
            "UDS CAN Request Identifier",
            "UDSCAN-REQ-004",
            "Verify diagnostic requests transmitted through VirtualCanBus use CAN ID 0x7E0.",
            "Default diagnostic addressing is configured.",
            "Process a UDS request.",
            "All received request frames shall use identifier 0x7E0."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const bool passed =
        allFramesUseIdentifier(
            result.requestFrames,
            0x7E0
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "All request frames use CAN ID 0x7E0",
        passed
        ? "Request ID = 0x7E0"
        : "Unexpected request CAN ID",
        passed
        ? "Diagnostic request identifier verified."
        : "Diagnostic request identifier verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-005
// ============================================================

TestCase TestRunner::runUdsCanResponseIdentifierTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_005",
            "UDS CAN Response Identifier",
            "UDSCAN-REQ-005",
            "Verify diagnostic responses transmitted through VirtualCanBus use CAN ID 0x7E8.",
            "Default diagnostic addressing is configured.",
            "Process a UDS request.",
            "All received response frames shall use identifier 0x7E8."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const bool passed =
        allFramesUseIdentifier(
            result.responseFrames,
            0x7E8
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "All response frames use CAN ID 0x7E8",
        passed
        ? "Response ID = 0x7E8"
        : "Unexpected response CAN ID",
        passed
        ? "Diagnostic response identifier verified."
        : "Diagnostic response identifier verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-006
// ============================================================

TestCase TestRunner::runUdsCanRequestReassemblyTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_006",
            "UDS CAN Request Reassembly",
            "UDSCAN-REQ-006",
            "Verify request frames received from VirtualCanBus can be reconstructed into the original UDS request.",
            "A diagnostic transaction has completed.",
            "Decode the request frames received through VirtualCanBus.",
            "The reconstructed request shall equal the original request."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const UdsRequest original =
        createSessionRequest();

    const auto result =
        transport.processRequest(
            original
        );

    const UdsRequest decoded =
        transport
        .getUdsTransport()
        .decodeRequestFrames(
            result.requestFrames
        );

    const bool passed =
        decoded.serviceId ==
        original.serviceId
        &&
        decoded.payload ==
        original.payload;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Reconstructed request equals original request",
        passed
        ? "Request successfully reconstructed"
        : "Request reconstruction mismatch",
        passed
        ? "UDS CAN request reassembly verified."
        : "UDS CAN request reassembly failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-007
// ============================================================

TestCase TestRunner::runUdsCanResponseReassemblyTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_007",
            "UDS CAN Response Reassembly",
            "UDSCAN-REQ-007",
            "Verify response frames received from VirtualCanBus can be reconstructed.",
            "A diagnostic transaction has completed.",
            "Decode the response frames received through VirtualCanBus.",
            "The reconstructed response shall equal the transaction response."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const UdsResponse decoded =
        transport
        .getUdsTransport()
        .decodeResponseFrames(
            result.responseFrames
        );

    const bool passed =
        responsesEqual(
            decoded,
            result.response
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Reconstructed response equals final transaction response",
        passed
        ? "Response successfully reconstructed"
        : "Response reconstruction mismatch",
        passed
        ? "UDS CAN response reassembly verified."
        : "UDS CAN response reassembly failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-008
// ============================================================

TestCase TestRunner::runUdsCanEndToEndTransactionTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_008",
            "UDS CAN End-to-End Transaction",
            "UDSCAN-REQ-008",
            "Verify complete diagnostic client to CAN bus to UDS server and back processing.",
            "UDS CAN transport is initialized.",
            "Request Extended Diagnostic Session.",
            "A positive 0x50 response with session 0x03 shall return."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const bool passed =
        result.response.isPositive()
        &&
        result.response.serviceId ==
        static_cast<std::uint8_t>(
            UdsService::
            DiagnosticSessionControl
            +
            UdsService::
            PositiveResponseOffset
            )
        &&
        result.response.payload.size() ==
        1
        &&
        result.response.payload[0] ==
        UdsService::ExtendedSession
        &&
        udsServer.getCurrentSession() ==
        UdsService::ExtendedSession;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Positive response 0x50 0x03",
        passed
        ? "Complete CAN diagnostic transaction succeeded"
        : "Unexpected end-to-end diagnostic result",
        passed
        ? "End-to-end UDS CAN transaction verified."
        : "End-to-end UDS CAN transaction failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-009
// ============================================================

TestCase TestRunner::runUdsCanTraceEvidenceTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_009",
            "UDS CAN Trace Evidence",
            "UDSCAN-REQ-009",
            "Verify request and response diagnostic traffic is recorded in the CAN trace.",
            "VirtualCanBus trace is initially empty.",
            "Complete one UDS CAN transaction.",
            "CAN trace shall contain both 0x7E0 and 0x7E8 entries."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    transport.processRequest(
        createSessionRequest()
    );

    const bool passed =
        traceContainsIdentifier(
            canBus,
            0x7E0
        )
        &&
        traceContainsIdentifier(
            canBus,
            0x7E8
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "CAN trace contains request 0x7E0 and response 0x7E8",
        passed
        ? "Both diagnostic identifiers recorded"
        : "Diagnostic CAN trace incomplete",
        passed
        ? "Diagnostic CAN trace evidence verified."
        : "Diagnostic CAN trace evidence failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-010
// ============================================================

TestCase TestRunner::runUdsCanTimingTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_010",
            "UDS CAN Timing",
            "UDSCAN-REQ-010",
            "Verify diagnostic CAN frames participate in VirtualCanBus timing.",
            "VirtualCanBus is idle.",
            "Start a diagnostic transaction at 10 ms.",
            "Transaction completion time shall be greater than start time."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest(),
            10.0
        );

    const bool passed =
        result.startTimeMs ==
        10.0
        &&
        result.completionTimeMs >
        result.startTimeMs
        &&
        canBus.getBusyUntilMs() ==
        result.completionTimeMs;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Completion time > 10 ms and equals bus busy-until time",
        passed
        ? "Diagnostic frames consumed CAN transmission time"
        : "Diagnostic timing evidence invalid",
        passed
        ? "UDS CAN timing verified."
        : "UDS CAN timing verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-011
// ============================================================

TestCase TestRunner::runUdsCanTransactionEvidenceTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_011",
            "UDS CAN Transaction Evidence",
            "UDSCAN-REQ-011",
            "Verify transaction results expose request frames, response frames, final response, and timing.",
            "UDS CAN transport is initialized.",
            "Complete one diagnostic transaction.",
            "All required transaction evidence shall be available."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest(),
            5.0
        );

    const bool passed =
        !result.requestFrames.empty()
        &&
        !result.responseFrames.empty()
        &&
        result.response.serviceId !=
        0
        &&
        result.startTimeMs ==
        5.0
        &&
        result.completionTimeMs >
        result.startTimeMs;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Request frames, response frames, UDS response, and timing evidence available",
        passed
        ? "Complete transaction evidence available"
        : "Transaction evidence incomplete",
        passed
        ? "UDS CAN transaction evidence verified."
        : "UDS CAN transaction evidence failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-012
// ============================================================

TestCase TestRunner::runUdsCanPositiveResponseTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_012",
            "UDS CAN Positive Response Preservation",
            "UDSCAN-REQ-012",
            "Verify a positive UDS response remains positive after CAN transport.",
            "A supported diagnostic request is available.",
            "Send Diagnostic Session Control through VirtualCanBus.",
            "Final UDS response shall remain positive."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    const auto result =
        transport.processRequest(
            createSessionRequest()
        );

    const bool passed =
        result.response.isPositive()
        &&
        result.response.serviceId ==
        0x50;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Positive response SID 0x50",
        passed
        ? "Positive response preserved"
        : "Positive response was not preserved",
        passed
        ? "Positive UDS CAN response preservation verified."
        : "Positive UDS CAN response preservation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-013
// ============================================================

TestCase TestRunner::runUdsCanNegativeResponseTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_013",
            "UDS CAN Negative Response Preservation",
            "UDSCAN-REQ-013",
            "Verify a negative UDS response remains negative after CAN transport.",
            "UDS CAN transport is initialized.",
            "Send an unsupported UDS service.",
            "Final response shall use negative response SID 0x7F."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

    UdsRequest request;

    request.serviceId =
        0x99;

    const auto result =
        transport.processRequest(
            request
        );

    const bool passed =
        result.response.isNegative()
        &&
        result.response.serviceId ==
        UdsService::NegativeResponse
        &&
        result.response.payload.size() ==
        2
        &&
        result.response.payload[0] ==
        0x99
        &&
        result.response.payload[1] ==
        UdsService::
        NegativeResponseCode::
        ServiceNotSupported;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Negative response 0x7F 0x99 0x11",
        passed
        ? "Negative response preserved"
        : "Negative response transport mismatch",
        passed
        ? "Negative UDS CAN response preservation verified."
        : "Negative UDS CAN response preservation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-014
// ============================================================

TestCase TestRunner::runUdsCanMultiFrameTransportTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_014",
            "UDS CAN Multi-Frame Transport",
            "UDSCAN-REQ-014",
            "Verify a UDS request larger than one ISO-TP Single Frame is transported through VirtualCanBus.",
            "UDS CAN transport is initialized.",
            "Send an unsupported service request containing 20 payload bytes.",
            "The request shall use multiple CAN frames and still reach UdsServer."
        );

    const auto start =
        Clock::now();

    VirtualCanBus canBus;

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);

    UdsCanTransport transport(
        canBus,
        udsServer
    );

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

    const auto result =
        transport.processRequest(
            request
        );

    const bool passed =
        result.requestFrames.size() >
        1
        &&
        allFramesUseIdentifier(
            result.requestFrames,
            0x7E0
        )
        &&
        result.response.isNegative()
        &&
        result.response.serviceId ==
        UdsService::NegativeResponse
        &&
        result.response.payload.size() ==
        2
        &&
        result.response.payload[0] ==
        0x99
        &&
        result.response.payload[1] ==
        UdsService::
        NegativeResponseCode::
        ServiceNotSupported;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Multi-frame request reaches UdsServer and returns NRC 0x11",
        passed
        ? "Multi-frame diagnostic request transported successfully"
        : "Multi-frame diagnostic transport failed",
        passed
        ? "Multi-frame UDS CAN transport verified."
        : "Multi-frame UDS CAN transport failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSCAN-REQ-015
// ============================================================

TestCase TestRunner::runUdsCanDeterminismTest()
{
    TestCase test =
        createUdsCanTransportTestCase(
            "TC_UDSCAN_015",
            "UDS CAN Transaction Determinism",
            "UDSCAN-REQ-015",
            "Verify identical initial state and request produce equivalent CAN diagnostic transactions.",
            "Two independent buses and UDS servers are initialized identically.",
            "Execute the same request at the same simulation time.",
            "Request frames, response frames, UDS response, and completion time shall match."
        );

    const auto start =
        Clock::now();

    VirtualCanBus firstBus;
    VirtualCanBus secondBus;

    DtcManager firstDtcManager;
    DtcManager secondDtcManager;

    UdsServer firstServer(
        firstDtcManager
    );

    UdsServer secondServer(
        secondDtcManager
    );

    UdsCanTransport firstTransport(
        firstBus,
        firstServer
    );

    UdsCanTransport secondTransport(
        secondBus,
        secondServer
    );

    const UdsRequest request =
        createSessionRequest();

    const auto firstResult =
        firstTransport.processRequest(
            request,
            25.0
        );

    const auto secondResult =
        secondTransport.processRequest(
            request,
            25.0
        );

    const bool passed =
        frameSequencesEqual(
            firstResult.requestFrames,
            secondResult.requestFrames
        )
        &&
        frameSequencesEqual(
            firstResult.responseFrames,
            secondResult.responseFrames
        )
        &&
        responsesEqual(
            firstResult.response,
            secondResult.response
        )
        &&
        firstResult.startTimeMs ==
        secondResult.startTimeMs
        &&
        firstResult.completionTimeMs ==
        secondResult.completionTimeMs;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Equivalent CAN diagnostic transactions",
        passed
        ? "Both transactions produced identical results"
        : "Diagnostic transactions differed",
        passed
        ? "UDS CAN transaction determinism verified."
        : "UDS CAN transaction determinism failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}