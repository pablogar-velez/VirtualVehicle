#include "TestRunner.h"

#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "../diagnostics/DtcDefinitions.h"
#include "../diagnostics/DtcManager.h"
#include "../diagnostics/UdsRequest.h"
#include "../diagnostics/UdsResponse.h"
#include "../diagnostics/UdsServer.h"
#include "../diagnostics/UdsService.h"

#include "../sensor/SensorFault.h"
#include "../simulation/SimulationEngine.h"

// ============================================================
// Local helpers
// ============================================================

namespace
{
    TestCase createUdsTest(
        const std::string& id,
        const std::string& name,
        const std::string& description,
        const std::string& requirementId,
        const std::string& precondition,
        const std::string& stimulus,
        const std::string& expectedResult)
    {
        TestCase test;

        test.id = id;
        test.name = name;
        test.description = description;
        test.requirementId = requirementId;
        test.precondition = precondition;
        test.stimulus = stimulus;
        test.expectedResult = expectedResult;

        return test;
    }

    TestCase completeUdsTest(
        TestCase test,
        bool passed,
        const std::string& expected,
        const std::string& actual,
        const std::string& message)
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

        return test;
    }

    bool responseEquals(
        const UdsResponse& left,
        const UdsResponse& right)
    {
        return
            left.positive == right.positive &&
            left.serviceId == right.serviceId &&
            left.payload == right.payload;
    }

    std::uint16_t decodeUnsigned16(
        const std::vector<std::uint8_t>& payload,
        std::size_t index)
    {
        return static_cast<std::uint16_t>(
            (
                static_cast<std::uint16_t>(
                    payload[index]
                    ) << 8
                )
            |
            payload[index + 1]
            );
    }

    std::int16_t decodeSigned16(
        const std::vector<std::uint8_t>& payload,
        std::size_t index)
    {
        const std::uint16_t raw =
            decodeUnsigned16(
                payload,
                index
            );

        return static_cast<std::int16_t>(
            raw
            );
    }

    bool containsDtc(
        const UdsResponse& response,
        std::uint32_t code)
    {
        if (
            !response.isPositive() ||
            response.payload.size() < 2
            )
        {
            return false;
        }

        for (
            std::size_t index = 2;
            index + 2 < response.payload.size();
            index += 3
            )
        {
            const std::uint32_t decodedCode =
                (
                    static_cast<std::uint32_t>(
                        response.payload[index]
                        ) << 16
                    )
                |
                (
                    static_cast<std::uint32_t>(
                        response.payload[index + 1]
                        ) << 8
                    )
                |
                static_cast<std::uint32_t>(
                    response.payload[index + 2]
                    );

            if (decodedCode == code)
            {
                return true;
            }
        }

        return false;
    }
}

// ============================================================
// TC_UDS_001
// UDS-REQ-001
// ============================================================

TestCase TestRunner::runUdsRequestRepresentationTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_001",
            "UDS Request Representation",
            "Verify that a UDS request stores a service identifier and payload.",
            "UDS-REQ-001",
            "UDS request structure is available.",
            "Create a request with SID 0x10 and payload 0x03.",
            "The request shall retain SID 0x10 and payload 0x03."
        );

    UdsRequest request;

    request.serviceId =
        UdsService::DiagnosticSessionControl;

    request.payload =
    {
        UdsService::ExtendedSession
    };

    const bool passed =
        request.serviceId == 0x10 &&
        request.payload.size() == 1 &&
        request.payload[0] == 0x03;

    return completeUdsTest(
        test,
        passed,
        "SID=0x10, Payload={0x03}",
        passed
        ? "SID=0x10, Payload={0x03}"
        : "Request representation mismatch",
        "UDS request representation verified."
    );
}

// ============================================================
// TC_UDS_002
// UDS-REQ-002
// ============================================================

TestCase TestRunner::runUdsResponseRepresentationTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_002",
            "UDS Response Representation",
            "Verify positive/negative indication, SID, and payload storage.",
            "UDS-REQ-002",
            "UDS response structure is available.",
            "Create a positive response.",
            "Response shall expose positive state, SID, and payload."
        );

    UdsResponse response;

    response.positive = true;
    response.serviceId = 0x50;
    response.payload = { 0x03 };

    const bool passed =
        response.isPositive() &&
        !response.isNegative() &&
        response.serviceId == 0x50 &&
        response.payload.size() == 1 &&
        response.payload[0] == 0x03;

    return completeUdsTest(
        test,
        passed,
        "Positive response SID=0x50 payload={0x03}",
        passed
        ? "Positive response represented correctly"
        : "Response representation mismatch",
        "UDS response representation verified."
    );
}

// ============================================================
// TC_UDS_003
// UDS-REQ-003
// ============================================================

TestCase TestRunner::runUdsServerProcessingTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_003",
            "UDS Server Request Processing",
            "Verify that UdsServer processes a diagnostic request.",
            "UDS-REQ-003",
            "A DTC manager and UDS server exist.",
            "Send Diagnostic Session Control request 0x10 0x01.",
            "Server shall produce a positive response."
        );

    DtcManager dtcManager;
    UdsServer server(dtcManager);

    UdsRequest request;
    request.serviceId = 0x10;
    request.payload = { 0x01 };

    const UdsResponse response =
        server.processRequest(request);

    const bool passed =
        response.isPositive() &&
        response.serviceId == 0x50;

    return completeUdsTest(
        test,
        passed,
        "Positive response from UdsServer",
        passed
        ? "Positive response SID=0x50"
        : "UDS request processing failed",
        "UDS server processing verified."
    );
}

// ============================================================
// TC_UDS_004
// UDS-REQ-004
// ============================================================

TestCase TestRunner::runUdsPositiveResponseSidTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_004",
            "UDS Positive Response SID",
            "Verify that positive response SID equals request SID plus 0x40.",
            "UDS-REQ-004",
            "UDS server initialized.",
            "Send request SID 0x10.",
            "Positive response SID shall be 0x50."
        );

    DtcManager manager;
    UdsServer server(manager);

    UdsRequest request{
        0x10,
        { 0x01 }
    };

    const UdsResponse response =
        server.processRequest(request);

    const bool passed =
        response.isPositive() &&
        response.serviceId ==
        static_cast<std::uint8_t>(
            request.serviceId + 0x40
            );

    return completeUdsTest(
        test,
        passed,
        "0x10 + 0x40 = 0x50",
        passed
        ? "Response SID=0x50"
        : "Positive response SID mismatch",
        "Positive response SID behavior verified."
    );
}

// ============================================================
// TC_UDS_005
// UDS-REQ-005
// ============================================================

TestCase TestRunner::runUdsNegativeResponseFormatTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_005",
            "UDS Negative Response Format",
            "Verify the simplified negative response format.",
            "UDS-REQ-005",
            "UDS server initialized.",
            "Send unsupported service SID 0x99.",
            "Response shall contain 0x7F, original SID and NRC."
        );

    DtcManager manager;
    UdsServer server(manager);

    UdsRequest request{
        0x99,
        {}
    };

    const UdsResponse response =
        server.processRequest(request);

    const bool passed =
        response.isNegative() &&
        response.serviceId == 0x7F &&
        response.payload.size() == 2 &&
        response.payload[0] == 0x99 &&
        response.payload[1] ==
        UdsService::NegativeResponseCode::
        ServiceNotSupported;

    return completeUdsTest(
        test,
        passed,
        "SID=0x7F, payload={0x99,0x11}",
        passed
        ? "Negative response format correct"
        : "Negative response format mismatch",
        "Negative response format verified."
    );
}

// ============================================================
// TC_UDS_006
// UDS-REQ-006
// ============================================================

TestCase TestRunner::runUdsUnsupportedServiceTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_006",
            "UDS Unsupported Service",
            "Verify ServiceNotSupported response for unknown SID.",
            "UDS-REQ-006",
            "UDS server initialized.",
            "Send SID 0x99.",
            "Server shall return NRC 0x11."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x99,
                {}
            }
        );

    const bool passed =
        response.isNegative() &&
        response.payload.size() == 2 &&
        response.payload[1] == 0x11;

    return completeUdsTest(
        test,
        passed,
        "NRC 0x11",
        passed
        ? "NRC 0x11 returned"
        : "Incorrect NRC",
        "Unsupported service handling verified."
    );
}

// ============================================================
// TC_UDS_007
// UDS-REQ-007
// ============================================================

TestCase TestRunner::runUdsDiagnosticSessionControlSupportTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_007",
            "Diagnostic Session Control Support",
            "Verify service 0x10 support.",
            "UDS-REQ-007",
            "UDS server initialized.",
            "Send 0x10 0x01.",
            "Server shall positively respond with SID 0x50."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                UdsService::DiagnosticSessionControl,
                { UdsService::DefaultSession }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.serviceId == 0x50;

    return completeUdsTest(
        test,
        passed,
        "Positive SID 0x50",
        passed
        ? "Service 0x10 supported"
        : "Service 0x10 rejected",
        "Diagnostic Session Control support verified."
    );
}

// ============================================================
// TC_UDS_008
// UDS-REQ-008
// ============================================================

TestCase TestRunner::runUdsDefaultSessionTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_008",
            "Default Diagnostic Session",
            "Verify default diagnostic session identifier 0x01.",
            "UDS-REQ-008",
            "New UDS server.",
            "Read initial diagnostic session.",
            "Current session shall equal 0x01."
        );

    DtcManager manager;
    UdsServer server(manager);

    const bool passed =
        server.getCurrentSession() ==
        UdsService::DefaultSession;

    return completeUdsTest(
        test,
        passed,
        "Default Session=0x01",
        passed
        ? "Current Session=0x01"
        : "Unexpected initial session",
        "Default session verified."
    );
}

// ============================================================
// TC_UDS_009
// UDS-REQ-009
// ============================================================

TestCase TestRunner::runUdsExtendedSessionTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_009",
            "Extended Diagnostic Session",
            "Verify extended diagnostic session 0x03 support.",
            "UDS-REQ-009",
            "UDS server in default session.",
            "Request session 0x03.",
            "Request shall be accepted."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x10,
                { 0x03 }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.payload.size() == 1 &&
        response.payload[0] == 0x03;

    return completeUdsTest(
        test,
        passed,
        "Extended Session 0x03 accepted",
        passed
        ? "Extended Session 0x03 accepted"
        : "Extended session request failed",
        "Extended session support verified."
    );
}

// ============================================================
// TC_UDS_010
// UDS-REQ-010
// ============================================================

TestCase TestRunner::runUdsSessionStateUpdateTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_010",
            "Diagnostic Session State Update",
            "Verify successful session request updates server state.",
            "UDS-REQ-010",
            "Server starts in Default Session.",
            "Request Extended Session.",
            "Current session shall become 0x03."
        );

    DtcManager manager;
    UdsServer server(manager);

    server.processRequest(
        UdsRequest{
            0x10,
            { 0x03 }
        }
    );

    const bool passed =
        server.getCurrentSession() == 0x03;

    return completeUdsTest(
        test,
        passed,
        "Current Session=0x03",
        passed
        ? "Current Session=0x03"
        : "Session state did not update",
        "Session state update verified."
    );
}

// ============================================================
// TC_UDS_011
// UDS-REQ-011
// ============================================================

TestCase TestRunner::runUdsUnsupportedSessionTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_011",
            "Unsupported Diagnostic Session",
            "Verify unsupported session produces negative response.",
            "UDS-REQ-011",
            "UDS server initialized.",
            "Request session 0x7E.",
            "Server shall return SubFunctionNotSupported."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x10,
                { 0x7E }
            }
        );

    const bool passed =
        response.isNegative() &&
        response.payload.size() == 2 &&
        response.payload[1] ==
        UdsService::NegativeResponseCode::
        SubFunctionNotSupported;

    return completeUdsTest(
        test,
        passed,
        "Negative response NRC=0x12",
        passed
        ? "NRC=0x12"
        : "Unsupported session response mismatch",
        "Unsupported session handling verified."
    );
}

// ============================================================
// TC_UDS_012
// UDS-REQ-012
// ============================================================

TestCase TestRunner::runUdsReadDtcInformationSupportTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_012",
            "Read DTC Information Support",
            "Verify service 0x19 support.",
            "UDS-REQ-012",
            "UDS server initialized.",
            "Send Read DTC request using supported subfunction.",
            "Server shall return positive SID 0x59."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x19,
                { UdsService::ReportActiveDtcs }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.serviceId == 0x59;

    return completeUdsTest(
        test,
        passed,
        "Positive SID 0x59",
        passed
        ? "Positive SID 0x59"
        : "Service 0x19 not processed",
        "Read DTC Information support verified."
    );
}

// ============================================================
// TC_UDS_013
// UDS-REQ-013
// ============================================================

TestCase TestRunner::runUdsReadActiveDtcsTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_013",
            "Read Active DTCs",
            "Verify active DTC collection is returned by service 0x19.",
            "UDS-REQ-013",
            "Front-left DTC is active.",
            "Request active DTC information.",
            "Response shall report one active DTC."
        );

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        10.0
    );

    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x19,
                { UdsService::ReportActiveDtcs }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.payload.size() >= 5 &&
        response.payload[1] == 1;

    return completeUdsTest(
        test,
        passed,
        "One active DTC",
        passed
        ? "Active DTC count=1"
        : "Active DTC count mismatch",
        "Active DTC reporting verified."
    );
}

// ============================================================
// TC_UDS_014
// UDS-REQ-014
// ============================================================

TestCase TestRunner::runUdsFrontLeftDtcReportingTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_014",
            "Front-Left DTC Reporting",
            "Verify DTC 0xC0031 is returned when active.",
            "UDS-REQ-014",
            "DTC C0031 active.",
            "Request active DTC information.",
            "Response shall include C0031."
        );

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        10.0
    );

    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x19,
                { UdsService::ReportActiveDtcs }
            }
        );

    const bool passed =
        containsDtc(
            response,
            DtcDefinitions::FrontLeftWheelSpeedSensor
        );

    return completeUdsTest(
        test,
        passed,
        "DTC 0xC0031 present",
        passed
        ? "DTC 0xC0031 present"
        : "DTC 0xC0031 missing",
        "Front-left DTC reporting verified."
    );
}

// ============================================================
// TC_UDS_015
// UDS-REQ-015
// ============================================================

TestCase TestRunner::runUdsFrontRightDtcReportingTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_015",
            "Front-Right DTC Reporting",
            "Verify DTC 0xC0034 is returned when active.",
            "UDS-REQ-015",
            "DTC C0034 active.",
            "Request active DTC information.",
            "Response shall include C0034."
        );

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::FrontRightWheelSpeedSensor,
        10.0
    );

    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x19,
                { UdsService::ReportActiveDtcs }
            }
        );

    const bool passed =
        containsDtc(
            response,
            DtcDefinitions::FrontRightWheelSpeedSensor
        );

    return completeUdsTest(
        test,
        passed,
        "DTC 0xC0034 present",
        passed
        ? "DTC 0xC0034 present"
        : "DTC 0xC0034 missing",
        "Front-right DTC reporting verified."
    );
}

// ============================================================
// TC_UDS_016
// UDS-REQ-016
// ============================================================

TestCase TestRunner::runUdsNoActiveDtcResponseTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_016",
            "No Active DTC Response",
            "Verify successful response when no active DTC exists.",
            "UDS-REQ-016",
            "DTC manager contains no active faults.",
            "Request active DTC information.",
            "Positive response shall report zero active DTCs."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x19,
                { UdsService::ReportActiveDtcs }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.payload.size() == 2 &&
        response.payload[1] == 0;

    return completeUdsTest(
        test,
        passed,
        "Active DTC count=0",
        passed
        ? "Active DTC count=0"
        : "No-DTC response mismatch",
        "No-active-DTC response verified."
    );
}

// ============================================================
// TC_UDS_017
// UDS-REQ-017
// ============================================================

TestCase TestRunner::runUdsClearDiagnosticInformationSupportTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_017",
            "Clear Diagnostic Information Support",
            "Verify service 0x14 support.",
            "UDS-REQ-017",
            "UDS server initialized.",
            "Send 0x14 FF FF FF.",
            "Server shall return positive SID 0x54."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x14,
                { 0xFF, 0xFF, 0xFF }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.serviceId == 0x54;

    return completeUdsTest(
        test,
        passed,
        "Positive SID 0x54",
        passed
        ? "Positive SID 0x54"
        : "Clear Diagnostic Information failed",
        "Clear Diagnostic Information support verified."
    );
}

// ============================================================
// TC_UDS_018
// UDS-REQ-018
// ============================================================

TestCase TestRunner::runUdsClearAllDtcsTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_018",
            "Clear All DTCs Through UDS",
            "Verify service 0x14 clears all registered DTC records.",
            "UDS-REQ-018",
            "Both wheel-speed DTCs are active.",
            "Send 0x14 FF FF FF.",
            "Both DTCs shall no longer be active."
        );

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        10.0
    );

    manager.reportFault(
        DtcDefinitions::FrontRightWheelSpeedSensor,
        10.0
    );

    UdsServer server(manager);

    server.processRequest(
        UdsRequest{
            0x14,
            { 0xFF, 0xFF, 0xFF }
        }
    );

    const bool passed =
        !manager.isDtcActive(
            DtcDefinitions::FrontLeftWheelSpeedSensor
        ) &&
        !manager.isDtcActive(
            DtcDefinitions::FrontRightWheelSpeedSensor
        );

    return completeUdsTest(
        test,
        passed,
        "Both DTCs cleared",
        passed
        ? "Both DTCs cleared"
        : "One or more DTCs remain active",
        "UDS Clear All integration verified."
    );
}

// ============================================================
// TC_UDS_019
// UDS-REQ-019
// ============================================================

TestCase TestRunner::runUdsClearDiagnosticResponseTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_019",
            "Clear Diagnostic Positive Response",
            "Verify successful clear returns a positive response.",
            "UDS-REQ-019",
            "UDS server initialized.",
            "Send valid Clear Diagnostic Information request.",
            "Response shall be positive with SID 0x54."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x14,
                { 0xFF, 0xFF, 0xFF }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.serviceId == 0x54;

    return completeUdsTest(
        test,
        passed,
        "Positive SID=0x54",
        passed
        ? "Positive SID=0x54"
        : "Clear response was not positive",
        "Clear diagnostic response verified."
    );
}

// ============================================================
// TC_UDS_020
// UDS-REQ-020
// ============================================================

TestCase TestRunner::runUdsReadDataByIdentifierSupportTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_020",
            "Read Data By Identifier Support",
            "Verify service 0x22 support.",
            "UDS-REQ-020",
            "UDS vehicle data available.",
            "Read DID F100.",
            "Server shall return positive SID 0x62."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x22,
                { 0xF1, 0x00 }
            }
        );

    const bool passed =
        response.isPositive() &&
        response.serviceId == 0x62;

    return completeUdsTest(
        test,
        passed,
        "Positive SID 0x62",
        passed
        ? "Positive SID 0x62"
        : "Read Data By Identifier failed",
        "Read Data By Identifier support verified."
    );
}

// ============================================================
// TC_UDS_021
// UDS-REQ-021
// ============================================================

TestCase TestRunner::runUdsVehicleSpeedDidTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_021",
            "Vehicle Speed DID Definition",
            "Verify vehicle speed DID equals 0xF100.",
            "UDS-REQ-021",
            "UDS service definitions loaded.",
            "Inspect VehicleSpeedDid constant.",
            "VehicleSpeedDid shall equal 0xF100."
        );

    const bool passed =
        UdsService::VehicleSpeedDid ==
        0xF100;

    return completeUdsTest(
        test,
        passed,
        "VehicleSpeedDid=0xF100",
        passed
        ? "VehicleSpeedDid=0xF100"
        : "Vehicle speed DID mismatch",
        "Vehicle speed DID verified."
    );
}

// ============================================================
// TC_UDS_022
// UDS-REQ-022
// ============================================================

TestCase TestRunner::runUdsVehicleSpeedReadoutTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_022",
            "Vehicle Speed DID Readout",
            "Verify F100 returns encoded vehicle speed.",
            "UDS-REQ-022",
            "Vehicle speed is 82.5 km/h.",
            "Read DID F100.",
            "Returned value shall decode to 82.5 km/h."
        );

    DtcManager manager;
    UdsServer server(manager);

    UdsVehicleData data;
    data.vehicleSpeedKmh = 82.5f;

    server.setVehicleData(data);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x22,
                { 0xF1, 0x00 }
            }
        );

    bool passed =
        response.isPositive() &&
        response.payload.size() == 4;

    float decodedSpeed = 0.0f;

    if (passed)
    {
        decodedSpeed =
            static_cast<float>(
                decodeUnsigned16(
                    response.payload,
                    2
                )
                ) / 10.0f;

        passed =
            std::fabs(
                decodedSpeed - 82.5f
            ) < 0.01f;
    }

    std::ostringstream actual;
    actual << decodedSpeed << " km/h";

    return completeUdsTest(
        test,
        passed,
        "82.5 km/h",
        actual.str(),
        "Vehicle speed DID readout verified."
    );
}

// ============================================================
// TC_UDS_023
// UDS-REQ-023
// ============================================================

TestCase TestRunner::runUdsEngineSpeedDidTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_023",
            "Engine RPM DID Definition",
            "Verify engine RPM DID equals 0xF101.",
            "UDS-REQ-023",
            "UDS definitions loaded.",
            "Inspect EngineRpmDid.",
            "EngineRpmDid shall equal 0xF101."
        );

    const bool passed =
        UdsService::EngineRpmDid ==
        0xF101;

    return completeUdsTest(
        test,
        passed,
        "EngineRpmDid=0xF101",
        passed
        ? "EngineRpmDid=0xF101"
        : "Engine RPM DID mismatch",
        "Engine RPM DID verified."
    );
}

// ============================================================
// TC_UDS_024
// UDS-REQ-024
// ============================================================

TestCase TestRunner::runUdsEngineSpeedReadoutTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_024",
            "Engine RPM DID Readout",
            "Verify F101 returns current engine RPM.",
            "UDS-REQ-024",
            "Engine RPM is 2140.",
            "Read DID F101.",
            "Returned RPM shall equal 2140."
        );

    DtcManager manager;
    UdsServer server(manager);

    UdsVehicleData data;
    data.engineRpm = 2140;

    server.setVehicleData(data);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x22,
                { 0xF1, 0x01 }
            }
        );

    bool passed =
        response.isPositive() &&
        response.payload.size() == 4;

    std::uint16_t rpm = 0;

    if (passed)
    {
        rpm =
            decodeUnsigned16(
                response.payload,
                2
            );

        passed =
            rpm == 2140;
    }

    return completeUdsTest(
        test,
        passed,
        "2140 RPM",
        std::to_string(rpm) + " RPM",
        "Engine RPM DID readout verified."
    );
}

// ============================================================
// TC_UDS_025
// UDS-REQ-025
// ============================================================

TestCase TestRunner::runUdsSteeringAngleDidTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_025",
            "Steering Angle DID Definition",
            "Verify steering angle DID equals 0xF102.",
            "UDS-REQ-025",
            "UDS definitions loaded.",
            "Inspect SteeringAngleDid.",
            "SteeringAngleDid shall equal 0xF102."
        );

    const bool passed =
        UdsService::SteeringAngleDid ==
        0xF102;

    return completeUdsTest(
        test,
        passed,
        "SteeringAngleDid=0xF102",
        passed
        ? "SteeringAngleDid=0xF102"
        : "Steering angle DID mismatch",
        "Steering angle DID verified."
    );
}

// ============================================================
// TC_UDS_026
// UDS-REQ-026
// ============================================================

TestCase TestRunner::runUdsSteeringAngleReadoutTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_026",
            "Steering Angle DID Readout",
            "Verify F102 returns signed steering angle.",
            "UDS-REQ-026",
            "Steering angle is -12.3 degrees.",
            "Read DID F102.",
            "Returned value shall decode to -12.3 degrees."
        );

    DtcManager manager;
    UdsServer server(manager);

    UdsVehicleData data;
    data.steeringAngleDeg = -12.3f;

    server.setVehicleData(data);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x22,
                { 0xF1, 0x02 }
            }
        );

    bool passed =
        response.isPositive() &&
        response.payload.size() == 4;

    float angle = 0.0f;

    if (passed)
    {
        angle =
            static_cast<float>(
                decodeSigned16(
                    response.payload,
                    2
                )
                ) / 10.0f;

        passed =
            std::fabs(
                angle - (-12.3f)
            ) < 0.01f;
    }

    std::ostringstream actual;
    actual << angle << " deg";

    return completeUdsTest(
        test,
        passed,
        "-12.3 deg",
        actual.str(),
        "Steering angle DID readout verified."
    );
}

// ============================================================
// TC_UDS_027
// UDS-REQ-027
// ============================================================

TestCase TestRunner::runUdsUnsupportedDidTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_027",
            "Unsupported DID",
            "Verify unknown DID produces RequestOutOfRange.",
            "UDS-REQ-027",
            "UDS server initialized.",
            "Read DID F1FF.",
            "Server shall return NRC 0x31."
        );

    DtcManager manager;
    UdsServer server(manager);

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x22,
                { 0xF1, 0xFF }
            }
        );

    const bool passed =
        response.isNegative() &&
        response.payload.size() == 2 &&
        response.payload[1] ==
        UdsService::NegativeResponseCode::
        RequestOutOfRange;

    return completeUdsTest(
        test,
        passed,
        "NRC=0x31",
        passed
        ? "NRC=0x31"
        : "Unsupported DID handling mismatch",
        "Unsupported DID behavior verified."
    );
}

// ============================================================
// TC_UDS_028
// UDS-REQ-028
// ============================================================

TestCase TestRunner::runUdsDtcManagerAccessTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_028",
            "UDS DTC Manager Access",
            "Verify UDS reads DTC state from the existing DTC manager.",
            "UDS-REQ-028",
            "A shared DTC manager is connected to UdsServer.",
            "Activate C0031 after UdsServer construction and read DTCs.",
            "UDS shall observe C0031."
        );

    DtcManager manager;
    UdsServer server(manager);

    manager.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        10.0
    );

    const UdsResponse response =
        server.processRequest(
            UdsRequest{
                0x19,
                { UdsService::ReportActiveDtcs }
            }
        );

    const bool passed =
        containsDtc(
            response,
            DtcDefinitions::FrontLeftWheelSpeedSensor
        );

    return completeUdsTest(
        test,
        passed,
        "UDS observes shared DTC C0031",
        passed
        ? "UDS observed C0031"
        : "UDS did not observe shared DTC",
        "Shared DTC manager access verified."
    );
}

// ============================================================
// TC_UDS_029
// UDS-REQ-029
// ============================================================

TestCase TestRunner::runUdsDtcClearIntegrationTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_029",
            "UDS DTC Clear Integration",
            "Verify UDS clear modifies the existing DTC manager.",
            "UDS-REQ-029",
            "C0031 active in shared DTC manager.",
            "Send Clear Diagnostic Information.",
            "Existing DTC manager shall show C0031 inactive."
        );

    DtcManager manager;

    manager.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        10.0
    );

    UdsServer server(manager);

    server.processRequest(
        UdsRequest{
            0x14,
            { 0xFF, 0xFF, 0xFF }
        }
    );

    const Dtc* dtc =
        manager.findDtc(
            DtcDefinitions::FrontLeftWheelSpeedSensor
        );

    const bool passed =
        dtc != nullptr &&
        !dtc->isActive() &&
        dtc->status ==
        DtcStatus::Cleared;

    return completeUdsTest(
        test,
        passed,
        "Shared DTC record status=Cleared",
        passed
        ? "Shared DTC record status=Cleared"
        : "Shared DTC record was not cleared",
        "UDS DTC clear integration verified."
    );
}

// ============================================================
// TC_UDS_030
// UDS-REQ-030
// ============================================================

TestCase TestRunner::runUdsRuntimeDiagnosticProcessingTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_030",
            "Runtime Diagnostic Processing",
            "Verify UDS requests are processed during simulation runtime.",
            "UDS-REQ-030",
            "Simulation is running.",
            "Advance simulation then issue UDS request without reset.",
            "UDS request shall be processed successfully."
        );

    SimulationEngine engine;

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    engine.update(
        200.0
    );

    const double before =
        engine.getCurrentTimeMs();

    const UdsResponse response =
        engine
        .getUdsServer()
        .processRequest(
            UdsRequest{
                0x10,
                { 0x03 }
            }
        );

    const double after =
        engine.getCurrentTimeMs();

    const bool passed =
        before > 0.0 &&
        after == before &&
        response.isPositive() &&
        engine
        .getUdsServer()
        .getCurrentSession() ==
        UdsService::ExtendedSession;

    return completeUdsTest(
        test,
        passed,
        "UDS processed during active simulation",
        passed
        ? "Runtime request processed successfully"
        : "Runtime diagnostic processing failed",
        "Runtime diagnostic processing verified."
    );
}

// ============================================================
// TC_UDS_031
// UDS-REQ-031
// ============================================================

TestCase TestRunner::runUdsCurrentVehicleDataTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_031",
            "Current Vehicle Data",
            "Verify DID response reflects current simulation vehicle speed.",
            "UDS-REQ-031",
            "Acceleration scenario is running.",
            "Advance simulation and read DID F100.",
            "UDS speed shall match current VehicleModel speed to 0.1 km/h."
        );

    SimulationEngine engine;

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    engine.update(
        500.0
    );

    const UdsResponse response =
        engine
        .getUdsServer()
        .processRequest(
            UdsRequest{
                0x22,
                { 0xF1, 0x00 }
            }
        );

    bool passed =
        response.isPositive() &&
        response.payload.size() == 4;

    float udsSpeed = 0.0f;

    if (passed)
    {
        udsSpeed =
            static_cast<float>(
                decodeUnsigned16(
                    response.payload,
                    2
                )
                ) / 10.0f;

        const float modelSpeed =
            engine
            .getVehicleModel()
            .getVehicleSpeedKmh();

        passed =
            std::fabs(
                udsSpeed - modelSpeed
            ) <= 0.051f;
    }

    std::ostringstream actual;
    actual << udsSpeed << " km/h";

    return completeUdsTest(
        test,
        passed,
        "UDS speed matches current VehicleModel speed",
        actual.str(),
        "Current runtime vehicle data verified."
    );
}

// ============================================================
// TC_UDS_032
// UDS-REQ-032
// ============================================================

TestCase TestRunner::runUdsResetStateTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_032",
            "UDS Reset State",
            "Verify simulation reset restores Default Diagnostic Session.",
            "UDS-REQ-032",
            "UDS server is switched to Extended Session.",
            "Reset SimulationEngine.",
            "UDS session shall return to 0x01."
        );

    SimulationEngine engine;

    engine
        .getUdsServer()
        .processRequest(
            UdsRequest{
                0x10,
                { 0x03 }
            }
        );

    const bool extendedBeforeReset =
        engine
        .getUdsServer()
        .getCurrentSession() ==
        UdsService::ExtendedSession;

    engine.reset();

    const bool defaultAfterReset =
        engine
        .getUdsServer()
        .getCurrentSession() ==
        UdsService::DefaultSession;

    const bool passed =
        extendedBeforeReset &&
        defaultAfterReset;

    return completeUdsTest(
        test,
        passed,
        "Extended before reset; Default after reset",
        passed
        ? "Session restored to 0x01"
        : "UDS reset state mismatch",
        "UDS reset behavior verified."
    );
}

// ============================================================
// TC_UDS_033
// UDS-REQ-033
// ============================================================

TestCase TestRunner::runUdsDeterminismTest()
{
    TestCase test =
        createUdsTest(
            "TC_UDS_033",
            "UDS Deterministic Response",
            "Verify identical initial state and request produce identical response.",
            "UDS-REQ-033",
            "Two identical UDS environments.",
            "Apply identical DTC state, vehicle data and request.",
            "Both responses shall be identical."
        );

    DtcManager managerA;
    DtcManager managerB;

    managerA.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        100.0
    );

    managerB.reportFault(
        DtcDefinitions::FrontLeftWheelSpeedSensor,
        100.0
    );

    UdsServer serverA(managerA);
    UdsServer serverB(managerB);

    UdsVehicleData data;

    data.vehicleSpeedKmh = 42.5f;
    data.engineRpm = 2500;
    data.steeringAngleDeg = 3.2f;

    serverA.setVehicleData(data);
    serverB.setVehicleData(data);

    const UdsRequest request{
        0x19,
        { UdsService::ReportActiveDtcs }
    };

    const UdsResponse responseA =
        serverA.processRequest(
            request
        );

    const UdsResponse responseB =
        serverB.processRequest(
            request
        );

    const bool passed =
        responseEquals(
            responseA,
            responseB
        );

    return completeUdsTest(
        test,
        passed,
        "Identical UDS responses",
        passed
        ? "Responses are identical"
        : "Responses differ",
        "UDS deterministic behavior verified."
    );
}