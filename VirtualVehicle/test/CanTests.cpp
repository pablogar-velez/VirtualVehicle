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

TestCase TestRunner::runCanArbitrationPriorityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_001";

    test.name =
        "CAN Arbitration Priority";

    test.description =
        "Verify that the CAN frame with the numerically "
        "lowest identifier wins arbitration.";

    test.requirementId =
        "CAN-REQ-003";

    test.precondition =
        "Multiple frames shall be pending at the same time.";

    test.stimulus =
        "Queue CAN IDs 0x120, 0x100 and 0x080.";

    test.expectedResult =
        "CAN ID 0x080 shall win arbitration.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame steeringFrame{};
    steeringFrame.arbitrationId =
        0x120;
    steeringFrame.dlc =
        4;

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId =
        0x100;
    powertrainFrame.dlc =
        6;

    CanFrame absFrame{};
    absFrame.arbitrationId =
        0x080;
    absFrame.dlc =
        5;

    bus.transmit(
        steeringFrame,
        0.0
    );

    bus.transmit(
        powertrainFrame,
        0.0
    );

    bus.transmit(
        absFrame,
        0.0
    );

    const CanFrame winner =
        bus.receive(
            0.0
        );

    test.result.expected =
        "Winner = 0x080";

    test.result.actual =
        winner.arbitrationId ==
        0x080
        ? "Winner = 0x080"
        : "Unexpected arbitration winner";

    if (
        winner.arbitrationId ==
        CanMessageDefinitions::ABS_WHEEL_STATE.id
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Lowest CAN identifier correctly won arbitration.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN arbitration priority was incorrect.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanBitrateTest()
{
    TestCase test;

    test.id =
        "TC_CAN_002";

    test.name =
        "CAN Bitrate";

    test.description =
        "Verify that the Virtual CAN Bus operates "
        "at the configured 500 kbit/s bitrate.";

    test.requirementId =
        "CAN-REQ-001";

    test.precondition =
        "Virtual CAN Bus shall be constructed.";

    test.stimulus =
        "Read configured CAN bitrate.";

    test.expectedResult =
        "Bitrate shall equal 500000 bit/s.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    const std::uint32_t bitrate =
        bus.getBitrate();

    test.result.expected =
        "500000 bit/s";

    test.result.actual =
        std::to_string(
            bitrate
        ) +
        " bit/s";

    if (bitrate == 500000)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Virtual CAN Bus bitrate is configured correctly.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Virtual CAN Bus bitrate does not match requirement.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanTraceRecordingTest()
{
    TestCase test;

    test.id =
        "TC_CAN_003";

    test.name =
        "CAN Trace Recording";

    test.description =
        "Verify that a transmitted CAN frame "
        "is recorded in the CAN trace.";

    test.requirementId =
        "CAN-REQ-008";

    test.precondition =
        "CAN trace shall initially be empty.";

    test.stimulus =
        "Transmit and receive one CAN frame.";

    test.expectedResult =
        "CAN trace shall contain one transmitted frame.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        0x100;

    frame.dlc =
        2;

    frame.data[0] =
        0xAA;

    frame.data[1] =
        0x55;

    bus.transmit(
        frame,
        0.0
    );

    bus.receive(
        0.0
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Trace size = 1, ID = 0x100";

    if (!trace.empty())
    {
        test.result.actual =
            "Trace size = " +
            std::to_string(
                trace.size()
            );

        if (
            trace.size() == 1 &&
            trace[0].arbitrationId == 0x100 &&
            trace[0].data[0] == 0xAA &&
            trace[0].data[1] == 0x55
            )
        {
            test.result.status =
                TestStatus::Passed;

            test.result.message =
                "CAN transmission was recorded correctly.";
        }
        else
        {
            test.result.status =
                TestStatus::Failed;

            test.result.message =
                "CAN trace content did not match transmission.";
        }
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was created.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanArbitrationStatisticsTest()
{
    TestCase test;

    test.id =
        "TC_CAN_004";

    test.name =
        "CAN Arbitration Statistics";

    test.description =
        "Verify that CAN arbitration events "
        "are recorded in statistics.";

    test.requirementId =
        "CAN-REQ-014";

    test.precondition =
        "At least two CAN frames shall be pending.";

    test.stimulus =
        "Request transmission of multiple frames.";

    test.expectedResult =
        "Arbitration counter shall increment.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frameA{};
    frameA.arbitrationId =
        0x120;
    frameA.dlc =
        1;

    CanFrame frameB{};
    frameB.arbitrationId =
        0x080;
    frameB.dlc =
        1;

    bus.transmit(
        frameA,
        0.0
    );

    bus.transmit(
        frameB,
        0.0
    );

    bus.receive(
        0.0
    );

    const std::uint64_t arbitrationCount =
        bus
        .getStatistics()
        .getArbitrationCount();

    test.result.expected =
        "Arbitration count >= 1";

    test.result.actual =
        "Arbitration count = " +
        std::to_string(
            arbitrationCount
        );

    if (arbitrationCount >= 1)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN arbitration event was recorded.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN arbitration statistics were not updated.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runAbsMessagePeriodicityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_005";

    test.name =
        "ABS Message Periodicity";

    test.description =
        "Verify that ABS_WHEEL_STATE transmission requests "
        "occur every 10 ms in simulation time.";

    test.requirementId =
        "ABS-REQ-007";

    test.precondition =
        "Simulation engine shall be reset.";

    test.stimulus =
        "Run the simulation for 100 ms.";

    test.expectedResult =
        "ABS transmission request period shall equal 10 ms.";

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

    const auto& trace =
        engine.getCanTrace();

    bool previousFound =
        false;

    double previousRequestTimeMs =
        0.0;

    bool periodValid =
        true;

    std::size_t frameCount =
        0;

    constexpr double toleranceMs =
        0.001;

    for (const CanTraceEntry& entry : trace)
    {
        if (
            entry.arbitrationId !=
            CanMessageDefinitions::ABS_WHEEL_STATE.id
            )
        {
            continue;
        }

        ++frameCount;

        if (previousFound)
        {
            const double periodMs =
                entry.requestTimeMs -
                previousRequestTimeMs;

            if (
                std::fabs(
                    periodMs -
                    CanMessageDefinitions::ABS_WHEEL_STATE.periodMs
                ) >
                toleranceMs
                )
            {
                periodValid =
                    false;

                break;
            }
        }

        previousRequestTimeMs =
            entry.requestTimeMs;

        previousFound =
            true;
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "ABS request period = 10.000 ms";

    test.result.actual =
        "ABS frames checked = " +
        std::to_string(
            frameCount
        );

    if (
        frameCount >= 2 &&
        periodValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "ABS_WHEEL_STATE periodicity matched the "
            "configured 10 ms period.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "ABS_WHEEL_STATE request periodicity did not "
            "match the configured 10 ms period.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runPowertrainMessagePeriodicityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_006";

    test.name =
        "Powertrain Message Periodicity";

    test.description =
        "Verify that POWERTRAIN_STATE transmission requests "
        "occur every 20 ms in simulation time.";

    test.requirementId =
        "PT-REQ-006";

    test.precondition =
        "Simulation engine shall be reset.";

    test.stimulus =
        "Run the simulation for 100 ms.";

    test.expectedResult =
        "Powertrain transmission request period shall equal 20 ms.";

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

    const auto& trace =
        engine.getCanTrace();

    bool previousFound =
        false;

    double previousRequestTimeMs =
        0.0;

    bool periodValid =
        true;

    std::size_t frameCount =
        0;

    constexpr double toleranceMs =
        0.001;

    for (const CanTraceEntry& entry : trace)
    {
        if (
            entry.arbitrationId !=
            CanMessageDefinitions::POWERTRAIN_STATE.id
            )
        {
            continue;
        }

        ++frameCount;

        if (previousFound)
        {
            const double periodMs =
                entry.requestTimeMs -
                previousRequestTimeMs;

            if (
                std::fabs(
                    periodMs -
                    CanMessageDefinitions::POWERTRAIN_STATE.periodMs
                ) >
                toleranceMs
                )
            {
                periodValid =
                    false;

                break;
            }
        }

        previousRequestTimeMs =
            entry.requestTimeMs;

        previousFound =
            true;
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Powertrain request period = 20.000 ms";

    test.result.actual =
        "Powertrain frames checked = " +
        std::to_string(
            frameCount
        );

    if (
        frameCount >= 2 &&
        periodValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "POWERTRAIN_STATE periodicity matched the "
            "configured 20 ms period.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "POWERTRAIN_STATE request periodicity did not "
            "match the configured 20 ms period.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runSteeringMessagePeriodicityTest()
{
    TestCase test;

    test.id =
        "TC_CAN_007";

    test.name =
        "Steering Message Periodicity";

    test.description =
        "Verify that STEERING_STATE transmission requests "
        "occur every 20 ms in simulation time.";

    test.requirementId =
        "STR-REQ-003";

    test.precondition =
        "Simulation engine shall be reset.";

    test.stimulus =
        "Run the simulation for 100 ms.";

    test.expectedResult =
        "Steering transmission request period shall equal 20 ms.";

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

    const auto& trace =
        engine.getCanTrace();

    bool previousFound =
        false;

    double previousRequestTimeMs =
        0.0;

    bool periodValid =
        true;

    std::size_t frameCount =
        0;

    constexpr double toleranceMs =
        0.001;

    for (const CanTraceEntry& entry : trace)
    {
        if (
            entry.arbitrationId !=
            CanMessageDefinitions::STEERING_STATE.id
            )
        {
            continue;
        }

        ++frameCount;

        if (previousFound)
        {
            const double periodMs =
                entry.requestTimeMs -
                previousRequestTimeMs;

            if (
                std::fabs(
                    periodMs -
                    CanMessageDefinitions::STEERING_STATE.periodMs
                ) >
                toleranceMs
                )
            {
                periodValid =
                    false;

                break;
            }
        }

        previousRequestTimeMs =
            entry.requestTimeMs;

        previousFound =
            true;
    }

    test.result.executionTimeMs =
        engine.getCurrentTimeMs();

    test.result.expected =
        "Steering request period = 20.000 ms";

    test.result.actual =
        "Steering frames checked = " +
        std::to_string(
            frameCount
        );

    if (
        frameCount >= 2 &&
        periodValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "STEERING_STATE periodicity matched the "
            "configured 20 ms period.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "STEERING_STATE request periodicity did not "
            "match the configured 20 ms period.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanBusUtilizationTest()
{
    TestCase test;

    test.id =
        "TC_CAN_008";

    test.name =
        "CAN Bus Utilization";

    test.description =
        "Verify that CAN bus utilization is calculated "
        "from accumulated transmission time and simulation time.";

    test.requirementId =
        "CAN-REQ-018";

    test.precondition =
        "Simulation shall transmit CAN frames.";

    test.stimulus =
        "Run the simulation for 1000 ms.";

    test.expectedResult =
        "Bus utilization shall be greater than 0 and less than 100 percent.";

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
        1000.0
    );

    const double simulationTimeMs =
        engine.getCurrentTimeMs();

    const double utilizationPercent =
        engine
        .getCanStatistics()
        .getBusUtilization(
            simulationTimeMs
        );

    test.result.executionTimeMs =
        simulationTimeMs;

    test.result.expected =
        "0% < utilization < 100%";

    test.result.actual =
        "Utilization = " +
        std::to_string(
            utilizationPercent
        ) +
        "%";

    if (
        utilizationPercent > 0.0 &&
        utilizationPercent < 100.0
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN bus utilization was calculated and "
            "is within the expected physical range.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN bus utilization was outside the "
            "expected range.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanTraceTimingEvidenceTest()
{
    TestCase test;

    test.id =
        "TC_CAN_009";

    test.name =
        "CAN Trace Timing Evidence";

    test.description =
        "Verify that CAN trace entries record request, "
        "start, end, waiting, and transmission timing evidence.";

    test.requirementId =
        "CAN-REQ-009";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Queue one CAN frame at 5 ms and transmit it at 7 ms.";

    test.expectedResult =
        "Trace timing fields shall contain consistent values.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    frame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    constexpr double requestTimeMs =
        5.0;

    constexpr double txStartTimeMs =
        7.0;

    bus.transmit(
        frame,
        requestTimeMs
    );

    bus.receive(
        txStartTimeMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "request=5 ms, start=7 ms, wait=2 ms, end>start, txTime>0";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace evidence was generated.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    const bool requestValid =
        std::fabs(
            entry.requestTimeMs -
            requestTimeMs
        ) <
        0.001;

    const bool startValid =
        std::fabs(
            entry.txStartTimeMs -
            txStartTimeMs
        ) <
        0.001;

    const bool waitValid =
        std::fabs(
            entry.waitingTimeMs -
            2.0
        ) <
        0.001;

    const bool transmissionValid =
        entry.transmissionTimeMs >
        0.0;

    const bool endValid =
        entry.txEndTimeMs >
        entry.txStartTimeMs;

    const bool timingConsistent =
        std::fabs(
            entry.txEndTimeMs -
            (
                entry.txStartTimeMs +
                entry.transmissionTimeMs
                )
        ) <
        0.001;

    test.result.actual =
        "Request=" +
        std::to_string(
            entry.requestTimeMs
        ) +
        ", Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        ", End=" +
        std::to_string(
            entry.txEndTimeMs
        ) +
        ", Wait=" +
        std::to_string(
            entry.waitingTimeMs
        ) +
        ", TX=" +
        std::to_string(
            entry.transmissionTimeMs
        ) +
        " ms";

    if (
        requestValid &&
        startValid &&
        waitValid &&
        transmissionValid &&
        endValid &&
        timingConsistent
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace contains consistent transmission "
            "timing evidence.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace timing evidence is incomplete "
            "or inconsistent.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanTransmissionDurationTest()
{
    TestCase test;

    test.id =
        "TC_CAN_010";

    test.name =
        "CAN Transmission Duration";

    test.description =
        "Verify that the Virtual CAN Bus calculates frame "
        "transmission duration using the configured CAN timing "
        "model and nominal bitrate.";

    test.requirementId =
        "CAN-REQ-006";

    test.precondition =
        "Virtual CAN Bus shall be configured at 500 kbit/s "
        "and contain a valid CAN frame.";

    test.stimulus =
        "Calculate the expected transmission duration using "
        "CanTiming and transmit the same frame through VirtualCanBus.";

    test.expectedResult =
        "The transmission duration recorded in the CAN trace "
        "shall match the duration calculated by CanTiming.";

    test.result.status =
        TestStatus::Running;

    constexpr std::uint32_t bitrate =
        500000;

    VirtualCanBus bus(
        bitrate,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    frame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    const double expectedTransmissionTimeMs =
        CanTiming::calculateTransmissionTimeMs(
            frame,
            bitrate
        );

    bus.transmit(
        frame,
        0.0
    );

    bus.receive(
        0.0
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Trace TX duration = CanTiming calculated duration";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No transmitted CAN frame was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    constexpr double toleranceMs =
        0.000001;

    const bool durationValid =
        expectedTransmissionTimeMs >
        0.0 &&
        std::fabs(
            entry.transmissionTimeMs -
            expectedTransmissionTimeMs
        ) <=
        toleranceMs;

    const bool endTimeConsistent =
        std::fabs(
            entry.txEndTimeMs -
            (
                entry.txStartTimeMs +
                entry.transmissionTimeMs
            )
        ) <=
        toleranceMs;

    test.result.actual =
        "Expected TX=" +
        std::to_string(
            expectedTransmissionTimeMs
        ) +
        " ms, Trace TX=" +
        std::to_string(
            entry.transmissionTimeMs
        ) +
        " ms, Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        " ms, End=" +
        std::to_string(
            entry.txEndTimeMs
        ) +
        " ms";

    if (
        durationValid &&
        endTimeConsistent
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN transmission duration matches the configured "
            "CAN timing model and bitrate.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "Recorded CAN transmission duration does not match "
            "the expected timing-model result.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanWaitingTimeTest()
{
    TestCase test;

    test.id =
        "TC_CAN_011";

    test.name =
        "CAN Waiting Time";

    test.description =
        "Verify that the Virtual CAN Bus calculates the time "
        "a CAN frame waits between its transmission request "
        "and actual transmission start.";

    test.requirementId =
        "CAN-REQ-007";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Request one CAN frame at 5 ms and start its "
        "transmission at 7 ms.";

    test.expectedResult =
        "Calculated CAN waiting time shall be 2 ms.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    frame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    constexpr double requestTimeMs =
        5.0;

    constexpr double transmissionStartTimeMs =
        7.0;

    constexpr double expectedWaitingTimeMs =
        transmissionStartTimeMs -
        requestTimeMs;

    bus.transmit(
        frame,
        requestTimeMs
    );

    bus.receive(
        transmissionStartTimeMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Waiting time = 2.000 ms";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was available for waiting-time evidence.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    constexpr double toleranceMs =
        0.001;

    const bool requestValid =
        std::fabs(
            entry.requestTimeMs -
            requestTimeMs
        ) <=
        toleranceMs;

    const bool startValid =
        std::fabs(
            entry.txStartTimeMs -
            transmissionStartTimeMs
        ) <=
        toleranceMs;

    const bool waitingTimeValid =
        std::fabs(
            entry.waitingTimeMs -
            expectedWaitingTimeMs
        ) <=
        toleranceMs;

    test.result.responseTimeMs =
        entry.waitingTimeMs;

    test.result.actual =
        "Request=" +
        std::to_string(
            entry.requestTimeMs
        ) +
        " ms, Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        " ms, Waiting=" +
        std::to_string(
            entry.waitingTimeMs
        ) +
        " ms";

    if (
        requestValid &&
        startValid &&
        waitingTimeValid
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN waiting time was calculated correctly "
            "from request time to transmission start.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN waiting-time calculation did not match "
            "the expected 2 ms delay.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanFrameCountTest()
{
    TestCase test;
    test.id = "TC_CAN_012";
    test.name = "CAN Frame Count";
    test.description =
        "Verify that CAN statistics count each successfully transmitted frame.";
    test.requirementId = "CAN-REQ-013";
    test.precondition =
        "Virtual CAN Bus statistics shall start with zero transmitted frames.";
    test.stimulus =
        "Transmit three CAN frames through the Virtual CAN Bus.";
    test.expectedResult =
        "CAN statistics shall report exactly three transmitted frames.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame frame1{};
    frame1.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    frame1.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame frame2{};
    frame2.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame2.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame frame3{};
    frame3.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    frame3.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(frame1, 0.0);
    bus.receive(0.0);

    bus.transmit(frame2, 1.0);
    bus.receive(1.0);

    bus.transmit(frame3, 2.0);
    bus.receive(2.0);

    const auto frameCount =
        bus.getStatistics().getFramesTransmitted();

    test.result.expected = "Frames transmitted = 3";
    test.result.actual =
        "Frames transmitted = " + std::to_string(frameCount);

    if (frameCount == 3)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN statistics counted all transmitted frames correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN frame count did not match the number of transmitted frames.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanAverageWaitingTimeTest()
{
    TestCase test;
    test.id = "TC_CAN_013";
    test.name = "CAN Average Waiting Time";
    test.description =
        "Verify that CAN statistics calculate the average waiting time across transmitted frames.";
    test.requirementId = "CAN-REQ-015";
    test.precondition =
        "Virtual CAN Bus shall be empty with zero accumulated waiting time.";
    test.stimulus =
        "Transmit two frames with known waiting times of 2 ms and 4 ms.";
    test.expectedResult =
        "Average waiting time shall be 3 ms.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame frameA{};
    frameA.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frameA.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame frameB{};
    frameB.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    frameB.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(frameA, 5.0);
    bus.receive(7.0);

    bus.transmit(frameB, 10.0);
    bus.receive(14.0);

    const double actualAverageMs =
        bus.getStatistics().getAverageWaitingTimeMs();

    constexpr double expectedAverageMs = 3.0;
    constexpr double toleranceMs = 0.001;

    test.result.expected = "Average waiting time = 3.000 ms";
    test.result.actual =
        "Average waiting time = " + std::to_string(actualAverageMs) + " ms";

    if (std::fabs(actualAverageMs - expectedAverageMs) <= toleranceMs)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN average waiting time was calculated correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN average waiting time did not match the expected value.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanMaximumWaitingTimeTest()
{
    TestCase test;
    test.id = "TC_CAN_014";
    test.name = "CAN Maximum Waiting Time";
    test.description =
        "Verify that CAN statistics retain the maximum waiting time observed across transmitted frames.";
    test.requirementId = "CAN-REQ-016";
    test.precondition =
        "Virtual CAN Bus statistics shall start with zero maximum waiting time.";
    test.stimulus =
        "Transmit three frames with waiting times of 1 ms, 5 ms, and 3 ms.";
    test.expectedResult =
        "Maximum waiting time shall be 5 ms.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame frame{};
    frame.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    bus.transmit(frame, 1.0);
    bus.receive(2.0);

    bus.transmit(frame, 10.0);
    bus.receive(15.0);

    bus.transmit(frame, 20.0);
    bus.receive(23.0);

    const double actualMaximumMs =
        bus.getStatistics().getMaximumWaitingTimeMs();

    constexpr double expectedMaximumMs = 5.0;
    constexpr double toleranceMs = 0.001;

    test.result.expected = "Maximum waiting time = 5.000 ms";
    test.result.actual =
        "Maximum waiting time = " + std::to_string(actualMaximumMs) + " ms";

    if (std::fabs(actualMaximumMs - expectedMaximumMs) <= toleranceMs)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN maximum waiting time was recorded correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN maximum waiting time did not match the expected value.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanTotalTransmissionTimeTest()
{
    TestCase test;
    test.id = "TC_CAN_015";
    test.name = "CAN Total Transmission Time";
    test.description =
        "Verify that CAN statistics accumulate the total transmission time of all transmitted frames.";
    test.requirementId = "CAN-REQ-017";
    test.precondition =
        "Virtual CAN Bus total transmission time shall initially be zero.";
    test.stimulus =
        "Transmit three frames and compare accumulated transmission time against the sum of CanTiming results.";
    test.expectedResult =
        "Total transmission time shall equal the sum of individual CAN frame transmission durations.";
    test.result.status = TestStatus::Running;

    constexpr std::uint32_t bitrate = 500000;
    VirtualCanBus bus(bitrate, LogLevel::Quiet);

    CanFrame frame1{};
    frame1.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    frame1.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame frame2{};
    frame2.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame2.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame frame3{};
    frame3.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    frame3.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    const double expectedTotalMs =
        CanTiming::calculateTransmissionTimeMs(frame1, bitrate) +
        CanTiming::calculateTransmissionTimeMs(frame2, bitrate) +
        CanTiming::calculateTransmissionTimeMs(frame3, bitrate);

    bus.transmit(frame1, 0.0);
    bus.receive(0.0);

    bus.transmit(frame2, 1.0);
    bus.receive(1.0);

    bus.transmit(frame3, 2.0);
    bus.receive(2.0);

    const double actualTotalMs =
        bus.getStatistics().getTotalTransmissionTimeMs();

    constexpr double toleranceMs = 0.000001;

    test.result.expected =
        "Total TX time = " + std::to_string(expectedTotalMs) + " ms";
    test.result.actual =
        "Total TX time = " + std::to_string(actualTotalMs) + " ms";

    if (std::fabs(actualTotalMs - expectedTotalMs) <= toleranceMs)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN total transmission time accumulated correctly.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN total transmission time did not match the sum of individual frame durations.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanPerMessageStatisticsTest()
{
    TestCase test;
    test.id = "TC_CAN_016";
    test.name = "CAN Per-Message Statistics";
    test.description =
        "Verify that CAN statistics maintain separate statistics for each CAN message identifier.";
    test.requirementId = "CAN-REQ-019";
    test.precondition =
        "Virtual CAN Bus message statistics shall initially contain no frames.";
    test.stimulus =
        "Transmit two POWERTRAIN_STATE frames and one ABS_WHEEL_STATE frame.";
    test.expectedResult =
        "Per-message statistics shall report Powertrain=2 frames and ABS=1 frame.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame absFrame{};
    absFrame.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    bus.transmit(powertrainFrame, 0.0);
    bus.receive(0.0);

    bus.transmit(absFrame, 1.0);
    bus.receive(1.0);

    bus.transmit(powertrainFrame, 2.0);
    bus.receive(2.0);

    const auto& stats =
        bus.getStatistics().getMessageStatistics();

    const auto ptIt =
        stats.find(CanMessageDefinitions::POWERTRAIN_STATE.id);
    const auto absIt =
        stats.find(CanMessageDefinitions::ABS_WHEEL_STATE.id);

    const bool ptFound = ptIt != stats.end();
    const bool absFound = absIt != stats.end();

    std::uint64_t ptFrames = 0;
    std::uint64_t absFrames = 0;

    if (ptFound)
    {
        ptFrames = ptIt->second.frameCount;
    }

    if (absFound)
    {
        absFrames = absIt->second.frameCount;
    }

    test.result.expected = "0x100 frames=2, 0x080 frames=1";
    test.result.actual =
        "0x100 frames=" + std::to_string(ptFrames) +
        ", 0x080 frames=" + std::to_string(absFrames);

    if (ptFound && absFound && ptFrames == 2 && absFrames == 1)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "CAN per-message frame statistics were separated and counted correctly by arbitration ID.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "CAN per-message statistics did not contain the expected frame counts.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanPendingFrameBehaviorTest()
{
    TestCase test;

    test.id =
        "TC_CAN_017";

    test.name =
        "CAN Pending Frame Behavior";

    test.description =
        "Verify that a non-winning CAN frame remains pending "
        "after arbitration and can be transmitted afterward.";

    test.requirementId =
        "CAN-REQ-005";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Queue two frames at the same request time with IDs "
        "0x100 and 0x080, then receive twice.";

    test.expectedResult =
        "0x080 shall transmit first and 0x100 shall remain "
        "pending until the second receive operation.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame absFrame{};
    absFrame.arbitrationId =
        CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc =
        CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    constexpr double requestTimeMs =
        0.0;

    bus.transmit(
        powertrainFrame,
        requestTimeMs
    );

    bus.transmit(
        absFrame,
        requestTimeMs
    );

    const bool pendingBeforeReceive =
        bus.hasFrame();

    const CanFrame firstFrame =
        bus.receive(
            0.0
        );

    const bool pendingAfterFirstReceive =
        bus.hasFrame();

    const CanFrame secondFrame =
        bus.receive(
            bus.getBusyUntilMs()
        );

    const bool pendingAfterSecondReceive =
        bus.hasFrame();

    const bool firstIsAbs =
        firstFrame.arbitrationId ==
        CanMessageDefinitions::ABS_WHEEL_STATE.id;

    const bool secondIsPowertrain =
        secondFrame.arbitrationId ==
        CanMessageDefinitions::POWERTRAIN_STATE.id;

    test.result.expected =
        "Before=true, First=0x080, PendingAfterFirst=true, "
        "Second=0x100, PendingAfterSecond=false";

    test.result.actual =
        "Before=" +
        std::string(
            pendingBeforeReceive
            ? "true"
            : "false"
        ) +
        ", First=" +
        std::string(
            firstIsAbs
            ? "0x080"
            : "unexpected"
        ) +
        ", PendingAfterFirst=" +
        std::string(
            pendingAfterFirstReceive
            ? "true"
            : "false"
        ) +
        ", Second=" +
        std::string(
            secondIsPowertrain
            ? "0x100"
            : "unexpected"
        ) +
        ", PendingAfterSecond=" +
        std::string(
            pendingAfterSecondReceive
            ? "true"
            : "false"
        );

    if (
        pendingBeforeReceive &&
        firstIsAbs &&
        pendingAfterFirstReceive &&
        secondIsPowertrain &&
        !pendingAfterSecondReceive
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "Non-winning CAN frame remained pending and "
            "was transmitted on the next receive operation.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN pending-frame behavior did not match "
            "the requirement.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanWaitingTimeTraceEvidenceTest()
{
    TestCase test;

    test.id =
        "TC_CAN_018";

    test.name =
        "CAN Waiting-Time Trace Evidence";

    test.description =
        "Verify that CAN trace entries preserve the calculated "
        "waiting-time evidence for transmitted frames.";

    test.requirementId =
        "CAN-REQ-012";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Request a CAN frame at 4 ms and transmit it at 9 ms.";

    test.expectedResult =
        "The CAN trace shall record a waiting time of 5 ms.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};
    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;
    frame.dlc =
        CanMessageDefinitions::STEERING_STATE.dlc;

    constexpr double requestTimeMs =
        4.0;

    constexpr double txStartTimeMs =
        9.0;

    constexpr double expectedWaitingTimeMs =
        5.0;

    bus.transmit(
        frame,
        requestTimeMs
    );

    bus.receive(
        txStartTimeMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Trace waiting time = 5.000 ms";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    constexpr double toleranceMs =
        0.001;

    const bool waitingTimeMatches =
        std::fabs(
            entry.waitingTimeMs -
            expectedWaitingTimeMs
        ) <=
        toleranceMs;

    const bool requestTimeMatches =
        std::fabs(
            entry.requestTimeMs -
            requestTimeMs
        ) <=
        toleranceMs;

    const bool txStartMatches =
        std::fabs(
            entry.txStartTimeMs -
            txStartTimeMs
        ) <=
        toleranceMs;

    test.result.responseTimeMs =
        entry.waitingTimeMs;

    test.result.actual =
        "Request=" +
        std::to_string(
            entry.requestTimeMs
        ) +
        " ms, Start=" +
        std::to_string(
            entry.txStartTimeMs
        ) +
        " ms, Waiting=" +
        std::to_string(
            entry.waitingTimeMs
        ) +
        " ms";

    if (
        waitingTimeMatches &&
        requestTimeMatches &&
        txStartMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace preserved the expected waiting-time "
            "evidence for the transmitted frame.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace waiting-time evidence did not match "
            "the expected request/start relationship.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanExclusiveBusAccessTest()
{
    TestCase test;

    test.id =
        "TC_CAN_019";

    test.name =
        "CAN Exclusive Bus Access";

    test.description =
        "Verify that two CAN frames do not occupy the virtual "
        "CAN bus at the same time.";

    test.requirementId =
        "CAN-REQ-004";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Queue two CAN frames at the same request time and "
        "transmit them sequentially.";

    test.expectedResult =
        "The second frame transmission shall start at or after "
        "the first frame transmission has completed.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame absFrame{};
    absFrame.arbitrationId =
        CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc =
        CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    bus.transmit(
        powertrainFrame,
        0.0
    );

    bus.transmit(
        absFrame,
        0.0
    );

    bus.receive(
        0.0
    );

    const double secondTransmissionStartMs =
        bus.getBusyUntilMs();

    bus.receive(
        secondTransmissionStartMs
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Second TX start >= First TX end";

    if (trace.size() < 2)
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Fewer than two trace entries";

        test.result.message =
            "The test could not observe two sequential CAN transmissions.";

        return test;
    }

    const CanTraceEntry& first =
        trace[0];

    const CanTraceEntry& second =
        trace[1];

    constexpr double toleranceMs =
        0.000001;

    const bool firstDurationValid =
        first.txEndTimeMs >
        first.txStartTimeMs;

    const bool secondDurationValid =
        second.txEndTimeMs >
        second.txStartTimeMs;

    const bool noOverlap =
        second.txStartTimeMs +
        toleranceMs >=
        first.txEndTimeMs;

    test.result.actual =
        "First=[" +
        std::to_string(
            first.txStartTimeMs
        ) +
        ", " +
        std::to_string(
            first.txEndTimeMs
        ) +
        "] ms, Second=[" +
        std::to_string(
            second.txStartTimeMs
        ) +
        ", " +
        std::to_string(
            second.txEndTimeMs
        ) +
        "] ms";

    if (
        firstDurationValid &&
        secondDurationValid &&
        noOverlap
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN transmissions occupied the bus exclusively "
            "without temporal overlap.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN transmissions overlapped or contained invalid "
            "transmission timing.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanTraceIdentifierTest()
{
    TestCase test;

    test.id =
        "TC_CAN_020";

    test.name =
        "CAN Trace Identifier Evidence";

    test.description =
        "Verify that each CAN trace entry records the arbitration "
        "identifier of the transmitted frame.";

    test.requirementId =
        "CAN-REQ-010";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Transmit a STEERING_STATE frame using CAN ID 0x120.";

    test.expectedResult =
        "The CAN trace entry shall contain arbitration ID 0x120.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};
    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;
    frame.dlc =
        CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(
        frame,
        0.0
    );

    bus.receive(
        0.0
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "Trace CAN ID = 0x120";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    const bool identifierMatches =
        entry.arbitrationId ==
        CanMessageDefinitions::STEERING_STATE.id;

    test.result.actual =
        identifierMatches
        ? "Trace CAN ID = 0x120"
        : "Trace CAN ID unexpected";

    if (identifierMatches)
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace preserved the transmitted arbitration identifier.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace arbitration identifier did not match "
            "the transmitted frame.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanTracePayloadTest()
{
    TestCase test;

    test.id =
        "TC_CAN_021";

    test.name =
        "CAN Trace Payload Evidence";

    test.description =
        "Verify that the CAN trace stores the DLC and payload bytes "
        "of the transmitted CAN frame.";

    test.requirementId =
        "CAN-REQ-011";

    test.precondition =
        "Virtual CAN Bus shall be empty.";

    test.stimulus =
        "Transmit a CAN frame with a known four-byte payload.";

    test.expectedResult =
        "The CAN trace shall contain the same DLC and payload bytes.";

    test.result.status =
        TestStatus::Running;

    VirtualCanBus bus(
        500000,
        LogLevel::Quiet
    );

    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;

    frame.dlc =
        4;

    frame.data[0] =
        0x12;

    frame.data[1] =
        0x34;

    frame.data[2] =
        0x56;

    frame.data[3] =
        0x78;

    bus.transmit(
        frame,
        0.0
    );

    bus.receive(
        0.0
    );

    const auto& trace =
        bus.getTrace();

    test.result.expected =
        "DLC=4, DATA=12 34 56 78";

    if (trace.empty())
    {
        test.result.status =
            TestStatus::Failed;

        test.result.actual =
            "Trace empty";

        test.result.message =
            "No CAN trace entry was recorded.";

        return test;
    }

    const CanTraceEntry& entry =
        trace.front();

    const bool dlcMatches =
        entry.dlc ==
        4;

    const bool payloadMatches =
        entry.data[0] == 0x12 &&
        entry.data[1] == 0x34 &&
        entry.data[2] == 0x56 &&
        entry.data[3] == 0x78;

    test.result.actual =
        "DLC=" +
        std::to_string(
            static_cast<int>(
                entry.dlc
            )
        ) +
        ", DATA=" +
        std::to_string(
            static_cast<int>(
                entry.data[0]
            )
        ) +
        " " +
        std::to_string(
            static_cast<int>(
                entry.data[1]
            )
        ) +
        " " +
        std::to_string(
            static_cast<int>(
                entry.data[2]
            )
        ) +
        " " +
        std::to_string(
            static_cast<int>(
                entry.data[3]
            )
        );

    if (
        dlcMatches &&
        payloadMatches
        )
    {
        test.result.status =
            TestStatus::Passed;

        test.result.message =
            "CAN trace preserved the transmitted DLC and payload bytes.";
    }
    else
    {
        test.result.status =
            TestStatus::Failed;

        test.result.message =
            "CAN trace payload evidence did not match "
            "the transmitted CAN frame.";
    }

    return test;
}

// ==================================================

TestCase TestRunner::runCanMultipleEcuRequestsTest()
{
    TestCase test;

    test.id = "TC_CAN_022";
    test.name = "Multiple ECU Transmission Requests";
    test.description =
        "Verify that the Virtual CAN Bus accepts multiple pending "
        "transmission requests from simulated ECU message sources.";
    test.requirementId = "CAN-REQ-002";
    test.precondition = "Virtual CAN Bus shall be empty.";
    test.stimulus =
        "Queue ABS, Powertrain, and Steering frames at the same request time and transmit all three.";
    test.expectedResult =
        "All three requests shall be accepted and transmitted through the Virtual CAN Bus.";
    test.result.status = TestStatus::Running;

    VirtualCanBus bus(500000, LogLevel::Quiet);

    CanFrame absFrame{};
    absFrame.arbitrationId = CanMessageDefinitions::ABS_WHEEL_STATE.id;
    absFrame.dlc = CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    CanFrame powertrainFrame{};
    powertrainFrame.arbitrationId = CanMessageDefinitions::POWERTRAIN_STATE.id;
    powertrainFrame.dlc = CanMessageDefinitions::POWERTRAIN_STATE.dlc;

    CanFrame steeringFrame{};
    steeringFrame.arbitrationId = CanMessageDefinitions::STEERING_STATE.id;
    steeringFrame.dlc = CanMessageDefinitions::STEERING_STATE.dlc;

    bus.transmit(powertrainFrame, 0.0);
    bus.transmit(steeringFrame, 0.0);
    bus.transmit(absFrame, 0.0);

    std::vector<std::uint32_t> transmittedIds;

    while (bus.hasFrame())
    {
        const CanFrame frame =
            bus.receive(bus.getBusyUntilMs());

        transmittedIds.push_back(frame.arbitrationId);
    }

    bool absSeen = false;
    bool powertrainSeen = false;
    bool steeringSeen = false;

    for (const std::uint32_t id : transmittedIds)
    {
        if (id == CanMessageDefinitions::ABS_WHEEL_STATE.id)
        {
            absSeen = true;
        }
        else if (id == CanMessageDefinitions::POWERTRAIN_STATE.id)
        {
            powertrainSeen = true;
        }
        else if (id == CanMessageDefinitions::STEERING_STATE.id)
        {
            steeringSeen = true;
        }
    }

    test.result.expected =
        "3 transmissions: 0x080, 0x100, 0x120";
    test.result.actual =
        "Count=" + std::to_string(transmittedIds.size()) +
        ", 0x080=" + std::string(absSeen ? "true" : "false") +
        ", 0x100=" + std::string(powertrainSeen ? "true" : "false") +
        ", 0x120=" + std::string(steeringSeen ? "true" : "false");

    if (transmittedIds.size() == 3 &&
        absSeen &&
        powertrainSeen &&
        steeringSeen)
    {
        test.result.status = TestStatus::Passed;
        test.result.message =
            "Virtual CAN Bus accepted and transmitted multiple ECU requests.";
    }
    else
    {
        test.result.status = TestStatus::Failed;
        test.result.message =
            "Virtual CAN Bus did not process all expected ECU transmission requests.";
    }

    return test;
}

