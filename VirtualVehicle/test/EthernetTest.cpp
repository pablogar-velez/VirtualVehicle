#include "TestRunner.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <exception>
#include <string>
#include <utility>
#include <vector>

#include "../ethernet/EthernetFrame.h"
#include "../ethernet/EthernetStatistics.h"
#include "../ethernet/EthernetTiming.h"
#include "../ethernet/EthernetTraceEntry.h"
#include "../ethernet/MacAddress.h"
#include "../ethernet/VirtualEthernetBus.h"
#include "../ethernet/EthernetNode.h"
#include "../simulation/SimulationEngine.h"

namespace
{
    using Clock = std::chrono::steady_clock;

    const MacAddress NodeA{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 };
    const MacAddress NodeB{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x02 };
    const MacAddress NodeC{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x03 };

    constexpr std::uint16_t TestEtherType = 0x88B5;

    TestCase createEthernetTestCase(
        const std::string& id,
        const std::string& name,
        const std::string& requirementId,
        const std::string& description,
        const std::string& precondition,
        const std::string& stimulus,
        const std::string& expectedResult)
    {
        TestCase test;
        test.id = id;
        test.name = name;
        test.requirementId = requirementId;
        test.description = description;
        test.precondition = precondition;
        test.stimulus = stimulus;
        test.expectedResult = expectedResult;
        return test;
    }

    double elapsedMilliseconds(
        const Clock::time_point& start,
        const Clock::time_point& end)
    {
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    void setResult(
        TestCase& test,
        bool passed,
        const std::string& expected,
        const std::string& actual,
        const std::string& message,
        double executionTimeMs)
    {
        test.result.status = passed ? TestStatus::Passed : TestStatus::Failed;
        test.result.expected = expected;
        test.result.actual = actual;
        test.result.message = message;
        test.result.executionTimeMs = executionTimeMs;
    }

    EthernetFrame createFrame(
        const MacAddress& source,
        const MacAddress& destination,
        const std::vector<std::uint8_t>& payload = { 0x10, 0x20, 0x30, 0x40 })
    {
        EthernetFrame frame;
        frame.sourceMac = source;
        frame.destinationMac = destination;
        frame.etherType = TestEtherType;
        frame.payload = payload;
        return frame;
    }

    bool nearlyEqual(
        double first,
        double second,
        double tolerance = 1e-9)
    {
        return
            std::fabs(first - second) <=
            tolerance;
    }

    template <typename Function>
    TestCase executeEthernetTest(
        TestCase test,
        const std::string& expected,
        Function function)
    {
        const auto start = Clock::now();

        try
        {
            const auto result = function();
            const auto end = Clock::now();

            setResult(
                test,
                result.first,
                expected,
                result.second,
                result.first
                ? "Ethernet requirement verified."
                : "Ethernet requirement verification failed.",
                elapsedMilliseconds(start, end)
            );
        }
        catch (const std::exception& exception)
        {
            const auto end = Clock::now();

            setResult(
                test,
                false,
                expected,
                std::string("Exception: ") + exception.what(),
                "Unexpected exception during Ethernet verification.",
                elapsedMilliseconds(start, end)
            );
        }
        catch (...)
        {
            const auto end = Clock::now();

            setResult(
                test,
                false,
                expected,
                "Unknown exception",
                "Unexpected exception during Ethernet verification.",
                elapsedMilliseconds(start, end)
            );
        }

        return test;
    }
}

TestCase TestRunner::runEthernetFrameRepresentationTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_001",
        "Ethernet Frame Representation",
        "ETH-REQ-001",
        "Verify Ethernet frame representation.",
        "Ethernet frame types are available.",
        "Construct a frame with known values.",
        "Source MAC, destination MAC, EtherType, and payload shall be preserved."
    );

    return executeEthernetTest(test, "All Ethernet frame fields preserved", []()
        {
            const EthernetFrame frame = createFrame(NodeA, NodeB);
            const bool passed =
                frame.sourceMac == NodeA &&
                frame.destinationMac == NodeB &&
                frame.etherType == TestEtherType &&
                frame.payload == std::vector<std::uint8_t>{ 0x10, 0x20, 0x30, 0x40 };

            return std::pair{ passed, passed ? std::string("Frame representation valid") : std::string("Frame representation mismatch") };
        });
}

TestCase TestRunner::runEthernetSourceMacAddressTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_002",
        "Ethernet Source MAC Address",
        "ETH-REQ-002",
        "Verify Ethernet source MAC storage.",
        "NodeA is defined.",
        "Assign NodeA as source MAC.",
        "Stored source MAC shall equal NodeA."
    );

    return executeEthernetTest(test, "sourceMac == NodeA", []()
        {
            const EthernetFrame frame = createFrame(NodeA, NodeB);
            const bool passed = frame.sourceMac == NodeA;
            return std::pair{ passed, frame.sourceMac.toString() };
        });
}

TestCase TestRunner::runEthernetDestinationMacAddressTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_003",
        "Ethernet Destination MAC Address",
        "ETH-REQ-003",
        "Verify Ethernet destination MAC storage.",
        "NodeB is defined.",
        "Assign NodeB as destination MAC.",
        "Stored destination MAC shall equal NodeB."
    );

    return executeEthernetTest(test, "destinationMac == NodeB", []()
        {
            const EthernetFrame frame = createFrame(NodeA, NodeB);
            const bool passed = frame.destinationMac == NodeB;
            return std::pair{ passed, frame.destinationMac.toString() };
        });
}

TestCase TestRunner::runEthernetEtherTypeTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_004",
        "Ethernet EtherType Field",
        "ETH-REQ-004",
        "Verify EtherType storage.",
        "Test EtherType 0x88B5 is defined.",
        "Assign EtherType 0x88B5.",
        "Stored EtherType shall equal 0x88B5."
    );

    return executeEthernetTest(test, "EtherType == 0x88B5", []()
        {
            const EthernetFrame frame = createFrame(NodeA, NodeB);
            const bool passed = frame.etherType == TestEtherType;
            return std::pair{ passed, passed ? std::string("EtherType preserved") : std::string("EtherType mismatch") };
        });
}

TestCase TestRunner::runEthernetPayloadTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_005",
        "Ethernet Payload",
        "ETH-REQ-005",
        "Verify byte-sequence payload storage.",
        "A four-byte payload is defined.",
        "Assign payload to frame.",
        "Complete payload shall be stored."
    );

    return executeEthernetTest(test, "Payload preserved in frame", []()
        {
            const std::vector<std::uint8_t> payload{ 0xAA, 0xBB, 0xCC, 0xDD };
            const EthernetFrame frame = createFrame(NodeA, NodeB, payload);
            const bool passed = frame.payload == payload;
            return std::pair{ passed, std::string("Payload size=") + std::to_string(frame.payload.size()) };
        });
}

TestCase TestRunner::runEthernetPayloadPreservationTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_006",
        "Ethernet Payload Preservation",
        "ETH-REQ-006",
        "Verify payload preservation through VirtualEthernetBus.",
        "NodeA and NodeB are registered.",
        "Transmit and receive a frame.",
        "Received payload shall equal transmitted payload."
    );

    return executeEthernetTest(test, "Received payload equals transmitted payload", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const std::vector<std::uint8_t> payload{ 0x01, 0x23, 0x45, 0x67, 0x89 };
            const EthernetFrame transmitted = createFrame(NodeA, NodeB, payload);
            const bool accepted = bus.transmit(transmitted);

            if (!accepted)
                return std::pair{ false, std::string("Transmission rejected") };

            const EthernetFrame received = bus.receive(NodeB);
            const bool passed = received.payload == payload;
            return std::pair{ passed, passed ? std::string("Payload preserved") : std::string("Payload mismatch") };
        });
}

TestCase TestRunner::runEthernetMacAddressRepresentationTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_007",
        "MAC Address Representation",
        "ETH-REQ-007",
        "Verify deterministic 48-bit MAC representation.",
        "NodeA contains six bytes.",
        "Read bytes and formatted string.",
        "Six bytes and deterministic string shall be preserved."
    );

    return executeEthernetTest(test, "02:00:00:00:00:01", []()
        {
            const bool passed =
                NodeA.getBytes().size() == 6 &&
                NodeA.toString() == "02:00:00:00:00:01";

            return std::pair{ passed, NodeA.toString() };
        });
}

TestCase TestRunner::runEthernetMacAddressEqualityTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_008",
        "MAC Address Equality",
        "ETH-REQ-008",
        "Verify MAC address comparison.",
        "Equal and different MAC addresses exist.",
        "Compare MAC address values.",
        "Equal addresses compare equal and different addresses compare unequal."
    );

    return executeEthernetTest(test, "Equality and inequality valid", []()
        {
            const MacAddress duplicateNodeA{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 };
            const bool passed = NodeA == duplicateNodeA && NodeA != NodeB;
            return std::pair{ passed, passed ? std::string("MAC comparison valid") : std::string("MAC comparison mismatch") };
        });
}

TestCase TestRunner::runEthernetDeterministicNodeAddressingTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_009",
        "Deterministic Ethernet Node Addressing",
        "ETH-REQ-009",
        "Verify deterministic configured node addressing.",
        "Two independent buses exist.",
        "Register identical node addresses on both buses.",
        "Both buses shall preserve the same configured addresses."
    );

    return executeEthernetTest(test, "Same node configuration on both buses", []()
        {
            VirtualEthernetBus firstBus;
            VirtualEthernetBus secondBus;

            firstBus.registerNode(NodeA);
            firstBus.registerNode(NodeB);
            secondBus.registerNode(NodeA);
            secondBus.registerNode(NodeB);

            const bool passed =
                firstBus.getRegisteredNodeCount() == 2 &&
                secondBus.getRegisteredNodeCount() == 2 &&
                firstBus.isNodeRegistered(NodeA) &&
                firstBus.isNodeRegistered(NodeB) &&
                secondBus.isNodeRegistered(NodeA) &&
                secondBus.isNodeRegistered(NodeB);

            return std::pair{ passed, passed ? std::string("Deterministic addressing verified") : std::string("Address configuration mismatch") };
        });
}

TestCase TestRunner::runVirtualEthernetBusComponentTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_010",
        "Virtual Ethernet Bus Component",
        "ETH-REQ-010",
        "Verify VirtualEthernetBus construction and initial state.",
        "No nodes registered.",
        "Construct the bus.",
        "Bus shall begin empty."
    );

    return executeEthernetTest(test, "0 nodes and 0 pending frames", []()
        {
            const VirtualEthernetBus bus;
            const bool passed =
                bus.getRegisteredNodeCount() == 0 &&
                bus.getTotalPendingFrameCount() == 0;

            return std::pair{ passed, std::string("Nodes=") + std::to_string(bus.getRegisteredNodeCount()) + ", Pending=" + std::to_string(bus.getTotalPendingFrameCount()) };
        });
}

TestCase TestRunner::runEthernetFrameTransmissionRequestTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_011",
        "Ethernet Frame Transmission Request",
        "ETH-REQ-011",
        "Verify frame submission to VirtualEthernetBus.",
        "NodeA and NodeB are registered.",
        "Transmit one frame to NodeB.",
        "Transmission shall be accepted and queued for NodeB."
    );

    return executeEthernetTest(test, "Accepted and one pending frame for NodeB", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const bool accepted = bus.transmit(createFrame(NodeA, NodeB));
            const bool passed =
                accepted &&
                bus.hasFrameFor(NodeB) &&
                bus.getPendingFrameCount(NodeB) == 1;

            return std::pair{ passed, std::string("Accepted=") + (accepted ? "true" : "false") + ", Pending=" + std::to_string(bus.getPendingFrameCount(NodeB)) };
        });
}

TestCase TestRunner::runEthernetFrameDeliveryTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_012",
        "Ethernet Frame Delivery",
        "ETH-REQ-012",
        "Verify destination-MAC based delivery.",
        "NodeA, NodeB, and NodeC are registered.",
        "Transmit a frame from NodeA to NodeB.",
        "Only NodeB shall receive the frame."
    );

    return executeEthernetTest(test, "Frame delivered only to NodeB", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);
            bus.registerNode(NodeC);

            const EthernetFrame transmitted = createFrame(NodeA, NodeB);
            const bool accepted = bus.transmit(transmitted);

            if (!accepted)
                return std::pair{ false, std::string("Transmission rejected") };

            const bool routingValid = bus.hasFrameFor(NodeB) && !bus.hasFrameFor(NodeC);
            const EthernetFrame received = bus.receive(NodeB);
            const bool passed = routingValid && received == transmitted;

            return std::pair{ passed, passed ? std::string("Destination routing valid") : std::string("Destination routing mismatch") };
        });
}

TestCase TestRunner::runEthernetUnknownDestinationTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_013",
        "Ethernet Unknown Destination Behavior",
        "ETH-REQ-013",
        "Verify unknown destination behavior.",
        "NodeC is not registered.",
        "Transmit from NodeA to NodeC.",
        "Transmission shall be rejected and unrelated queues remain empty."
    );

    return executeEthernetTest(test, "Unknown destination rejected", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const bool accepted = bus.transmit(createFrame(NodeA, NodeC));
            const bool passed =
                !accepted &&
                !bus.hasFrameFor(NodeB) &&
                bus.getTotalPendingFrameCount() == 0;

            return std::pair{ passed, accepted ? std::string("Unknown destination incorrectly accepted") : std::string("Unknown destination rejected") };
        });
}

TestCase TestRunner::runEthernetMultipleNodesTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_014",
        "Multiple Ethernet Nodes",
        "ETH-REQ-014",
        "Verify multiple registered nodes and independent receive queues.",
        "Three nodes are available.",
        "Transmit one frame to NodeB and one frame to NodeC.",
        "Each node shall receive only its own frame."
    );

    return executeEthernetTest(test, "Independent delivery to NodeB and NodeC", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);
            bus.registerNode(NodeC);

            const EthernetFrame frameToB = createFrame(NodeA, NodeB, { 0xB0, 0x01 });
            const EthernetFrame frameToC = createFrame(NodeA, NodeC, { 0xC0, 0x01 });

            const bool acceptedB = bus.transmit(frameToB);
            const bool acceptedC = bus.transmit(frameToC);

            const bool queueStateValid =
                bus.getRegisteredNodeCount() == 3 &&
                bus.getPendingFrameCount(NodeB) == 1 &&
                bus.getPendingFrameCount(NodeC) == 1 &&
                bus.getTotalPendingFrameCount() == 2;

            if (!acceptedB || !acceptedC || !queueStateValid)
                return std::pair{ false, std::string("Multiple-node queue state invalid") };

            const EthernetFrame receivedB = bus.receive(NodeB);
            const EthernetFrame receivedC = bus.receive(NodeC);

            const bool passed =
                receivedB == frameToB &&
                receivedC == frameToC &&
                bus.getTotalPendingFrameCount() == 0;

            return std::pair{ passed, passed ? std::string("Independent delivery verified") : std::string("Multiple-node delivery mismatch") };
        });
}

TestCase TestRunner::runEthernetLinkRateTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_015",
        "Configurable Ethernet Link Rate",
        "ETH-REQ-015",
        "Verify configured virtual Ethernet link rate.",
        "A VirtualEthernetBus is constructed with the Baseline v1.7 link rate.",
        "Read the configured link rate.",
        "The link rate shall equal 100 Mbit/s."
    );

    return executeEthernetTest(test, "100000000 bit/s", []()
        {
            VirtualEthernetBus bus{ 100000000ULL };
            const bool passed = bus.getLinkRate() == 100000000ULL;

            return std::pair{
                passed,
                std::string("LinkRate=") + std::to_string(bus.getLinkRate()) + " bit/s"
            };
        });
}

TestCase TestRunner::runEthernetTransmissionDurationTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_016",
        "Ethernet Transmission Duration",
        "ETH-REQ-016",
        "Verify deterministic Ethernet transmission duration.",
        "A four-byte payload is transmitted at 100 Mbit/s.",
        "Calculate the simulated transmission duration.",
        "The duration shall equal the simplified 14-byte header plus payload model."
    );

    return executeEthernetTest(test, "18 bytes = 144 bits = 0.00144 ms", []()
        {
            const EthernetFrame frame = createFrame(
                NodeA,
                NodeB,
                { 0x10, 0x20, 0x30, 0x40 }
            );

            const double durationMs =
                EthernetTiming::calculateTransmissionTimeMs(
                    frame,
                    100000000ULL
                );

            const bool passed =
                EthernetTiming::calculateFrameBytes(frame) == 18 &&
                EthernetTiming::calculateFrameBits(frame) == 144ULL &&
                nearlyEqual(durationMs, 0.00144);

            return std::pair{
                passed,
                std::string("DurationMs=") + std::to_string(durationMs)
            };
        });
}

TestCase TestRunner::runEthernetBusTimeProgressionTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_017",
        "Ethernet Bus Time Progression",
        "ETH-REQ-017",
        "Verify deterministic serialized Ethernet bus time progression.",
        "NodeA and NodeB are registered on a 100 Mbit/s bus.",
        "Submit two frames at the same request time.",
        "The second transmission shall begin after the first transmission completes."
    );

    return executeEthernetTest(test, "busyUntil == 2 * frame transmission duration", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const EthernetFrame firstFrame = createFrame(NodeA, NodeB);
            const EthernetFrame secondFrame = createFrame(NodeA, NodeB);

            const double frameDurationMs =
                EthernetTiming::calculateTransmissionTimeMs(
                    firstFrame,
                    bus.getLinkRate()
                );

            const bool firstAccepted = bus.transmit(firstFrame, 0.0);
            const bool secondAccepted = bus.transmit(secondFrame, 0.0);

            const auto& trace = bus.getTrace();

            const bool passed =
                firstAccepted &&
                secondAccepted &&
                trace.size() == 2 &&
                nearlyEqual(trace[0].startTimeMs, 0.0) &&
                nearlyEqual(trace[0].completionTimeMs, frameDurationMs) &&
                nearlyEqual(trace[1].startTimeMs, frameDurationMs) &&
                nearlyEqual(bus.getBusyUntilMs(), frameDurationMs * 2.0);

            return std::pair{
                passed,
                std::string("BusyUntilMs=") + std::to_string(bus.getBusyUntilMs())
            };
        });
}

TestCase TestRunner::runEthernetBusyStateTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_018",
        "Ethernet Busy State",
        "ETH-REQ-018",
        "Verify Ethernet transmission completion timing state.",
        "One valid Ethernet frame is available.",
        "Transmit the frame at request time 5.0 ms.",
        "VirtualEthernetBus shall expose the deterministic completion time."
    );

    return executeEthernetTest(test, "busyUntil > 5.0 ms and equals trace completion", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const bool accepted =
                bus.transmit(
                    createFrame(NodeA, NodeB),
                    5.0
                );

            const auto& trace = bus.getTrace();

            const bool passed =
                accepted &&
                trace.size() == 1 &&
                bus.getBusyUntilMs() > 5.0 &&
                nearlyEqual(
                    bus.getBusyUntilMs(),
                    trace.front().completionTimeMs
                );

            return std::pair{
                passed,
                std::string("BusyUntilMs=") + std::to_string(bus.getBusyUntilMs())
            };
        });
}

TestCase TestRunner::runEthernetTraceRecordingTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_019",
        "Ethernet Trace Recording",
        "ETH-REQ-019",
        "Verify Ethernet transmission trace recording.",
        "NodeA and NodeB are registered.",
        "Transmit one valid Ethernet frame.",
        "Exactly one Ethernet trace entry shall be recorded."
    );

    return executeEthernetTest(test, "Trace entries == 1", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const EthernetFrame frame = createFrame(NodeA, NodeB);
            const bool accepted = bus.transmit(frame, 2.0);

            const bool passed =
                accepted &&
                bus.getTrace().size() == 1 &&
                bus.getTrace().front().frame == frame;

            return std::pair{
                passed,
                std::string("TraceEntries=") + std::to_string(bus.getTrace().size())
            };
        });
}

TestCase TestRunner::runEthernetTraceAddressEvidenceTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_020",
        "Ethernet Trace Address Evidence",
        "ETH-REQ-020",
        "Verify source and destination MAC evidence in Ethernet trace.",
        "A frame from NodeA to NodeB is available.",
        "Transmit the frame and inspect the trace entry.",
        "Trace source MAC shall be NodeA and destination MAC shall be NodeB."
    );

    return executeEthernetTest(test, "Trace MACs == NodeA -> NodeB", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            bus.transmit(createFrame(NodeA, NodeB));

            const auto& entry = bus.getTrace().front();
            const bool passed =
                entry.frame.sourceMac == NodeA &&
                entry.frame.destinationMac == NodeB;

            return std::pair{
                passed,
                entry.frame.sourceMac.toString() + " -> " + entry.frame.destinationMac.toString()
            };
        });
}

TestCase TestRunner::runEthernetTraceEtherTypeEvidenceTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_021",
        "Ethernet Trace EtherType Evidence",
        "ETH-REQ-021",
        "Verify EtherType evidence in Ethernet trace.",
        "A frame using EtherType 0x88B5 is available.",
        "Transmit the frame and inspect the trace.",
        "The trace shall preserve EtherType 0x88B5."
    );

    return executeEthernetTest(test, "Trace EtherType == 0x88B5", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            bus.transmit(createFrame(NodeA, NodeB));

            const bool passed =
                bus.getTrace().front().frame.etherType == TestEtherType;

            return std::pair{
                passed,
                passed ? std::string("EtherType preserved") : std::string("EtherType mismatch")
            };
        });
}

TestCase TestRunner::runEthernetTracePayloadEvidenceTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_022",
        "Ethernet Trace Payload Evidence",
        "ETH-REQ-022",
        "Verify payload evidence in Ethernet trace.",
        "A deterministic payload is available.",
        "Transmit the frame and inspect trace payload evidence.",
        "The complete transmitted payload shall be preserved in the trace."
    );

    return executeEthernetTest(test, "Trace payload equals transmitted payload", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const std::vector<std::uint8_t> payload{
                0xDE, 0xAD, 0xBE, 0xEF, 0x01
            };

            bus.transmit(
                createFrame(
                    NodeA,
                    NodeB,
                    payload
                )
            );

            const bool passed =
                bus.getTrace().front().frame.payload == payload;

            return std::pair{
                passed,
                std::string("PayloadBytes=") + std::to_string(bus.getTrace().front().frame.payload.size())
            };
        });
}

TestCase TestRunner::runEthernetTraceTimingEvidenceTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_023",
        "Ethernet Trace Timing Evidence",
        "ETH-REQ-023",
        "Verify Ethernet request, start, duration, and completion timing evidence.",
        "One valid frame is available.",
        "Transmit the frame at request time 7.5 ms.",
        "Trace timing fields shall be internally consistent."
    );

    return executeEthernetTest(test, "completion == start + duration", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            bus.transmit(
                createFrame(NodeA, NodeB),
                7.5
            );

            const auto& entry = bus.getTrace().front();

            const bool passed =
                nearlyEqual(entry.requestTimeMs, 7.5) &&
                nearlyEqual(entry.startTimeMs, 7.5) &&
                entry.transmissionTimeMs > 0.0 &&
                nearlyEqual(
                    entry.completionTimeMs,
                    entry.startTimeMs + entry.transmissionTimeMs
                ) &&
                nearlyEqual(entry.waitingTimeMs, 0.0);

            return std::pair{
                passed,
                std::string("Start=") + std::to_string(entry.startTimeMs) +
                    ", Completion=" + std::to_string(entry.completionTimeMs)
            };
        });
}

TestCase TestRunner::runEthernetFrameCountTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_024",
        "Ethernet Frame Count",
        "ETH-REQ-024",
        "Verify successfully transmitted Ethernet frame count.",
        "Three registered Ethernet nodes are available.",
        "Transmit three valid frames.",
        "Ethernet statistics shall report three transmitted frames."
    );

    return executeEthernetTest(test, "FrameCount == 3", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);
            bus.registerNode(NodeC);

            bus.transmit(createFrame(NodeA, NodeB));
            bus.transmit(createFrame(NodeA, NodeC));
            bus.transmit(createFrame(NodeB, NodeC));

            const bool passed =
                bus.getStatistics().getFrameCount() == 3;

            return std::pair{
                passed,
                std::string("FrameCount=") + std::to_string(bus.getStatistics().getFrameCount())
            };
        });
}

TestCase TestRunner::runEthernetByteCountTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_025",
        "Ethernet Transmitted Byte Count",
        "ETH-REQ-025",
        "Verify transmitted Ethernet payload-byte accounting.",
        "Two frames with known payload lengths are available.",
        "Transmit payloads of 3 and 5 bytes.",
        "Ethernet statistics shall report eight payload bytes."
    );

    return executeEthernetTest(test, "PayloadByteCount == 8", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            bus.transmit(
                createFrame(
                    NodeA,
                    NodeB,
                    { 0x01, 0x02, 0x03 }
                )
            );

            bus.transmit(
                createFrame(
                    NodeA,
                    NodeB,
                    { 0x10, 0x20, 0x30, 0x40, 0x50 }
                )
            );

            const bool passed =
                bus.getStatistics().getPayloadByteCount() == 8;

            return std::pair{
                passed,
                std::string("PayloadBytes=") + std::to_string(bus.getStatistics().getPayloadByteCount())
            };
        });
}

TestCase TestRunner::runEthernetPerNodeStatisticsTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_026",
        "Per-Node Ethernet Statistics",
        "ETH-REQ-026",
        "Verify Ethernet transmission statistics associated with individual source nodes.",
        "NodeA, NodeB, and NodeC are registered.",
        "Transmit two frames from NodeA and one frame from NodeB.",
        "Per-node statistics shall preserve independent frame and payload counts."
    );

    return executeEthernetTest(test, "NodeA=2 frames, NodeB=1 frame, NodeC=0 frames", []()
        {
            VirtualEthernetBus bus;
            bus.registerNode(NodeA);
            bus.registerNode(NodeB);
            bus.registerNode(NodeC);

            bus.transmit(createFrame(NodeA, NodeB, { 0x01, 0x02 }));
            bus.transmit(createFrame(NodeA, NodeC, { 0x03, 0x04, 0x05 }));
            bus.transmit(createFrame(NodeB, NodeA, { 0x06 }));

            const auto& nodeAStats =
                bus.getStatistics().getNodeStatistics(NodeA);

            const auto& nodeBStats =
                bus.getStatistics().getNodeStatistics(NodeB);

            const auto& nodeCStats =
                bus.getStatistics().getNodeStatistics(NodeC);

            const bool passed =
                nodeAStats.framesTransmitted == 2 &&
                nodeAStats.payloadBytesTransmitted == 5 &&
                nodeBStats.framesTransmitted == 1 &&
                nodeBStats.payloadBytesTransmitted == 1 &&
                nodeCStats.framesTransmitted == 0 &&
                nodeCStats.payloadBytesTransmitted == 0;

            return std::pair{
                passed,
                std::string("NodeAFrames=") + std::to_string(nodeAStats.framesTransmitted) +
                    ", NodeBFrames=" + std::to_string(nodeBStats.framesTransmitted) +
                    ", NodeCFrames=" + std::to_string(nodeCStats.framesTransmitted)
            };
        });
}

TestCase TestRunner::runVirtualEthernetNodeTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_027",
        "Virtual Ethernet Node",
        "ETH-REQ-027",
        "Verify reusable virtual Ethernet node construction and configured MAC address.",
        "NodeA is defined.",
        "Construct an EthernetNode using NodeA.",
        "The node shall preserve its configured MAC address and start with no received frames."
    );

    return executeEthernetTest(test, "Configured MAC preserved; receive history empty", []()
        {
            const EthernetNode node(NodeA);

            const bool passed =
                node.getMacAddress() == NodeA &&
                !node.hasReceivedFrame() &&
                node.getReceivedFrames().empty();

            return std::pair{
                passed,
                std::string("MAC=") + node.getMacAddress().toString() +
                ", received=" + std::to_string(node.getReceivedFrames().size())
            };
        });
}

TestCase TestRunner::runEthernetNodeReceiveFilteringTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_028",
        "Ethernet Node Receive Filtering",
        "ETH-REQ-028",
        "Verify that an Ethernet node accepts only frames addressed to its configured MAC address.",
        "NodeB and NodeC are registered on the virtual Ethernet bus.",
        "Transmit one frame to NodeB and one frame to NodeC, then process NodeB receive traffic.",
        "NodeB shall receive only its addressed frame while the NodeC frame remains queued for NodeC."
    );

    return executeEthernetTest(test, "NodeB receives only NodeB-addressed frame", []()
        {
            VirtualEthernetBus bus;
            EthernetNode nodeB(NodeB);

            bus.registerNode(NodeB);
            bus.registerNode(NodeC);

            bus.transmit(createFrame(NodeA, NodeB), 0.0);
            bus.transmit(createFrame(NodeA, NodeC), 1.0);

            nodeB.processReceivedFrames(bus);

            const auto& received = nodeB.getReceivedFrames();

            const bool passed =
                received.size() == 1 &&
                received.front().destinationMac == NodeB &&
                bus.getPendingFrameCount(NodeB) == 0 &&
                bus.getPendingFrameCount(NodeC) == 1;

            return std::pair{
                passed,
                std::string("NodeB received=") + std::to_string(received.size()) +
                ", NodeC pending=" + std::to_string(bus.getPendingFrameCount(NodeC))
            };
        });
}

TestCase TestRunner::runEthernetNodeTransmissionTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_029",
        "Ethernet Node Transmission",
        "ETH-REQ-029",
        "Verify that a configured Ethernet node constructs and transmits a frame using its own source MAC address.",
        "NodeA and NodeB are registered on the virtual Ethernet bus.",
        "Transmit a payload from NodeA to NodeB using EthernetNode::transmit.",
        "The resulting frame shall use NodeA as source MAC and preserve destination, EtherType, and payload."
    );

    return executeEthernetTest(test, "Node-generated frame preserves configured source and requested data", []()
        {
            VirtualEthernetBus bus;
            EthernetNode nodeA(NodeA);

            bus.registerNode(NodeA);
            bus.registerNode(NodeB);

            const std::vector<std::uint8_t> payload{ 0xAA, 0xBB, 0xCC };

            const bool accepted = nodeA.transmit(
                bus,
                NodeB,
                TestEtherType,
                payload,
                4.0
            );

            const EthernetFrame delivered = bus.receive(NodeB);

            const bool passed =
                accepted &&
                delivered.sourceMac == NodeA &&
                delivered.destinationMac == NodeB &&
                delivered.etherType == TestEtherType &&
                delivered.payload == payload;

            return std::pair{
                passed,
                std::string("source=") + delivered.sourceMac.toString() +
                ", destination=" + delivered.destinationMac.toString()
            };
        });
}

TestCase TestRunner::runSimulationEthernetOwnershipTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_030",
        "SimulationEngine Ethernet Ownership",
        "ETH-REQ-030",
        "Verify that SimulationEngine owns and initializes the active VirtualEthernetBus and configured Ethernet nodes.",
        "A new SimulationEngine instance is available.",
        "Inspect the engine Ethernet bus and node configuration.",
        "The engine shall expose a 100 Mbit/s Ethernet bus with both runtime Ethernet nodes registered."
    );

    return executeEthernetTest(test, "100 Mbit/s bus; 2 registered runtime nodes", []()
        {
            SimulationEngine engine(LogLevel::Events);

            const auto& bus = engine.getEthernetBus();
            const auto& nodeA = engine.getEthernetNodeA();
            const auto& nodeB = engine.getEthernetNodeB();

            const bool passed =
                bus.getLinkRate() == EthernetTiming::DefaultLinkRate &&
                bus.getRegisteredNodeCount() == 2 &&
                bus.isNodeRegistered(nodeA.getMacAddress()) &&
                bus.isNodeRegistered(nodeB.getMacAddress()) &&
                nodeA.getMacAddress() != nodeB.getMacAddress();

            return std::pair{
                passed,
                std::string("rate=") + std::to_string(bus.getLinkRate()) +
                ", nodes=" + std::to_string(bus.getRegisteredNodeCount())
            };
        });
}

TestCase TestRunner::runCanEthernetCoexistenceTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_031",
        "CAN and Ethernet Coexistence",
        "ETH-REQ-031",
        "Verify that CAN and Ethernet communication coexist within one SimulationEngine instance.",
        "A new SimulationEngine instance is available.",
        "Submit Ethernet traffic and advance the simulation so normal periodic CAN traffic also executes.",
        "Both CAN and Ethernet traces shall contain communication evidence without disabling either subsystem."
    );

    return executeEthernetTest(test, "CAN trace and Ethernet trace both contain traffic", []()
        {
            SimulationEngine engine(LogLevel::Events);

            const MacAddress source = engine.getEthernetNodeA().getMacAddress();
            const MacAddress destination = engine.getEthernetNodeB().getMacAddress();

            const bool accepted = engine.submitEthernetFrame(
                source,
                destination,
                TestEtherType,
                { 0x01, 0x02, 0x03, 0x04 }
            );

            engine.update(10.0);

            const bool passed =
                accepted &&
                !engine.getCanTrace().empty() &&
                !engine.getEthernetBus().getTrace().empty() &&
                engine.getEthernetNodeB().hasReceivedFrame();

            return std::pair{
                passed,
                std::string("CAN trace=") + std::to_string(engine.getCanTrace().size()) +
                ", Ethernet trace=" + std::to_string(engine.getEthernetBus().getTrace().size())
            };
        });
}

TestCase TestRunner::runEthernetRuntimeProcessingTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_032",
        "Ethernet Runtime Processing",
        "ETH-REQ-032",
        "Verify that pending Ethernet communication is processed by SimulationEngine runtime execution.",
        "A frame is submitted from runtime Ethernet Node A to runtime Ethernet Node B.",
        "Inspect NodeB before and after advancing SimulationEngine time.",
        "The frame shall remain pending before update and shall be delivered to NodeB during runtime processing."
    );

    return executeEthernetTest(test, "Pending before update; delivered after update", []()
        {
            SimulationEngine engine(LogLevel::Events);

            const MacAddress source = engine.getEthernetNodeA().getMacAddress();
            const MacAddress destination = engine.getEthernetNodeB().getMacAddress();

            const bool accepted = engine.submitEthernetFrame(
                source,
                destination,
                TestEtherType,
                { 0x10, 0x20, 0x30 }
            );

            const bool pendingBeforeUpdate =
                engine.getEthernetBus().getPendingFrameCount(destination) == 1 &&
                !engine.getEthernetNodeB().hasReceivedFrame();

            engine.update(1.0);

            const auto& received = engine.getEthernetNodeB().getReceivedFrames();

            const bool passed =
                accepted &&
                pendingBeforeUpdate &&
                engine.getEthernetBus().getPendingFrameCount(destination) == 0 &&
                received.size() == 1 &&
                received.front().sourceMac == source &&
                received.front().destinationMac == destination;

            return std::pair{
                passed,
                std::string("pendingBefore=") + (pendingBeforeUpdate ? "true" : "false") +
                ", receivedAfter=" + std::to_string(received.size())
            };
        });
}

TestCase TestRunner::runEthernetRuntimeResetTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_033",
        "Ethernet Runtime Reset",
        "ETH-REQ-033",
        "Verify that SimulationEngine reset clears Ethernet runtime state and reinitializes configured Ethernet nodes.",
        "Runtime Ethernet traffic has been transmitted and received.",
        "Reset SimulationEngine and inspect Ethernet bus, statistics, timing, and node receive history.",
        "Runtime Ethernet evidence shall be cleared while both configured nodes remain registered for the new simulation run."
    );

    return executeEthernetTest(test, "Ethernet runtime state cleared and 2 nodes re-registered", []()
        {
            SimulationEngine engine(LogLevel::Events);

            const MacAddress source = engine.getEthernetNodeA().getMacAddress();
            const MacAddress destination = engine.getEthernetNodeB().getMacAddress();

            engine.submitEthernetFrame(
                source,
                destination,
                TestEtherType,
                { 0x55, 0x66, 0x77 }
            );

            engine.update(1.0);

            const bool evidenceBeforeReset =
                !engine.getEthernetBus().getTrace().empty() &&
                engine.getEthernetBus().getStatistics().getFrameCount() == 1 &&
                engine.getEthernetNodeB().hasReceivedFrame();

            engine.reset();

            const auto& bus = engine.getEthernetBus();

            const bool passed =
                evidenceBeforeReset &&
                bus.getTrace().empty() &&
                bus.getStatistics().getFrameCount() == 0 &&
                bus.getStatistics().getPayloadByteCount() == 0 &&
                bus.getBusyUntilMs() == 0.0 &&
                bus.getTotalPendingFrameCount() == 0 &&
                bus.getRegisteredNodeCount() == 2 &&
                !engine.getEthernetNodeA().hasReceivedFrame() &&
                !engine.getEthernetNodeB().hasReceivedFrame();

            return std::pair{
                passed,
                std::string("trace=") + std::to_string(bus.getTrace().size()) +
                ", frames=" + std::to_string(bus.getStatistics().getFrameCount()) +
                ", nodes=" + std::to_string(bus.getRegisteredNodeCount())
            };
        });
}

TestCase TestRunner::runEthernetCanRegressionTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_034",
        "Existing CAN Regression",
        "ETH-REQ-034",
        "Verify that Automotive Ethernet integration preserves existing CAN runtime behavior.",
        "A SimulationEngine instance contains both CAN and Ethernet subsystems.",
        "Advance the simulation through periodic ABS, Powertrain, and Steering CAN transmissions.",
        "The CAN bus shall retain its configured bitrate, produce trace evidence, and preserve the expected periodic ECU identifiers."
    );

    return executeEthernetTest(test, "CAN bitrate and periodic ECU traffic preserved", []()
        {
            SimulationEngine engine(LogLevel::Events);

            engine.update(40.0);

            const auto& trace = engine.getCanTrace();

            bool absSeen = false;
            bool powertrainSeen = false;
            bool steeringSeen = false;

            for (const auto& entry : trace)
            {
                if (entry.arbitrationId == 0x080)
                    absSeen = true;

                if (entry.arbitrationId == 0x100)
                    powertrainSeen = true;

                if (entry.arbitrationId == 0x120)
                    steeringSeen = true;
            }

            const bool passed =
                engine.getCanBitrate() == 500000 &&
                !trace.empty() &&
                engine.getCanStatistics().getFramesTransmitted() > 0 &&
                absSeen &&
                powertrainSeen &&
                steeringSeen;

            return std::pair{
                passed,
                std::string("bitrate=") + std::to_string(engine.getCanBitrate()) +
                ", trace=" + std::to_string(trace.size()) +
                ", ABS=" + (absSeen ? "true" : "false") +
                ", PT=" + (powertrainSeen ? "true" : "false") +
                ", STR=" + (steeringSeen ? "true" : "false")
            };
        });
}

TestCase TestRunner::runEthernetDiagnosticRegressionTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_035",
        "Existing Diagnostic Regression",
        "ETH-REQ-035",
        "Verify that Automotive Ethernet integration preserves the existing UDS runtime diagnostic path.",
        "A SimulationEngine instance is in the default diagnostic session.",
        "Submit an Extended Diagnostic Session request and advance runtime processing until completion.",
        "The UDS request shall complete through the existing CAN/ISO-TP runtime path and return a positive Extended Session response."
    );

    return executeEthernetTest(test, "Positive UDS Extended Session response preserved", []()
        {
            SimulationEngine engine(LogLevel::Events);

            UdsRequest request;
            request.serviceId = 0x10;
            request.payload = { 0x03 };

            engine.submitUdsRequest(request);

            for (
                int step = 0;
                step < 20 && engine.hasPendingUdsTransaction();
                ++step)
            {
                engine.update(10.0);
            }

            const bool completed =
                !engine.hasPendingUdsTransaction() &&
                engine.hasCompletedUdsResponse();

            bool positive = false;
            std::uint8_t responseSid = 0;

            if (completed)
            {
                const UdsResponse& response =
                    engine.getCompletedUdsResponse();

                responseSid = response.serviceId;

                positive =
                    response.isPositive() &&
                    response.serviceId == 0x50 &&
                    !response.payload.empty() &&
                    response.payload.front() == 0x03;
            }

            const bool passed = completed && positive;

            return std::pair{
                passed,
                std::string("completed=") + (completed ? "true" : "false") +
                ", positive=" + (positive ? "true" : "false") +
                ", SID=0x" + std::to_string(static_cast<unsigned int>(responseSid))
            };
        });
}

TestCase TestRunner::runEthernetBaselineRegressionTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_036",
        "Baseline v1.6 Regression Sentinel",
        "ETH-REQ-036",
        "Provide dedicated Ethernet-milestone regression evidence while the complete v1.6 suite remains part of the final v1.7 execution.",
        "Baseline v1.6 is verification-closed at 217 PASS / 0 FAIL.",
        "Exercise representative system, CAN, diagnostics, and reset behavior after Ethernet integration.",
        "Representative baseline behavior shall remain valid; full ETH-REQ-036 closure additionally requires all 217 legacy v1.6 tests to pass in the complete suite."
    );

    return executeEthernetTest(test, "Representative v1.6 baseline behavior preserved", []()
        {
            SimulationEngine engine(LogLevel::Events);

            engine.update(20.0);

            const bool runtimeProgressed =
                engine.getCurrentTimeMs() == 20.0;

            const bool canPreserved =
                engine.getCanBitrate() == 500000 &&
                !engine.getCanTrace().empty();

            UdsRequest request;
            request.serviceId = 0x10;
            request.payload = { 0x03 };

            engine.submitUdsRequest(request);

            for (
                int step = 0;
                step < 20 && engine.hasPendingUdsTransaction();
                ++step)
            {
                engine.update(10.0);
            }

            const bool diagnosticsPreserved =
                !engine.hasPendingUdsTransaction() &&
                engine.hasCompletedUdsResponse() &&
                engine.getCompletedUdsResponse().isPositive();

            engine.reset();

            const bool resetPreserved =
                engine.getCurrentTimeMs() == 0.0 &&
                engine.getCanTrace().empty() &&
                !engine.hasPendingUdsTransaction() &&
                !engine.hasCompletedUdsResponse();

            const bool passed =
                runtimeProgressed &&
                canPreserved &&
                diagnosticsPreserved &&
                resetPreserved;

            return std::pair{
                passed,
                std::string("runtime=") + (runtimeProgressed ? "true" : "false") +
                ", CAN=" + (canPreserved ? "true" : "false") +
                ", UDS=" + (diagnosticsPreserved ? "true" : "false") +
                ", reset=" + (resetPreserved ? "true" : "false")
            };
        });
}

TestCase TestRunner::runEthernetDeterminismTest()
{
    TestCase test = createEthernetTestCase(
        "TC_ETH_037",
        "Deterministic Ethernet Communication",
        "ETH-REQ-037",
        "Verify deterministic Ethernet delivery, timing, trace, and statistics for identical initial state and input sequence.",
        "Two independent SimulationEngine instances start from equivalent initial state.",
        "Submit identical Ethernet frames at identical simulation times and execute identical update sequences.",
        "Both simulations shall produce equivalent Ethernet delivery history, trace timing, and statistics."
    );

    return executeEthernetTest(test, "Equivalent Ethernet results for identical runs", []()
        {
            SimulationEngine first(LogLevel::Events);
            SimulationEngine second(LogLevel::Events);

            const MacAddress firstSource =
                first.getEthernetNodeA().getMacAddress();

            const MacAddress firstDestination =
                first.getEthernetNodeB().getMacAddress();

            const MacAddress secondSource =
                second.getEthernetNodeA().getMacAddress();

            const MacAddress secondDestination =
                second.getEthernetNodeB().getMacAddress();

            const std::vector<std::uint8_t> payloadA{
                0x01, 0x02, 0x03, 0x04
            };

            const std::vector<std::uint8_t> payloadB{
                0x10, 0x20, 0x30, 0x40, 0x50
            };

            const bool firstAcceptedA = first.submitEthernetFrame(
                firstSource,
                firstDestination,
                TestEtherType,
                payloadA
            );

            const bool secondAcceptedA = second.submitEthernetFrame(
                secondSource,
                secondDestination,
                TestEtherType,
                payloadA
            );

            first.update(5.0);
            second.update(5.0);

            const bool firstAcceptedB = first.submitEthernetFrame(
                firstSource,
                firstDestination,
                TestEtherType,
                payloadB
            );

            const bool secondAcceptedB = second.submitEthernetFrame(
                secondSource,
                secondDestination,
                TestEtherType,
                payloadB
            );

            first.update(10.0);
            second.update(10.0);

            const auto& firstTrace = first.getEthernetBus().getTrace();
            const auto& secondTrace = second.getEthernetBus().getTrace();

            bool traceEquivalent =
                firstTrace.size() == secondTrace.size();

            if (traceEquivalent)
            {
                for (std::size_t index = 0; index < firstTrace.size(); ++index)
                {
                    const auto& lhs = firstTrace[index];
                    const auto& rhs = secondTrace[index];

                    if (
                        lhs.frame != rhs.frame ||
                        !nearlyEqual(lhs.requestTimeMs, rhs.requestTimeMs) ||
                        !nearlyEqual(lhs.startTimeMs, rhs.startTimeMs) ||
                        !nearlyEqual(lhs.transmissionTimeMs, rhs.transmissionTimeMs) ||
                        !nearlyEqual(lhs.completionTimeMs, rhs.completionTimeMs) ||
                        !nearlyEqual(lhs.waitingTimeMs, rhs.waitingTimeMs))
                    {
                        traceEquivalent = false;
                        break;
                    }
                }
            }

            const auto& firstStats =
                first.getEthernetBus().getStatistics();

            const auto& secondStats =
                second.getEthernetBus().getStatistics();

            const bool statisticsEquivalent =
                firstStats.getFrameCount() == secondStats.getFrameCount() &&
                firstStats.getPayloadByteCount() == secondStats.getPayloadByteCount();

            const auto& firstReceived =
                first.getEthernetNodeB().getReceivedFrames();

            const auto& secondReceived =
                second.getEthernetNodeB().getReceivedFrames();

            const bool deliveryEquivalent =
                firstReceived == secondReceived;

            const bool passed =
                firstAcceptedA &&
                secondAcceptedA &&
                firstAcceptedB &&
                secondAcceptedB &&
                traceEquivalent &&
                statisticsEquivalent &&
                deliveryEquivalent;

            return std::pair{
                passed,
                std::string("trace=") + (traceEquivalent ? "true" : "false") +
                ", stats=" + (statisticsEquivalent ? "true" : "false") +
                ", delivery=" + (deliveryEquivalent ? "true" : "false") +
                ", frames=" + std::to_string(firstTrace.size())
            };
        });
}

