#include "TestRunner.h"

#include <chrono>
#include <cstdint>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

#include "../transport/IsoTpFrame.h"
#include "../transport/IsoTpProtocol.h"

namespace
{
    using Clock =
        std::chrono::steady_clock;

    TestCase createIsoTpTestCase(
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

    bool frameVectorsEqual(
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

    std::vector<std::uint8_t>
        createSequentialPayload(
            std::size_t size)
    {
        std::vector<std::uint8_t> payload;

        payload.reserve(
            size
        );

        for (
            std::size_t index = 0;
            index < size;
            ++index
            )
        {
            payload.push_back(
                static_cast<std::uint8_t>(
                    index & 0xFF
                    )
            );
        }

        return payload;
    }
}

// ============================================================
// ISOTP-REQ-001
// ============================================================

TestCase TestRunner::runIsoTpTransportComponentTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_001",
            "ISO-TP Transport Component",
            "ISOTP-REQ-001",
            "Verify ISO-TP segmentation and reassembly capability.",
            "A valid diagnostic payload is available.",
            "Segment and reassemble a multi-frame payload.",
            "The reconstructed payload shall equal the original payload."
        );

    const auto start =
        Clock::now();

    const std::vector<std::uint8_t> payload{
        0x22,
        0xF1,
        0x90,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07
    };

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const auto result =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        result.success &&
        result.payload ==
        payload;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Successful segmentation and exact payload reassembly",
        passed
        ? "Payload reconstructed successfully"
        : result.error,
        passed
        ? "ISO-TP transport component is operational."
        : "ISO-TP transport round-trip failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-002
// ============================================================

TestCase TestRunner::runIsoTpClassicCanFrameSizeTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_002",
            "ISO-TP Classic CAN Frame Size",
            "ISOTP-REQ-002",
            "Verify ISO-TP uses Classic CAN frame payload size.",
            "A payload requiring segmentation is available.",
            "Segment the payload and inspect every CAN frame.",
            "Every generated CAN frame shall have DLC 8."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            40
        );

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    bool passed =
        !frames.empty();

    for (const CanFrame& frame : frames)
    {
        if (frame.dlc != 8)
        {
            passed = false;
            break;
        }
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "All ISO-TP CAN frames use DLC 8",
        passed
        ? "All generated frames use DLC 8"
        : "At least one generated frame did not use DLC 8",
        passed
        ? "Classic CAN frame size verified."
        : "Classic CAN frame size verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-003
// ============================================================

TestCase TestRunner::runIsoTpStandardCanIdentifierTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_003",
            "ISO-TP Standard CAN Identifier",
            "ISOTP-REQ-003",
            "Verify support for 11-bit CAN identifiers and rejection beyond 0x7FF.",
            "ISO-TP transport component is available.",
            "Segment using identifiers 0x000, 0x7FF, and 0x800.",
            "0x000 and 0x7FF shall be accepted; 0x800 shall be rejected."
        );

    const auto start =
        Clock::now();

    bool lowerAccepted = false;
    bool upperAccepted = false;
    bool invalidRejected = false;

    const std::vector<std::uint8_t> payload{
        0x10,
        0x01
    };

    try
    {
        const auto lower =
            IsoTpProtocol::segment(
                0x000,
                payload
            );

        lowerAccepted =
            !lower.empty() &&
            lower.front().arbitrationId ==
            0x000;
    }
    catch (...)
    {
        lowerAccepted = false;
    }

    try
    {
        const auto upper =
            IsoTpProtocol::segment(
                0x7FF,
                payload
            );

        upperAccepted =
            !upper.empty() &&
            upper.front().arbitrationId ==
            0x7FF;
    }
    catch (...)
    {
        upperAccepted = false;
    }

    try
    {
        IsoTpProtocol::segment(
            0x800,
            payload
        );
    }
    catch (const std::invalid_argument&)
    {
        invalidRejected =
            true;
    }
    catch (...)
    {
        invalidRejected =
            false;
    }

    const bool passed =
        lowerAccepted &&
        upperAccepted &&
        invalidRejected;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "0x000 and 0x7FF accepted; 0x800 rejected",
        passed
        ? "11-bit CAN identifier boundaries behaved correctly"
        : "Identifier boundary behavior was incorrect",
        passed
        ? "Standard CAN identifier support verified."
        : "Standard CAN identifier verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-004
// ============================================================

TestCase TestRunner::runIsoTpMaximumPayloadTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_004",
            "ISO-TP Maximum Payload",
            "ISOTP-REQ-004",
            "Verify the initial ISO-TP implementation supports 4095-byte payloads.",
            "A 4095-byte diagnostic payload is available.",
            "Segment and reassemble the maximum payload.",
            "The full 4095-byte payload shall be reconstructed exactly."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            IsoTpProtocol::MaximumPayloadLength
        );

    bool passed = false;
    std::string actual;

    try
    {
        const auto frames =
            IsoTpProtocol::segment(
                0x7E0,
                payload
            );

        const auto result =
            IsoTpProtocol::reassemble(
                frames
            );

        passed =
            result.success &&
            result.payload ==
            payload;

        actual =
            passed
            ? "4095-byte payload reconstructed"
            : result.error;
    }
    catch (const std::exception& exception)
    {
        actual =
            exception.what();
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "4095-byte payload supported",
        actual,
        passed
        ? "Maximum ISO-TP payload verified."
        : "Maximum ISO-TP payload verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-005
// ============================================================

TestCase TestRunner::runIsoTpEmptyPayloadRejectionTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_005",
            "ISO-TP Empty Payload Rejection",
            "ISOTP-REQ-005",
            "Verify empty payloads are rejected.",
            "An empty diagnostic payload is available.",
            "Attempt ISO-TP segmentation.",
            "The operation shall throw std::invalid_argument."
        );

    const auto start =
        Clock::now();

    bool rejected =
        false;

    try
    {
        IsoTpProtocol::segment(
            0x7E0,
            {}
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
        "Empty payload rejected",
        rejected
        ? "Empty payload rejected"
        : "Empty payload accepted",
        rejected
        ? "Empty payload validation verified."
        : "Empty payload validation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-006
// ============================================================

TestCase TestRunner::runIsoTpSingleFrameSupportTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_006",
            "ISO-TP Single Frame Support",
            "ISOTP-REQ-006",
            "Verify ISO-TP Single Frame generation.",
            "A short diagnostic payload is available.",
            "Segment a 3-byte payload.",
            "Exactly one Single Frame shall be generated."
        );

    const auto start =
        Clock::now();

    const std::vector<std::uint8_t> payload{
        0x22,
        0xF1,
        0x00
    };

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    bool passed =
        frames.size() == 1;

    if (passed)
    {
        const auto info =
            IsoTpProtocol::parseFrame(
                frames.front()
            );

        passed =
            info.valid &&
            info.type ==
            IsoTpFrameType::SingleFrame;
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "One ISO-TP Single Frame",
        passed
        ? "One Single Frame generated"
        : "Single Frame generation failed",
        passed
        ? "Single Frame support verified."
        : "Single Frame support verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-007
// ============================================================

TestCase TestRunner::runIsoTpSingleFramePayloadCapacityTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_007",
            "ISO-TP Single Frame Payload Capacity",
            "ISOTP-REQ-007",
            "Verify payload lengths 1 through 7 use one Single Frame.",
            "Payloads of lengths 1 through 7 are available.",
            "Segment each payload.",
            "Each payload shall produce exactly one Single Frame."
        );

    const auto start =
        Clock::now();

    bool passed =
        true;

    for (
        std::size_t size = 1;
        size <= 7;
        ++size
        )
    {
        const auto payload =
            createSequentialPayload(
                size
            );

        const auto frames =
            IsoTpProtocol::segment(
                0x7E0,
                payload
            );

        if (frames.size() != 1)
        {
            passed = false;
            break;
        }

        const auto info =
            IsoTpProtocol::parseFrame(
                frames.front()
            );

        if (
            !info.valid ||
            info.type !=
            IsoTpFrameType::SingleFrame ||
            info.payloadLength !=
            size
            )
        {
            passed = false;
            break;
        }
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Payload sizes 1-7 each use one Single Frame",
        passed
        ? "All sizes 1-7 used one Single Frame"
        : "At least one payload size did not use one Single Frame",
        passed
        ? "Single Frame payload capacity verified."
        : "Single Frame payload capacity verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-008
// ============================================================

TestCase TestRunner::runIsoTpSingleFramePciTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_008",
            "ISO-TP Single Frame PCI",
            "ISOTP-REQ-008",
            "Verify Single Frame PCI encoding.",
            "A 5-byte payload is available.",
            "Segment the payload and inspect PCI byte.",
            "Upper nibble shall be 0 and lower nibble shall equal 5."
        );

    const auto start =
        Clock::now();

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(5)
        );

    const std::uint8_t pci =
        frames.front().data[0];

    const bool passed =
        (pci & 0xF0) == 0x00 &&
        (pci & 0x0F) == 0x05;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "PCI = 0x05",
        passed
        ? "PCI upper nibble 0x0, length 5"
        : "Single Frame PCI encoding incorrect",
        passed
        ? "Single Frame PCI verified."
        : "Single Frame PCI verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-009
// ============================================================

TestCase TestRunner::runIsoTpFirstFrameSupportTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_009",
            "ISO-TP First Frame Support",
            "ISOTP-REQ-009",
            "Verify payloads larger than 7 bytes begin with a First Frame.",
            "An 8-byte payload is available.",
            "Segment the payload.",
            "The first generated frame shall be a First Frame."
        );

    const auto start =
        Clock::now();

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(8)
        );

    const auto info =
        IsoTpProtocol::parseFrame(
            frames.front()
        );

    const bool passed =
        frames.size() > 1 &&
        info.valid &&
        info.type ==
        IsoTpFrameType::FirstFrame;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "First frame type = FirstFrame",
        passed
        ? "8-byte payload began with First Frame"
        : "8-byte payload did not begin with First Frame",
        passed
        ? "First Frame support verified."
        : "First Frame support verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-010
// ============================================================

TestCase TestRunner::runIsoTpFirstFramePayloadLengthTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_010",
            "ISO-TP First Frame Payload Length",
            "ISOTP-REQ-010",
            "Verify the 12-bit First Frame payload length field.",
            "A 300-byte payload is available.",
            "Segment and parse the First Frame.",
            "The declared ISO-TP payload length shall equal 300."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            300
        );

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const auto info =
        IsoTpProtocol::parseFrame(
            frames.front()
        );

    const bool passed =
        info.valid &&
        info.type ==
        IsoTpFrameType::FirstFrame &&
        info.payloadLength ==
        300;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "First Frame payload length = 300",
        passed
        ? "Decoded payload length = 300"
        : "Decoded First Frame length was incorrect",
        passed
        ? "First Frame payload length verified."
        : "First Frame payload length verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-011
// ============================================================

TestCase TestRunner::runIsoTpFirstFrameDataCapacityTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_011",
            "ISO-TP First Frame Data Capacity",
            "ISOTP-REQ-011",
            "Verify First Frame transports the first 6 diagnostic payload bytes.",
            "A payload larger than 7 bytes is available.",
            "Segment the payload and inspect First Frame data bytes 2-7.",
            "The first 6 payload bytes shall be present in the First Frame."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            20
        );

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    bool passed =
        !frames.empty();

    for (
        std::size_t index = 0;
        index < 6 && passed;
        ++index
        )
    {
        if (
            frames.front().data[index + 2] !=
            payload[index]
            )
        {
            passed = false;
        }
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "First Frame contains payload bytes 0-5",
        passed
        ? "First six payload bytes matched"
        : "First Frame payload bytes did not match",
        passed
        ? "First Frame data capacity verified."
        : "First Frame data capacity verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-012
// ============================================================

TestCase TestRunner::runIsoTpConsecutiveFrameSupportTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_012",
            "ISO-TP Consecutive Frame Support",
            "ISOTP-REQ-012",
            "Verify Consecutive Frames are produced after the First Frame.",
            "A 20-byte payload is available.",
            "Segment the payload.",
            "Frames after the First Frame shall be Consecutive Frames."
        );

    const auto start =
        Clock::now();

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(20)
        );

    bool passed =
        frames.size() > 1;

    for (
        std::size_t index = 1;
        index < frames.size() && passed;
        ++index
        )
    {
        const auto info =
            IsoTpProtocol::parseFrame(
                frames[index]
            );

        if (
            !info.valid ||
            info.type !=
            IsoTpFrameType::ConsecutiveFrame
            )
        {
            passed = false;
        }
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "All frames after FF are Consecutive Frames",
        passed
        ? "Consecutive Frames generated correctly"
        : "Invalid frame type found after First Frame",
        passed
        ? "Consecutive Frame support verified."
        : "Consecutive Frame support verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-013
// ============================================================

TestCase TestRunner::runIsoTpConsecutiveFrameDataCapacityTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_013",
            "ISO-TP Consecutive Frame Data Capacity",
            "ISOTP-REQ-013",
            "Verify each Consecutive Frame carries up to 7 payload bytes.",
            "A 20-byte payload is available.",
            "Segment the payload and inspect Consecutive Frame payload bytes.",
            "The first Consecutive Frame shall contain the next 7 payload bytes."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            20
        );

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    bool passed =
        frames.size() >= 2;

    if (passed)
    {
        for (
            std::size_t index = 0;
            index < 7;
            ++index
            )
        {
            if (
                frames[1].data[index + 1] !=
                payload[index + 6]
                )
            {
                passed = false;
                break;
            }
        }
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "First CF carries payload bytes 6-12",
        passed
        ? "Seven consecutive payload bytes matched"
        : "Consecutive Frame payload capacity incorrect",
        passed
        ? "Consecutive Frame data capacity verified."
        : "Consecutive Frame data capacity verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-014
// ============================================================

TestCase TestRunner::runIsoTpInitialSequenceNumberTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_014",
            "ISO-TP Initial Sequence Number",
            "ISOTP-REQ-014",
            "Verify the first Consecutive Frame uses sequence number 1.",
            "A segmented payload is available.",
            "Inspect the first Consecutive Frame.",
            "Sequence number shall equal 0x1."
        );

    const auto start =
        Clock::now();

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(20)
        );

    const auto info =
        IsoTpProtocol::parseFrame(
            frames[1]
        );

    const bool passed =
        info.valid &&
        info.type ==
        IsoTpFrameType::ConsecutiveFrame &&
        info.sequenceNumber ==
        0x01;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Sequence number = 0x1",
        passed
        ? "First Consecutive Frame SN = 0x1"
        : "Initial sequence number incorrect",
        passed
        ? "Initial sequence number verified."
        : "Initial sequence number verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-015
// ============================================================

TestCase TestRunner::runIsoTpSequenceNumberIncrementTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_015",
            "ISO-TP Sequence Number Increment",
            "ISOTP-REQ-015",
            "Verify Consecutive Frame sequence numbers increment.",
            "A payload requiring multiple Consecutive Frames is available.",
            "Segment and inspect Consecutive Frame sequence numbers.",
            "Sequence shall increment 1, 2, 3..."
        );

    const auto start =
        Clock::now();

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(30)
        );

    bool passed =
        frames.size() >= 4;

    std::uint8_t expected =
        0x01;

    for (
        std::size_t index = 1;
        index < frames.size() && passed;
        ++index
        )
    {
        const auto info =
            IsoTpProtocol::parseFrame(
                frames[index]
            );

        if (
            !info.valid ||
            info.sequenceNumber !=
            expected
            )
        {
            passed = false;
            break;
        }

        expected =
            static_cast<std::uint8_t>(
                (expected + 1) &
                0x0F
                );
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Consecutive Frame SN increments sequentially",
        passed
        ? "Sequence numbers incremented correctly"
        : "Sequence-number increment was incorrect",
        passed
        ? "Sequence-number increment verified."
        : "Sequence-number increment verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-016
// ============================================================

TestCase TestRunner::runIsoTpSequenceNumberRolloverTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_016",
            "ISO-TP Sequence Number Rollover",
            "ISOTP-REQ-016",
            "Verify sequence number rolls from 0xF to 0x0.",
            "A payload requiring at least 16 Consecutive Frames is available.",
            "Segment and inspect Consecutive Frame sequence numbers.",
            "The frame after SN 0xF shall use SN 0x0."
        );

    const auto start =
        Clock::now();

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(120)
        );

    bool foundF =
        false;

    bool rolloverCorrect =
        false;

    for (
        std::size_t index = 1;
        index + 1 < frames.size();
        ++index
        )
    {
        const auto current =
            IsoTpProtocol::parseFrame(
                frames[index]
            );

        const auto next =
            IsoTpProtocol::parseFrame(
                frames[index + 1]
            );

        if (
            current.sequenceNumber ==
            0x0F
            )
        {
            foundF =
                true;

            rolloverCorrect =
                next.sequenceNumber ==
                0x00;

            break;
        }
    }

    const bool passed =
        foundF &&
        rolloverCorrect;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Sequence 0xF followed by 0x0",
        passed
        ? "Sequence rollover 0xF -> 0x0 observed"
        : "Expected sequence rollover was not observed",
        passed
        ? "Sequence-number rollover verified."
        : "Sequence-number rollover verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-017
// ============================================================

TestCase TestRunner::runIsoTpFlowControlFrameSupportTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_017",
            "ISO-TP Flow Control Frame Support",
            "ISOTP-REQ-017",
            "Verify Flow Control frame generation and parsing.",
            "ISO-TP transport component is available.",
            "Create and parse a Flow Control frame.",
            "Parsed frame type shall be FlowControl."
        );

    const auto start =
        Clock::now();

    const CanFrame frame =
        IsoTpProtocol::createFlowControlFrame(
            0x7E8
        );

    const auto info =
        IsoTpProtocol::parseFrame(
            frame
        );

    const bool passed =
        info.valid &&
        info.type ==
        IsoTpFrameType::FlowControl;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "ISO-TP Flow Control frame",
        passed
        ? "Flow Control frame created and parsed"
        : "Flow Control frame handling failed",
        passed
        ? "Flow Control support verified."
        : "Flow Control support verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-018
// ============================================================

TestCase TestRunner::runIsoTpFlowStatusTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_018",
            "ISO-TP Flow Status",
            "ISOTP-REQ-018",
            "Verify ContinueToSend, Wait, and Overflow Flow Status values.",
            "ISO-TP Flow Control support is available.",
            "Create and parse each supported Flow Status.",
            "All three Flow Status values shall be represented correctly."
        );

    const auto start =
        Clock::now();

    const std::vector<IsoTpFlowStatus> statuses{
        IsoTpFlowStatus::ContinueToSend,
        IsoTpFlowStatus::Wait,
        IsoTpFlowStatus::Overflow
    };

    bool passed =
        true;

    for (
        const IsoTpFlowStatus status :
    statuses
        )
    {
        const CanFrame frame =
            IsoTpProtocol::createFlowControlFrame(
                0x7E8,
                status
            );

        const auto info =
            IsoTpProtocol::parseFrame(
                frame
            );

        if (
            !info.valid ||
            info.flowStatus !=
            status
            )
        {
            passed = false;
            break;
        }
    }

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "CTS, Wait, and Overflow supported",
        passed
        ? "All supported flow statuses parsed correctly"
        : "At least one Flow Status was incorrect",
        passed
        ? "Flow Status support verified."
        : "Flow Status support verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-019
// ============================================================

TestCase TestRunner::runIsoTpFlowControlParametersTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_019",
            "ISO-TP Flow Control Parameters",
            "ISOTP-REQ-019",
            "Verify Flow Status, Block Size, and STmin representation.",
            "ISO-TP Flow Control support is available.",
            "Create Flow Control frame with BS=8 and STmin=10.",
            "Parsed values shall match the configured values."
        );

    const auto start =
        Clock::now();

    const CanFrame frame =
        IsoTpProtocol::createFlowControlFrame(
            0x7E8,
            IsoTpFlowStatus::ContinueToSend,
            8,
            10
        );

    const auto info =
        IsoTpProtocol::parseFrame(
            frame
        );

    const bool passed =
        info.valid &&
        info.flowStatus ==
        IsoTpFlowStatus::ContinueToSend &&
        info.blockSize ==
        8 &&
        info.separationTimeMinimum ==
        10;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "FS=CTS, BS=8, STmin=10",
        passed
        ? "Flow Control parameters matched"
        : "Flow Control parameters did not match",
        passed
        ? "Flow Control parameters verified."
        : "Flow Control parameter verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-020
// ============================================================

TestCase TestRunner::runIsoTpSingleFrameReassemblyTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_020",
            "ISO-TP Single Frame Reassembly",
            "ISOTP-REQ-020",
            "Verify payload reconstruction from a Single Frame.",
            "A valid short payload is available.",
            "Segment and reassemble the payload.",
            "Reassembled payload shall equal the original."
        );

    const auto start =
        Clock::now();

    const std::vector<std::uint8_t> payload{
        0x10,
        0x03
    };

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const auto result =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        result.success &&
        result.payload ==
        payload;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Exact Single Frame payload reconstruction",
        passed
        ? "Single Frame payload reconstructed"
        : result.error,
        passed
        ? "Single Frame reassembly verified."
        : "Single Frame reassembly verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-021
// ============================================================

TestCase TestRunner::runIsoTpMultiFrameReassemblyTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_021",
            "ISO-TP Multi-Frame Reassembly",
            "ISOTP-REQ-021",
            "Verify complete multi-frame payload reconstruction.",
            "A 100-byte payload is available.",
            "Segment and reassemble the payload.",
            "Reassembled payload shall equal all 100 original bytes."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            100
        );

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const auto result =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        result.success &&
        result.payload ==
        payload;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Exact 100-byte payload reconstruction",
        passed
        ? "Multi-frame payload reconstructed"
        : result.error,
        passed
        ? "Multi-frame reassembly verified."
        : "Multi-frame reassembly verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-022
// ============================================================

TestCase TestRunner::runIsoTpSequenceValidationTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_022",
            "ISO-TP Sequence Validation",
            "ISOTP-REQ-022",
            "Verify invalid Consecutive Frame sequence numbers are rejected.",
            "A valid multi-frame sequence is available.",
            "Corrupt the first Consecutive Frame sequence number.",
            "Reassembly shall fail."
        );

    const auto start =
        Clock::now();

    auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(30)
        );

    frames[1].data[0] =
        0x22;

    const auto result =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        !result.success;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Reassembly rejected for incorrect sequence number",
        passed
        ? result.error
        : "Invalid sequence was accepted",
        passed
        ? "Sequence validation verified."
        : "Sequence validation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-023
// ============================================================

TestCase TestRunner::runIsoTpCanIdentifierValidationTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_023",
            "ISO-TP CAN Identifier Validation",
            "ISOTP-REQ-023",
            "Verify all frames in one ISO-TP message require the same CAN ID.",
            "A valid multi-frame sequence is available.",
            "Change one Consecutive Frame arbitration ID.",
            "Reassembly shall fail."
        );

    const auto start =
        Clock::now();

    auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(30)
        );

    frames[1].arbitrationId =
        0x7E1;

    const auto result =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        !result.success;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Mismatched arbitration ID rejected",
        passed
        ? result.error
        : "Mismatched arbitration ID accepted",
        passed
        ? "CAN identifier validation verified."
        : "CAN identifier validation failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-024
// ============================================================

TestCase TestRunner::runIsoTpIncompleteMessageDetectionTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_024",
            "ISO-TP Incomplete Message Detection",
            "ISOTP-REQ-024",
            "Verify incomplete multi-frame messages are rejected.",
            "A valid multi-frame sequence is available.",
            "Remove the final Consecutive Frame before reassembly.",
            "Reassembly shall fail due to incomplete payload."
        );

    const auto start =
        Clock::now();

    auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            createSequentialPayload(50)
        );

    frames.pop_back();

    const auto result =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        !result.success;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Incomplete message rejected",
        passed
        ? result.error
        : "Incomplete message accepted",
        passed
        ? "Incomplete message detection verified."
        : "Incomplete message detection failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-025
// ============================================================

TestCase TestRunner::runIsoTpDeterministicSegmentationTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_025",
            "ISO-TP Deterministic Segmentation",
            "ISOTP-REQ-025",
            "Verify identical inputs generate identical segmented CAN frames.",
            "Identical CAN ID and payload are available.",
            "Perform segmentation twice.",
            "Both segmented frame sequences shall be identical."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            100
        );

    const auto first =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const auto second =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const bool passed =
        frameVectorsEqual(
            first,
            second
        );

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Identical segmented CAN frame sequences",
        passed
        ? "Both segmentation results were identical"
        : "Segmentation results differed",
        passed
        ? "Deterministic segmentation verified."
        : "Deterministic segmentation verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}

// ============================================================
// ISOTP-REQ-026
// ============================================================

TestCase TestRunner::runIsoTpDeterministicReassemblyTest()
{
    TestCase test =
        createIsoTpTestCase(
            "TC_ISOTP_026",
            "ISO-TP Deterministic Reassembly",
            "ISOTP-REQ-026",
            "Verify identical frame sequences produce identical reassembly results.",
            "One valid ISO-TP frame sequence is available.",
            "Reassemble the same frame sequence twice.",
            "Both reconstructed payloads shall be identical."
        );

    const auto start =
        Clock::now();

    const auto payload =
        createSequentialPayload(
            100
        );

    const auto frames =
        IsoTpProtocol::segment(
            0x7E0,
            payload
        );

    const auto first =
        IsoTpProtocol::reassemble(
            frames
        );

    const auto second =
        IsoTpProtocol::reassemble(
            frames
        );

    const bool passed =
        first.success &&
        second.success &&
        first.payload ==
        second.payload &&
        first.payload ==
        payload;

    const auto end =
        Clock::now();

    setResult(
        test,
        passed,
        "Identical successful reassembly results",
        passed
        ? "Both reassembly results were identical"
        : "Reassembly results differed",
        passed
        ? "Deterministic reassembly verified."
        : "Deterministic reassembly verification failed.",
        elapsedMilliseconds(start, end)
    );

    return test;
}