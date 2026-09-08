#include "TestRunner.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "../can/CanMessageDefinitions.h"
#include "../diagnostics/DtcDefinitions.h"
#include "../simulation/SimulationEngine.h"

namespace
{
    TestCase createDfxTest(
        const std::string& id,
        const std::string& name,
        const std::string& requirementId,
        const std::string& description,
        const std::string& precondition,
        const std::string& stimulus,
        const std::string& expected)
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
            expected;

        test.result.status =
            TestStatus::Running;

        test.result.expected =
            expected;

        return test;
    }

    void finishDfxTest(
        TestCase& test,
        bool passed,
        const std::string& actual)
    {
        test.result.actual =
            actual;

        test.result.status =
            passed
            ? TestStatus::Passed
            : TestStatus::Failed;

        test.result.message =
            passed
            ? "Diagnostic fault expansion behavior verified."
            : "Diagnostic fault expansion behavior did not match the requirement.";
    }

    bool traceContainsIdFromIndex(
        const std::vector<CanTraceEntry>& trace,
        std::size_t startIndex,
        std::uint32_t arbitrationId)
    {
        if (
            startIndex >
            trace.size()
            )
        {
            return false;
        }

        return std::any_of(
            trace.begin() +
            static_cast<std::ptrdiff_t>(
                startIndex
                ),
            trace.end(),
            [arbitrationId](
                const CanTraceEntry& entry)
            {
                return entry.arbitrationId ==
                    arbitrationId;
            }
        );
    }

    void runFor(
        SimulationEngine& engine,
        double durationMs)
    {
        constexpr double stepMs =
            10.0;

        const double targetTimeMs =
            engine.getCurrentTimeMs() +
            durationMs;

        while (
            engine.getCurrentTimeMs() <
            targetTimeMs
            )
        {
            engine.update(
                stepMs
            );
        }
    }
}

TestCase TestRunner::runAbsCanFaultInjectionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_001",
            "ABS CAN Fault Injection",
            "DFX-REQ-001",
            "Verify ABS CAN communication fault injection state and DTC activation.",
            "SimulationEngine initialized with no active communication fault.",
            "Inject the ABS CAN communication fault.",
            "ABS communication fault state shall be active and its DTC shall be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.injectAbsCanCommunicationFault();

    const bool faultActive =
        engine.isAbsCanCommunicationFaultActive();

    const bool dtcActive =
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::AbsCanCommunication
        );

    const bool passed =
        faultActive &&
        dtcActive;

    finishDfxTest(
        test,
        passed,
        std::string("faultActive=") +
        (faultActive ? "true" : "false") +
        ", dtcActive=" +
        (dtcActive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runAbsCanTransmissionSuppressionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_002",
            "ABS CAN Transmission Suppression",
            "DFX-REQ-002",
            "Verify ABS_WHEEL_STATE is suppressed while the ABS CAN communication fault is active.",
            "SimulationEngine initialized and periodic CAN processing operational.",
            "Inject ABS CAN communication fault and advance simulation.",
            "No new ABS_WHEEL_STATE frame shall be transmitted while unrelated CAN traffic continues."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    runFor(engine, 30.0);

    engine.injectAbsCanCommunicationFault();

    const std::size_t traceStart =
        engine.getCanTrace().size();

    runFor(engine, 60.0);

    const auto& trace =
        engine.getCanTrace();

    const bool absSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::ABS_WHEEL_STATE.id
        );

    const bool powertrainSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::POWERTRAIN_STATE.id
        );

    const bool steeringSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::STEERING_STATE.id
        );

    const bool passed =
        !absSeen &&
        powertrainSeen &&
        steeringSeen;

    finishDfxTest(
        test,
        passed,
        std::string("absSeen=") +
        (absSeen ? "true" : "false") +
        ", powertrainSeen=" +
        (powertrainSeen ? "true" : "false") +
        ", steeringSeen=" +
        (steeringSeen ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runAbsCanRecoveryTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_003",
            "ABS CAN Fault Recovery",
            "DFX-REQ-003",
            "Verify ABS CAN communication resumes after the injected fault is cleared.",
            "ABS CAN communication fault is active.",
            "Clear the ABS CAN communication fault and advance simulation.",
            "ABS_WHEEL_STATE transmission shall resume and the ABS communication DTC shall not be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    engine.injectAbsCanCommunicationFault();
    runFor(engine, 30.0);

    engine.clearAbsCanCommunicationFault();

    const std::size_t traceStart =
        engine.getCanTrace().size();

    runFor(engine, 40.0);

    const bool absSeen =
        traceContainsIdFromIndex(
            engine.getCanTrace(),
            traceStart,
            CanMessageDefinitions::ABS_WHEEL_STATE.id
        );

    const bool faultCleared =
        !engine.isAbsCanCommunicationFaultActive();

    const bool dtcInactive =
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::AbsCanCommunication
        );

    const bool passed =
        absSeen &&
        faultCleared &&
        dtcInactive;

    finishDfxTest(
        test,
        passed,
        std::string("absSeen=") +
        (absSeen ? "true" : "false") +
        ", faultCleared=" +
        (faultCleared ? "true" : "false") +
        ", dtcInactive=" +
        (dtcInactive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runPowertrainCanFaultInjectionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_004",
            "Powertrain CAN Fault Injection",
            "DFX-REQ-004",
            "Verify Powertrain CAN communication fault injection state and DTC activation.",
            "SimulationEngine initialized with no active communication fault.",
            "Inject the Powertrain CAN communication fault.",
            "Powertrain communication fault state and its DTC shall become active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    engine.injectPowertrainCanCommunicationFault();

    const bool faultActive =
        engine.isPowertrainCanCommunicationFaultActive();

    const bool dtcActive =
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        );

    const bool passed =
        faultActive &&
        dtcActive;

    finishDfxTest(
        test,
        passed,
        std::string("faultActive=") +
        (faultActive ? "true" : "false") +
        ", dtcActive=" +
        (dtcActive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runPowertrainCanTransmissionSuppressionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_005",
            "Powertrain CAN Transmission Suppression",
            "DFX-REQ-005",
            "Verify POWERTRAIN_STATE is suppressed while its CAN communication fault is active.",
            "SimulationEngine initialized and periodic CAN processing operational.",
            "Inject Powertrain CAN communication fault and advance simulation.",
            "No new POWERTRAIN_STATE frame shall be transmitted while unrelated CAN traffic continues."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    runFor(engine, 30.0);

    engine.injectPowertrainCanCommunicationFault();

    const std::size_t traceStart =
        engine.getCanTrace().size();

    runFor(engine, 60.0);

    const auto& trace =
        engine.getCanTrace();

    const bool powertrainSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::POWERTRAIN_STATE.id
        );

    const bool absSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::ABS_WHEEL_STATE.id
        );

    const bool steeringSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::STEERING_STATE.id
        );

    const bool passed =
        !powertrainSeen &&
        absSeen &&
        steeringSeen;

    finishDfxTest(
        test,
        passed,
        std::string("powertrainSeen=") +
        (powertrainSeen ? "true" : "false") +
        ", absSeen=" +
        (absSeen ? "true" : "false") +
        ", steeringSeen=" +
        (steeringSeen ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runPowertrainCanRecoveryTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_006",
            "Powertrain CAN Fault Recovery",
            "DFX-REQ-006",
            "Verify Powertrain CAN communication resumes after clearing the injected fault.",
            "Powertrain CAN communication fault is active.",
            "Clear the fault and advance simulation.",
            "POWERTRAIN_STATE transmission shall resume and the associated DTC shall not be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    engine.injectPowertrainCanCommunicationFault();
    runFor(engine, 30.0);

    engine.clearPowertrainCanCommunicationFault();

    const std::size_t traceStart =
        engine.getCanTrace().size();

    runFor(engine, 50.0);

    const bool frameSeen =
        traceContainsIdFromIndex(
            engine.getCanTrace(),
            traceStart,
            CanMessageDefinitions::POWERTRAIN_STATE.id
        );

    const bool faultCleared =
        !engine.isPowertrainCanCommunicationFaultActive();

    const bool dtcInactive =
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        );

    const bool passed =
        frameSeen &&
        faultCleared &&
        dtcInactive;

    finishDfxTest(
        test,
        passed,
        std::string("frameSeen=") +
        (frameSeen ? "true" : "false") +
        ", faultCleared=" +
        (faultCleared ? "true" : "false") +
        ", dtcInactive=" +
        (dtcInactive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runSteeringCanFaultInjectionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_007",
            "Steering CAN Fault Injection",
            "DFX-REQ-007",
            "Verify Steering CAN communication fault injection state and DTC activation.",
            "SimulationEngine initialized with no active communication fault.",
            "Inject the Steering CAN communication fault.",
            "Steering communication fault state and its DTC shall become active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    engine.injectSteeringCanCommunicationFault();

    const bool faultActive =
        engine.isSteeringCanCommunicationFaultActive();

    const bool dtcActive =
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::SteeringCanCommunication
        );

    const bool passed =
        faultActive &&
        dtcActive;

    finishDfxTest(
        test,
        passed,
        std::string("faultActive=") +
        (faultActive ? "true" : "false") +
        ", dtcActive=" +
        (dtcActive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runSteeringCanTransmissionSuppressionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_008",
            "Steering CAN Transmission Suppression",
            "DFX-REQ-008",
            "Verify STEERING_STATE is suppressed while its CAN communication fault is active.",
            "SimulationEngine initialized and periodic CAN processing operational.",
            "Inject Steering CAN communication fault and advance simulation.",
            "No new STEERING_STATE frame shall be transmitted while unrelated CAN traffic continues."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    runFor(engine, 30.0);

    engine.injectSteeringCanCommunicationFault();

    const std::size_t traceStart =
        engine.getCanTrace().size();

    runFor(engine, 60.0);

    const auto& trace =
        engine.getCanTrace();

    const bool steeringSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::STEERING_STATE.id
        );

    const bool absSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::ABS_WHEEL_STATE.id
        );

    const bool powertrainSeen =
        traceContainsIdFromIndex(
            trace,
            traceStart,
            CanMessageDefinitions::POWERTRAIN_STATE.id
        );

    const bool passed =
        !steeringSeen &&
        absSeen &&
        powertrainSeen;

    finishDfxTest(
        test,
        passed,
        std::string("steeringSeen=") +
        (steeringSeen ? "true" : "false") +
        ", absSeen=" +
        (absSeen ? "true" : "false") +
        ", powertrainSeen=" +
        (powertrainSeen ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runSteeringCanRecoveryTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_009",
            "Steering CAN Fault Recovery",
            "DFX-REQ-009",
            "Verify Steering CAN communication resumes after clearing the injected fault.",
            "Steering CAN communication fault is active.",
            "Clear the fault and advance simulation.",
            "STEERING_STATE transmission shall resume and the associated DTC shall not be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();
    engine.injectSteeringCanCommunicationFault();
    runFor(engine, 30.0);

    engine.clearSteeringCanCommunicationFault();

    const std::size_t traceStart =
        engine.getCanTrace().size();

    runFor(engine, 50.0);

    const bool frameSeen =
        traceContainsIdFromIndex(
            engine.getCanTrace(),
            traceStart,
            CanMessageDefinitions::STEERING_STATE.id
        );

    const bool faultCleared =
        !engine.isSteeringCanCommunicationFaultActive();

    const bool dtcInactive =
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::SteeringCanCommunication
        );

    const bool passed =
        frameSeen &&
        faultCleared &&
        dtcInactive;

    finishDfxTest(
        test,
        passed,
        std::string("frameSeen=") +
        (frameSeen ? "true" : "false") +
        ", faultCleared=" +
        (faultCleared ? "true" : "false") +
        ", dtcInactive=" +
        (dtcInactive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runEthernetNodeAFaultTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_010",
            "Ethernet Node A Communication Fault",
            "DFX-REQ-010",
            "Verify Node A Ethernet communication is blocked while its injected communication fault is active.",
            "SimulationEngine Ethernet runtime initialized with Node A and Node B.",
            "Inject Node A communication fault and submit frames with Node A as source and destination.",
            "Both Node A communication directions shall be rejected and the communication DTC shall be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    const MacAddress nodeA =
        engine.getEthernetNodeA().getMacAddress();

    const MacAddress nodeB =
        engine.getEthernetNodeB().getMacAddress();

    engine.injectEthernetNodeACommunicationFault();

    const bool aToB =
        engine.submitEthernetFrame(
            nodeA,
            nodeB,
            0x88B5,
            { 0x01, 0x02 }
        );

    const bool bToA =
        engine.submitEthernetFrame(
            nodeB,
            nodeA,
            0x88B5,
            { 0x03, 0x04 }
        );

    const bool faultActive =
        engine.isEthernetNodeACommunicationFaultActive();

    const bool dtcActive =
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::EthernetNodeACommunication
        );

    const bool passed =
        !aToB &&
        !bToA &&
        faultActive &&
        dtcActive;

    finishDfxTest(
        test,
        passed,
        std::string("aToB=") +
        (aToB ? "accepted" : "blocked") +
        ", bToA=" +
        (bToA ? "accepted" : "blocked") +
        ", dtcActive=" +
        (dtcActive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runEthernetNodeARecoveryTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_011",
            "Ethernet Node A Fault Recovery",
            "DFX-REQ-011",
            "Verify Node A Ethernet communication is restored after clearing the injected fault.",
            "Node A Ethernet communication fault is active.",
            "Clear Node A communication fault and submit a valid Node A to Node B frame.",
            "The frame shall be accepted and the Node A communication DTC shall not be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    const MacAddress nodeA =
        engine.getEthernetNodeA().getMacAddress();

    const MacAddress nodeB =
        engine.getEthernetNodeB().getMacAddress();

    engine.injectEthernetNodeACommunicationFault();
    engine.clearEthernetNodeACommunicationFault();

    const bool accepted =
        engine.submitEthernetFrame(
            nodeA,
            nodeB,
            0x88B5,
            { 0x10, 0x20, 0x30 }
        );

    const bool faultCleared =
        !engine.isEthernetNodeACommunicationFaultActive();

    const bool dtcInactive =
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::EthernetNodeACommunication
        );

    const bool passed =
        accepted &&
        faultCleared &&
        dtcInactive;

    finishDfxTest(
        test,
        passed,
        std::string("accepted=") +
        (accepted ? "true" : "false") +
        ", faultCleared=" +
        (faultCleared ? "true" : "false") +
        ", dtcInactive=" +
        (dtcInactive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runCommunicationDtcRegistrationTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_012",
            "Distinct Communication DTC Registration",
            "DFX-REQ-012",
            "Verify all four v1.8 communication DTCs are registered and independently activated.",
            "SimulationEngine initialized with the v1.8 DTC registry.",
            "Inject the four communication faults.",
            "All four distinct DTC codes shall exist and be active."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    const auto& manager =
        engine.getDtcManager();

    const bool definitionsExist =
        manager.hasDtc(
            DtcDefinitions::AbsCanCommunication
        ) &&
        manager.hasDtc(
            DtcDefinitions::PowertrainCanCommunication
        ) &&
        manager.hasDtc(
            DtcDefinitions::SteeringCanCommunication
        ) &&
        manager.hasDtc(
            DtcDefinitions::EthernetNodeACommunication
        );

    engine.injectAbsCanCommunicationFault();
    engine.injectPowertrainCanCommunicationFault();
    engine.injectSteeringCanCommunicationFault();
    engine.injectEthernetNodeACommunicationFault();

    const bool allActive =
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::AbsCanCommunication
        ) &&
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        ) &&
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::SteeringCanCommunication
        ) &&
        engine.getDtcManager().isDtcActive(
            DtcDefinitions::EthernetNodeACommunication
        );

    const bool passed =
        definitionsExist &&
        allActive;

    finishDfxTest(
        test,
        passed,
        std::string("definitionsExist=") +
        (definitionsExist ? "true" : "false") +
        ", allActive=" +
        (allActive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runCommunicationFaultResetTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_013",
            "Diagnostic Communication Fault Reset",
            "DFX-REQ-013",
            "Verify reset clears every v1.8 injected communication fault and active communication DTC.",
            "All four v1.8 communication faults are active.",
            "Reset the SimulationEngine.",
            "All v1.8 injected fault states and active communication DTCs shall be cleared."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    engine.injectAbsCanCommunicationFault();
    engine.injectPowertrainCanCommunicationFault();
    engine.injectSteeringCanCommunicationFault();
    engine.injectEthernetNodeACommunicationFault();

    engine.reset();

    const bool faultStatesClear =
        !engine.isAbsCanCommunicationFaultActive() &&
        !engine.isPowertrainCanCommunicationFaultActive() &&
        !engine.isSteeringCanCommunicationFaultActive() &&
        !engine.isEthernetNodeACommunicationFaultActive();

    const bool dtcsInactive =
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::AbsCanCommunication
        ) &&
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        ) &&
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::SteeringCanCommunication
        ) &&
        !engine.getDtcManager().isDtcActive(
            DtcDefinitions::EthernetNodeACommunication
        );

    const bool passed =
        faultStatesClear &&
        dtcsInactive;

    finishDfxTest(
        test,
        passed,
        std::string("faultStatesClear=") +
        (faultStatesClear ? "true" : "false") +
        ", dtcsInactive=" +
        (dtcsInactive ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runCommunicationFaultDeterminismTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_014",
            "Deterministic Communication Fault Behavior",
            "DFX-REQ-014",
            "Verify equivalent fault input sequences produce equivalent runtime fault and communication evidence.",
            "Two independent SimulationEngine instances start from equivalent reset state.",
            "Apply identical Powertrain CAN fault, update, recovery, and update sequences.",
            "Both engines shall produce equivalent fault state, DTC state, and CAN trace identifiers."
        );

    SimulationEngine first(
        LogLevel::Quiet
    );

    SimulationEngine second(
        LogLevel::Quiet
    );

    first.reset();
    second.reset();

    first.injectPowertrainCanCommunicationFault();
    second.injectPowertrainCanCommunicationFault();

    runFor(first, 60.0);
    runFor(second, 60.0);

    first.clearPowertrainCanCommunicationFault();
    second.clearPowertrainCanCommunicationFault();

    runFor(first, 40.0);
    runFor(second, 40.0);

    const auto& firstTrace =
        first.getCanTrace();

    const auto& secondTrace =
        second.getCanTrace();

    bool traceEquivalent =
        firstTrace.size() ==
        secondTrace.size();

    if (traceEquivalent)
    {
        for (
            std::size_t i = 0;
            i < firstTrace.size();
            ++i
            )
        {
            if (
                firstTrace[i].arbitrationId !=
                secondTrace[i].arbitrationId
                )
            {
                traceEquivalent =
                    false;

                break;
            }
        }
    }

    const bool faultStateEquivalent =
        first.isPowertrainCanCommunicationFaultActive() ==
        second.isPowertrainCanCommunicationFaultActive();

    const bool dtcStateEquivalent =
        first.getDtcManager().isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        ) ==
        second.getDtcManager().isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        );

    const bool passed =
        traceEquivalent &&
        faultStateEquivalent &&
        dtcStateEquivalent;

    finishDfxTest(
        test,
        passed,
        std::string("traceEquivalent=") +
        (traceEquivalent ? "true" : "false") +
        ", faultStateEquivalent=" +
        (faultStateEquivalent ? "true" : "false") +
        ", dtcStateEquivalent=" +
        (dtcStateEquivalent ? "true" : "false")
    );

    return test;
}

TestCase TestRunner::runDiagnosticFaultBaselineRegressionTest()
{
    TestCase test =
        createDfxTest(
            "TC_DFX_015",
            "Baseline v1.7 Regression Sentinel",
            "DFX-REQ-015",
            "Provide dedicated v1.8 regression evidence while the complete 254-test v1.7 suite remains part of final verification.",
            "Baseline v1.7 is verification-closed at 254 PASS / 0 FAIL.",
            "Exercise representative simulation, CAN, UDS, Ethernet, fault-reset, and recovery behavior after v1.8 integration.",
            "Representative v1.7 behavior shall remain valid; formal closure additionally requires the complete 254-test legacy suite to pass."
        );

    SimulationEngine engine(
        LogLevel::Quiet
    );

    engine.reset();

    runFor(engine, 30.0);

    const bool canOperational =
        !engine.getCanTrace().empty();

    const MacAddress nodeA =
        engine.getEthernetNodeA().getMacAddress();

    const MacAddress nodeB =
        engine.getEthernetNodeB().getMacAddress();

    const bool ethernetOperational =
        engine.submitEthernetFrame(
            nodeA,
            nodeB,
            0x88B5,
            { 0xAA, 0x55 }
        );

    UdsRequest request;

    request.serviceId =
        0x10;

    request.payload =
    {
        0x03
    };

    engine.submitUdsRequest(
        request
    );

    for (
        int step = 0;
        step < 20 &&
        engine.hasPendingUdsTransaction();
        ++step
        )
    {
        engine.update(
            10.0
        );
    }

    const bool udsOperational =
        !engine.hasPendingUdsTransaction() &&
        engine.hasCompletedUdsResponse() &&
        engine.getCompletedUdsResponse().isPositive();

    engine.injectAbsCanCommunicationFault();
    engine.reset();

    const bool resetOperational =
        engine.getCurrentTimeMs() ==
        0.0 &&
        !engine.isAbsCanCommunicationFaultActive() &&
        engine.getCanTrace().empty();

    const bool passed =
        canOperational &&
        ethernetOperational &&
        udsOperational &&
        resetOperational;

    finishDfxTest(
        test,
        passed,
        std::string("CAN=") +
        (canOperational ? "true" : "false") +
        ", Ethernet=" +
        (ethernetOperational ? "true" : "false") +
        ", UDS=" +
        (udsOperational ? "true" : "false") +
        ", reset=" +
        (resetOperational ? "true" : "false")
    );

    return test;
}
