#include "SimulationEngine.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "../can/CanMessageDefinitions.h"
#include "../can/CanTracePrinter.h"

// ==================================================
// Constructor
// ==================================================

SimulationEngine::SimulationEngine(
    LogLevel logLevel)
    : logLevel(logLevel),
    canBus(
        canBitrate,
        logLevel
    )
{
    vehicleModel.setSteeringAngle(
        3.2f
    );

    steeringState.steeringAngleDeg =
        vehicleModel.getSteeringAngleDeg();

    steeringState.steeringTorqueNm =
        1.45f;

    steeringEcu.setSteeringState(
        steeringState
    );
}

// ==================================================
// Interactive simulation update
// ==================================================

void SimulationEngine::update(
    double deltaTimeMs)
{
    if (
        deltaTimeMs <=
        0.0
        )
    {
        return;
    }

    const double targetTimeMs =
        currentTimeMs +
        deltaTimeMs;

    // ==================================================
    // Apply current scenario
    // ==================================================

    scenarioController.update(
        vehicleModel
    );

    updateBrakeEvents();

    // ==================================================
    // Initial ECU events at t = 0
    // ==================================================

    if (
        currentTimeMs == 0.0 &&
        nextAbsTxMs == 0.0 &&
        nextPowertrainTxMs == 0.0 &&
        nextSteeringTxMs == 0.0
        )
    {
        updateSensors();

        processAbsEcu(
            0.0
        );

        processPowertrainEcu(
            0.0
        );

        processSteeringEcu(
            0.0
        );

        nextAbsTxMs =
            CanMessageDefinitions::
            ABS_WHEEL_STATE.periodMs;

        nextPowertrainTxMs =
            CanMessageDefinitions::
            POWERTRAIN_STATE.periodMs;

        nextSteeringTxMs =
            CanMessageDefinitions::
            STEERING_STATE.periodMs;

        processCanBus(
            0.0
        );
    }

    // ==================================================
    // Process internal events until target time
    // ==================================================

    while (
        currentTimeMs <
        targetTimeMs
        )
    {
        const double nextEventTimeMs =
            getNextInternalEventTime(
                targetTimeMs
            );

        // --------------------------------------------------
        // Advance vehicle physics
        // --------------------------------------------------

        advancePhysicsTo(
            nextEventTimeMs
        );

        // --------------------------------------------------
        // Update physical state
        // --------------------------------------------------

        updateVehicleEvents();

        updateSensors();

        // --------------------------------------------------
        // ABS periodic task
        // --------------------------------------------------

        if (
            nextAbsTxMs <=
            currentTimeMs
            )
        {
            processAbsEcu(
                nextAbsTxMs
            );

            nextAbsTxMs +=
                CanMessageDefinitions::
                ABS_WHEEL_STATE.periodMs;
        }

        // --------------------------------------------------
        // Powertrain periodic task
        // --------------------------------------------------

        if (
            nextPowertrainTxMs <=
            currentTimeMs
            )
        {
            processPowertrainEcu(
                nextPowertrainTxMs
            );

            nextPowertrainTxMs +=
                CanMessageDefinitions::
                POWERTRAIN_STATE.periodMs;
        }

        // --------------------------------------------------
        // Steering periodic task
        // --------------------------------------------------

        if (
            nextSteeringTxMs <=
            currentTimeMs
            )
        {
            processSteeringEcu(
                nextSteeringTxMs
            );

            nextSteeringTxMs +=
                CanMessageDefinitions::
                STEERING_STATE.periodMs;
        }

        // --------------------------------------------------
        // CAN processing
        // --------------------------------------------------

        processCanBus(
            currentTimeMs
        );

        if (
            currentTimeMs >=
            targetTimeMs
            )
        {
            break;
        }
    }
}

// ==================================================
// Scenario control
// ==================================================

void SimulationEngine::startScenario(
    DrivingScenario scenario)
{
    scenarioController.startScenario(
        scenario,
        currentTimeMs,
        eventLogger
    );
}

// ==================================================
// Fault injection
// ==================================================

void SimulationEngine::setFrontLeftWheelSensorFault(
    SensorFault fault)
{
    frontLeftWheelSensor.setFault(
        fault
    );

    eventLogger.log(
        currentTimeMs,
        "FAULT",
        "Front Left Wheel Speed Sensor fault injected"
    );
}

void SimulationEngine::setFrontRightWheelSensorFault(
    SensorFault fault)
{
    frontRightWheelSensor.setFault(
        fault
    );

    eventLogger.log(
        currentTimeMs,
        "FAULT",
        "Front Right Wheel Speed Sensor fault injected"
    );
}

void SimulationEngine::clearFrontLeftWheelSensorFault()
{
    frontLeftWheelSensor.clearFault();

    eventLogger.log(
        currentTimeMs,
        "FAULT",
        "Front Left Wheel Speed Sensor fault cleared"
    );
}

void SimulationEngine::clearFrontRightWheelSensorFault()
{
    frontRightWheelSensor.clearFault();

    eventLogger.log(
        currentTimeMs,
        "FAULT",
        "Front Right Wheel Speed Sensor fault cleared"
    );
}

// ==================================================
// Physics
// ==================================================

void SimulationEngine::advancePhysicsTo(
    double newTimeMs)
{
    if (
        newTimeMs <=
        currentTimeMs
        )
    {
        return;
    }

    const double deltaTimeMs =
        newTimeMs -
        currentTimeMs;

    vehicleModel.update(
        deltaTimeMs
    );

    currentTimeMs =
        newTimeMs;
}

// ==================================================
// Determine next internal event
// ==================================================

double
SimulationEngine::getNextInternalEventTime(
    double targetTimeMs) const
{
    double nextTimeMs =
        targetTimeMs;

    if (
        nextAbsTxMs >
        currentTimeMs &&
        nextAbsTxMs <
        nextTimeMs
        )
    {
        nextTimeMs =
            nextAbsTxMs;
    }

    if (
        nextPowertrainTxMs >
        currentTimeMs &&
        nextPowertrainTxMs <
        nextTimeMs
        )
    {
        nextTimeMs =
            nextPowertrainTxMs;
    }

    if (
        nextSteeringTxMs >
        currentTimeMs &&
        nextSteeringTxMs <
        nextTimeMs
        )
    {
        nextTimeMs =
            nextSteeringTxMs;
    }

    return nextTimeMs;
}

// ==================================================
// Brake event detection
// ==================================================

void SimulationEngine::updateBrakeEvents()
{
    const bool currentBrakeApplied =
        vehicleModel.getBrakePercent() >
        0.0f;

    if (
        currentBrakeApplied &&
        !previousBrakeApplied
        )
    {
        eventLogger.log(
            currentTimeMs,
            "BRAKE",
            "Brake applied"
        );
    }
    else if (
        !currentBrakeApplied &&
        previousBrakeApplied
        )
    {
        eventLogger.log(
            currentTimeMs,
            "BRAKE",
            "Brake released"
        );
    }

    previousBrakeApplied =
        currentBrakeApplied;
}

// ==================================================
// Vehicle event detection
// ==================================================

void SimulationEngine::updateVehicleEvents()
{
    const float speed =
        vehicleModel.getVehicleSpeedKmh();

    if (
        speed >
        0.5f
        )
    {
        vehicleWasMoving =
            true;
    }

    if (
        vehicleWasMoving &&
        speed <=
        0.5f
        )
    {
        eventLogger.log(
            currentTimeMs,
            "VEHICLE",
            "Vehicle stopped"
        );

        vehicleWasMoving =
            false;
    }
}

// ==================================================
// Sensor update
// ==================================================

void SimulationEngine::updateSensors()
{
    frontLeftWheelSensor.setTrueSpeed(
        vehicleModel
        .getFrontLeftWheelSpeedKmh()
    );

    frontRightWheelSensor.setTrueSpeed(
        vehicleModel
        .getFrontRightWheelSpeedKmh()
    );
}

// ==================================================
// ABS ECU
// ==================================================

void SimulationEngine::processAbsEcu(
    double eventTimeMs)
{
    const WheelSpeedReading frontLeftReading =
        frontLeftWheelSensor.readSpeed();

    const WheelSpeedReading frontRightReading =
        frontRightWheelSensor.readSpeed();

    absEcu.updateSensors(
        frontLeftReading,
        frontRightReading
    );

    // ==================================================
    // ABS health transition detection
    // ==================================================

    const AbsHealthStatus currentAbsHealthStatus =
        absEcu
        .getState()
        .healthStatus;

    if (
        currentAbsHealthStatus ==
        AbsHealthStatus::Degraded &&
        previousAbsHealthStatus ==
        AbsHealthStatus::Healthy
        )
    {
        eventLogger.log(
            eventTimeMs,
            "ABS",
            "ABS HEALTH DEGRADED"
        );
    }
    else if (
        currentAbsHealthStatus ==
        AbsHealthStatus::Healthy &&
        previousAbsHealthStatus ==
        AbsHealthStatus::Degraded
        )
    {
        eventLogger.log(
            eventTimeMs,
            "ABS",
            "ABS HEALTH RESTORED"
        );
    }

    previousAbsHealthStatus =
        currentAbsHealthStatus;

    // ==================================================
    // ABS active transition
    // ==================================================

    const bool currentAbsActive =
        absEcu
        .getState()
        .absActive;

    if (
        currentAbsActive &&
        !previousAbsActive
        )
    {
        eventLogger.log(
            eventTimeMs,
            "ABS",
            "ABS ACTIVATED - wheel slip detected"
        );
    }
    else if (
        !currentAbsActive &&
        previousAbsActive
        )
    {
        eventLogger.log(
            eventTimeMs,
            "ABS",
            "ABS DEACTIVATED - wheel slip cleared"
        );
    }

    previousAbsActive =
        currentAbsActive;

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout
            << "["
            << eventTimeMs
            << " ms] ABS ECU request -> 0x"
            << std::hex
            << CanMessageDefinitions::
            ABS_WHEEL_STATE.id
            << std::dec
            << "\n";
    }

    absEcu.transmitAbsState(
        canBus,
        eventTimeMs
    );
}

// ==================================================
// Powertrain ECU
// ==================================================

void SimulationEngine::processPowertrainEcu(
    double eventTimeMs)
{
    vehicleState.speedKmh =
        vehicleModel
        .getVehicleSpeedKmh();

    vehicleState.engineRpm =
        static_cast<std::uint16_t>(
            800.0f +
            vehicleState.speedKmh *
            40.0f
            );

    vehicleState.gear =
        vehicleState.speedKmh < 20.0f
        ? 1
        : vehicleState.speedKmh < 40.0f
        ? 2
        : vehicleState.speedKmh < 70.0f
        ? 3
        : 4;

    vehicleState.brakeApplied =
        vehicleModel
        .getBrakePercent() >
        0.0f;

    powertrainEcu.setVehicleState(
        vehicleState
    );

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout
            << "["
            << eventTimeMs
            << " ms] POWERTRAIN ECU request -> 0x"
            << std::hex
            << CanMessageDefinitions::
            POWERTRAIN_STATE.id
            << std::dec
            << "\n";
    }

    powertrainEcu.transmitVehicleState(
        canBus,
        eventTimeMs
    );
}

// ==================================================
// Steering ECU
// ==================================================

void SimulationEngine::processSteeringEcu(
    double eventTimeMs)
{
    steeringState.steeringAngleDeg =
        vehicleModel
        .getSteeringAngleDeg();

    steeringEcu.setSteeringState(
        steeringState
    );

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout
            << "["
            << eventTimeMs
            << " ms] STEERING ECU request -> 0x"
            << std::hex
            << CanMessageDefinitions::
            STEERING_STATE.id
            << std::dec
            << "\n";
    }

    steeringEcu.transmitSteeringState(
        canBus,
        eventTimeMs
    );
}

// ==================================================
// CAN bus
// ==================================================

void SimulationEngine::processCanBus(
    double eventTimeMs)
{
    if (!canBus.hasFrame())
    {
        return;
    }

    double busTimeMs =
        std::max(
            eventTimeMs,
            canBus.getBusyUntilMs()
        );

    while (
        canBus.hasFrame()
        )
    {
        const CanFrame frame =
            canBus.receive(
                busTimeMs
            );

        dashboardEcu.receiveFrame(
            frame
        );

        busTimeMs =
            canBus.getBusyUntilMs();
    }
}

// ==================================================
// Reset
// ==================================================

void SimulationEngine::reset()
{
    currentTimeMs =
        0.0;

    nextAbsTxMs =
        0.0;

    nextPowertrainTxMs =
        0.0;

    nextSteeringTxMs =
        0.0;

    previousAbsActive =
        false;

    previousBrakeApplied =
        false;

    vehicleWasMoving =
        false;

    previousAbsHealthStatus =
        AbsHealthStatus::Healthy;

    vehicleState =
        VehicleState{};

    steeringState =
        SteeringState{};

    vehicleModel =
        VehicleModel{};

    scenarioController =
        ScenarioController{};

    eventLogger =
        EventLogger{};

    frontLeftWheelSensor =
        WheelSpeedSensor{};

    frontRightWheelSensor =
        WheelSpeedSensor{};

    canBus =
        VirtualCanBus(
            canBitrate,
            logLevel
        );

    vehicleModel.setSteeringAngle(
        3.2f
    );

    steeringState.steeringAngleDeg =
        3.2f;

    steeringState.steeringTorqueNm =
        1.45f;

    steeringEcu.setSteeringState(
        steeringState
    );
}

// ==================================================
// Demo
// ==================================================

void SimulationEngine::runDemo()
{
    reset();

    constexpr double demoStepMs =
        10.0;

    constexpr double demoDurationMs =
        5000.0;

    startScenario(
        DrivingScenario::Acceleration
    );

    bool cruiseStarted =
        false;

    bool hardBrakingStarted =
        false;

    bool recoveryStarted =
        false;

    while (
        currentTimeMs <
        demoDurationMs
        )
    {
        if (
            !cruiseStarted &&
            currentTimeMs >=
            2000.0
            )
        {
            startScenario(
                DrivingScenario::Cruise
            );

            cruiseStarted =
                true;
        }

        if (
            !hardBrakingStarted &&
            currentTimeMs >=
            3000.0
            )
        {
            startScenario(
                DrivingScenario::HardBraking
            );

            hardBrakingStarted =
                true;
        }

        if (
            !recoveryStarted &&
            currentTimeMs >=
            4500.0
            )
        {
            startScenario(
                DrivingScenario::Recovery
            );

            recoveryStarted =
                true;
        }

        update(
            demoStepMs
        );
    }
}

// ==================================================
// Output
// ==================================================

void SimulationEngine::printResults() const
{
    std::cout << "\n";

    dashboardEcu
        .displayVehicleState();

    if (
        logLevel ==
        LogLevel::Events ||
        logLevel ==
        LogLevel::Verbose
        )
    {
        eventLogger.printLog();
    }

    CanTracePrinter::printLast(
        canBus.getTrace(),
        15
    );

    canBus.printStatistics(
        currentTimeMs
    );
}

// ==================================================
// Getters
// ==================================================

double
SimulationEngine::getCurrentTimeMs() const
{
    return currentTimeMs;
}

const VehicleModel&
SimulationEngine::getVehicleModel() const
{
    return vehicleModel;
}

const VehicleState&
SimulationEngine::getVehicleState() const
{
    return vehicleState;
}

const SteeringState&
SimulationEngine::getSteeringState() const
{
    return steeringState;
}

const AbsState&
SimulationEngine::getAbsState() const
{
    return absEcu.getState();
}

const std::vector<VehicleEvent>&
SimulationEngine::getEvents() const
{
    return eventLogger.getEvents();
}

const std::vector<CanTraceEntry>&
SimulationEngine::getCanTrace() const
{
    return canBus.getTrace();
}

// ==================================================
// CAN Statistics
// ==================================================

const CanStatistics&
SimulationEngine::getCanStatistics() const
{
    return canBus.getStatistics();
}

std::uint32_t
SimulationEngine::getCanBitrate() const
{
    return canBus.getBitrate();
}