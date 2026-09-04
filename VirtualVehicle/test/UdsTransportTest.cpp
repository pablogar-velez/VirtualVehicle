#include "TestRunner.h"

#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "../diagnostics/DtcManager.h"
#include "../diagnostics/UdsRequest.h"
#include "../diagnostics/UdsResponse.h"
#include "../diagnostics/UdsServer.h"
#include "../diagnostics/UdsService.h"
#include "../diagnostics/UdsTransport.h"

#include "../transport/IsoTpProtocol.h"

namespace
{
    using Clock =
        std::chrono::steady_clock;

    TestCase createUdsTransportTestCase(
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
}

// ============================================================
// UDSTP-REQ-001
// ============================================================

TestCase TestRunner::runUdsTransportRequestTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_001",
            "UDS Request Transport",
            "UDSTP-REQ-001",
            "Verify a UDS request can be converted into ISO-TP CAN frames.",
            "A UDS server and UDS transport component are available.",
            "Create ISO-TP frames from a Diagnostic Session Control request.",
            "One or more valid ISO-TP request frames shall be generated."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsRequest request;

    request.serviceId =
        UdsService::DiagnosticSessionControl;

    request.payload = {
        UdsService::ExtendedSession
    };

    const auto frames =
        transport.createRequestFrames(
            request
        );

    bool passed =
        !frames.empty();

    if (passed)
    {
        const auto info =
            IsoTpProtocol::parseFrame(
                frames.front()
            );

        passed =
            info.valid;
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Valid ISO-TP request frame sequence",
        passed
        ? "Valid ISO-TP request frames generated"
        : "UDS request transport failed",
        passed
        ? "UDS request transport verified."
        : "UDS request transport verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-002
// ============================================================

TestCase TestRunner::runUdsTransportResponseTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_002",
            "UDS Response Transport",
            "UDSTP-REQ-002",
            "Verify a UDS response can be converted into ISO-TP CAN frames.",
            "A UDS transport component is available.",
            "Create ISO-TP frames from a positive UDS response.",
            "One or more valid ISO-TP response frames shall be generated."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsResponse response;

    response.positive =
        true;

    response.serviceId =
        0x50;

    response.payload = {
        UdsService::ExtendedSession
    };

    const auto frames =
        transport.createResponseFrames(
            response
        );

    bool passed =
        !frames.empty();

    if (passed)
    {
        const auto info =
            IsoTpProtocol::parseFrame(
                frames.front()
            );

        passed =
            info.valid;
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Valid ISO-TP response frame sequence",
        passed
        ? "Valid ISO-TP response frames generated"
        : "UDS response transport failed",
        passed
        ? "UDS response transport verified."
        : "UDS response transport verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-003
// ============================================================

TestCase TestRunner::runUdsTransportRequestCanIdTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_003",
            "UDS Request CAN Identifier",
            "UDSTP-REQ-003",
            "Verify default UDS request frames use CAN ID 0x7E0.",
            "Default UDS transport configuration is active.",
            "Create request frames.",
            "Every generated request frame shall use CAN ID 0x7E0."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsRequest request;

    request.serviceId =
        UdsService::ReadDataByIdentifier;

    request.payload = {
        0xF1,
        0x00
    };

    const auto frames =
        transport.createRequestFrames(
            request
        );

    const bool passed =
        transport.getRequestCanId() ==
        0x7E0
        &&
        allFramesUseIdentifier(
            frames,
            0x7E0
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Request CAN ID = 0x7E0",
        passed
        ? "All request frames used 0x7E0"
        : "Unexpected request CAN identifier",
        passed
        ? "UDS request CAN identifier verified."
        : "UDS request CAN identifier verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-004
// ============================================================

TestCase TestRunner::runUdsTransportResponseCanIdTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_004",
            "UDS Response CAN Identifier",
            "UDSTP-REQ-004",
            "Verify default UDS response frames use CAN ID 0x7E8.",
            "Default UDS transport configuration is active.",
            "Create response frames.",
            "Every generated response frame shall use CAN ID 0x7E8."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsResponse response;

    response.positive =
        true;

    response.serviceId =
        0x62;

    response.payload = {
        0xF1,
        0x00,
        0x00,
        0x64
    };

    const auto frames =
        transport.createResponseFrames(
            response
        );

    const bool passed =
        transport.getResponseCanId() ==
        0x7E8
        &&
        allFramesUseIdentifier(
            frames,
            0x7E8
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Response CAN ID = 0x7E8",
        passed
        ? "All response frames used 0x7E8"
        : "Unexpected response CAN identifier",
        passed
        ? "UDS response CAN identifier verified."
        : "UDS response CAN identifier verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-005
// ============================================================

TestCase TestRunner::runUdsTransportRequestSerializationTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_005",
            "UDS Request Serialization",
            "UDSTP-REQ-005",
            "Verify request SID and payload byte ordering through ISO-TP.",
            "A UDS request is available.",
            "Create and decode ISO-TP request frames.",
            "Decoded request shall equal the original request."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsRequest original;

    original.serviceId =
        UdsService::ReadDataByIdentifier;

    original.payload = {
        0xF1,
        0x02
    };

    const auto frames =
        transport.createRequestFrames(
            original
        );

    const UdsRequest decoded =
        transport.decodeRequestFrames(
            frames
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
        "Decoded request equals original request",
        passed
        ? "SID and payload preserved"
        : "Request serialization changed UDS data",
        passed
        ? "UDS request serialization verified."
        : "UDS request serialization verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-006
// ============================================================

TestCase TestRunner::runUdsTransportResponseSerializationTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_006",
            "UDS Response Serialization",
            "UDSTP-REQ-006",
            "Verify response SID and payload byte ordering through ISO-TP.",
            "A positive UDS response is available.",
            "Create and decode ISO-TP response frames.",
            "Decoded SID and payload shall equal the original values."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsResponse original;

    original.positive =
        true;

    original.serviceId =
        0x62;

    original.payload = {
        0xF1,
        0x00,
        0x01,
        0xF4
    };

    const auto frames =
        transport.createResponseFrames(
            original
        );

    const UdsResponse decoded =
        transport.decodeResponseFrames(
            frames
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
        "Decoded SID and payload equal original response",
        passed
        ? "Response SID and payload preserved"
        : "Response serialization changed UDS data",
        passed
        ? "UDS response serialization verified."
        : "UDS response serialization verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-007
// ============================================================

TestCase TestRunner::runUdsTransportPositiveResponseTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_007",
            "UDS Positive Response Preservation",
            "UDSTP-REQ-007",
            "Verify positive response state is preserved through ISO-TP.",
            "A positive UDS response is available.",
            "Transport and decode the response.",
            "The reconstructed response shall be positive."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsResponse original;

    original.positive =
        true;

    original.serviceId =
        0x50;

    original.payload = {
        UdsService::ExtendedSession
    };

    const auto frames =
        transport.createResponseFrames(
            original
        );

    const auto decoded =
        transport.decodeResponseFrames(
            frames
        );

    const bool passed =
        decoded.isPositive() &&
        decoded.serviceId ==
        0x50;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Positive UDS response preserved",
        passed
        ? "Response remained positive"
        : "Positive response state was not preserved",
        passed
        ? "Positive response preservation verified."
        : "Positive response preservation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-008
// ============================================================

TestCase TestRunner::runUdsTransportNegativeResponseTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_008",
            "UDS Negative Response Preservation",
            "UDSTP-REQ-008",
            "Verify SID 0x7F remains a negative UDS response.",
            "A negative UDS response is available.",
            "Transport and decode the response.",
            "The reconstructed response shall be negative with SID 0x7F."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsResponse original;

    original.positive =
        false;

    original.serviceId =
        UdsService::NegativeResponse;

    original.payload = {
        UdsService::ReadDataByIdentifier,
        UdsService::NegativeResponseCode::
            RequestOutOfRange
    };

    const auto frames =
        transport.createResponseFrames(
            original
        );

    const auto decoded =
        transport.decodeResponseFrames(
            frames
        );

    const bool passed =
        decoded.isNegative() &&
        decoded.serviceId ==
        UdsService::NegativeResponse &&
        decoded.payload ==
        original.payload;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Negative response 7F 22 31 preserved",
        passed
        ? "Negative UDS response preserved"
        : "Negative response was not reconstructed correctly",
        passed
        ? "Negative response preservation verified."
        : "Negative response preservation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-009
// ============================================================

TestCase TestRunner::runUdsTransportRequestIdentifierValidationTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_009",
            "UDS Request Identifier Validation",
            "UDSTP-REQ-009",
            "Verify request frames with an unexpected CAN ID are rejected.",
            "A valid UDS request frame sequence is available.",
            "Change the request CAN identifier before decoding.",
            "The UDS transport component shall reject the frame sequence."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsRequest request;

    request.serviceId =
        UdsService::DiagnosticSessionControl;

    request.payload = {
        UdsService::ExtendedSession
    };

    auto frames =
        transport.createRequestFrames(
            request
        );

    frames.front().arbitrationId =
        0x7E1;

    bool rejected =
        false;

    try
    {
        transport.decodeRequestFrames(
            frames
        );
    }
    catch (const std::invalid_argument&)
    {
        rejected =
            true;
    }
    catch (...)
    {
        rejected =
            false;
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        rejected,
        "Unexpected request CAN ID rejected",
        rejected
        ? "Incorrect request identifier rejected"
        : "Incorrect request identifier accepted",
        rejected
        ? "Request identifier validation verified."
        : "Request identifier validation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-010
// ============================================================

TestCase TestRunner::runUdsTransportResponseIdentifierValidationTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_010",
            "UDS Response Identifier Validation",
            "UDSTP-REQ-010",
            "Verify response frames with an unexpected CAN ID are rejected.",
            "A valid UDS response frame sequence is available.",
            "Change the response CAN identifier before decoding.",
            "The UDS transport component shall reject the frame sequence."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsResponse response;

    response.positive =
        true;

    response.serviceId =
        0x50;

    response.payload = {
        UdsService::ExtendedSession
    };

    auto frames =
        transport.createResponseFrames(
            response
        );

    frames.front().arbitrationId =
        0x7E9;

    bool rejected =
        false;

    try
    {
        transport.decodeResponseFrames(
            frames
        );
    }
    catch (const std::invalid_argument&)
    {
        rejected =
            true;
    }
    catch (...)
    {
        rejected =
            false;
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        rejected,
        "Unexpected response CAN ID rejected",
        rejected
        ? "Incorrect response identifier rejected"
        : "Incorrect response identifier accepted",
        rejected
        ? "Response identifier validation verified."
        : "Response identifier validation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-011
// ============================================================

TestCase TestRunner::runUdsTransportEndToEndTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_011",
            "UDS ISO-TP End-to-End Processing",
            "UDSTP-REQ-011",
            "Verify a UDS request can traverse the complete UDS/ISO-TP processing path.",
            "UdsServer and UdsTransport are initialized.",
            "Send Diagnostic Session Control Extended Session through UdsTransport.",
            "A positive 0x50 response for Extended Session shall be returned."
        );

    const auto start =
        Clock::now();

    DtcManager dtcManager;
    UdsServer udsServer(dtcManager);
    UdsTransport transport(udsServer);

    UdsRequest request;

    request.serviceId =
        UdsService::DiagnosticSessionControl;

    request.payload = {
        UdsService::ExtendedSession
    };

    const UdsResponse response =
        transport.processRequest(
            request
        );

    const bool passed =
        response.isPositive() &&
        response.serviceId ==
        static_cast<std::uint8_t>(
            UdsService::DiagnosticSessionControl +
            UdsService::PositiveResponseOffset
            )
        &&
        response.payload.size() ==
        1
        &&
        response.payload[0] ==
        UdsService::ExtendedSession
        &&
        udsServer.getCurrentSession() ==
        UdsService::ExtendedSession;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Positive 0x50 0x03 response and server session 0x03",
        passed
        ? "End-to-end UDS/ISO-TP processing succeeded"
        : "End-to-end UDS/ISO-TP processing produced unexpected state",
        passed
        ? "End-to-end UDS transport verified."
        : "End-to-end UDS transport verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// UDSTP-REQ-012
// ============================================================

TestCase TestRunner::runUdsTransportDeterminismTest()
{
    TestCase test =
        createUdsTransportTestCase(
            "TC_UDSTP_012",
            "UDS Transport Determinism",
            "UDSTP-REQ-012",
            "Verify identical requests and server state produce identical responses.",
            "Two independent UDS servers are initialized in identical state.",
            "Process the same ReadDataByIdentifier request through both transports.",
            "Both final UDS responses shall be identical."
        );

    const auto start =
        Clock::now();

    DtcManager firstDtcManager;
    DtcManager secondDtcManager;

    UdsServer firstServer(
        firstDtcManager
    );

    UdsServer secondServer(
        secondDtcManager
    );

    UdsVehicleData vehicleData;

    vehicleData.vehicleSpeedKmh =
        50.0f;

    vehicleData.engineRpm =
        2800;

    vehicleData.steeringAngleDeg =
        -12.5f;

    firstServer.setVehicleData(
        vehicleData
    );

    secondServer.setVehicleData(
        vehicleData
    );

    UdsTransport firstTransport(
        firstServer
    );

    UdsTransport secondTransport(
        secondServer
    );

    UdsRequest request;

    request.serviceId =
        UdsService::ReadDataByIdentifier;

    request.payload = {
        0xF1,
        0x00
    };

    const UdsResponse firstResponse =
        firstTransport.processRequest(
            request
        );

    const UdsResponse secondResponse =
        secondTransport.processRequest(
            request
        );

    const bool passed =
        responsesEqual(
            firstResponse,
            secondResponse
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Identical UDS responses",
        passed
        ? "Both transports produced identical responses"
        : "Transport responses differed",
        passed
        ? "UDS transport determinism verified."
        : "UDS transport determinism verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}