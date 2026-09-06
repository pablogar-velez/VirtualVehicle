#include "SimulationEngine.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../can/CanMessageDefinitions.h"
#include "../can/CanTracePrinter.h"

#include "../diagnostics/DtcDefinitions.h"

// ==================================================
// Constructor
// ==================================================

SimulationEngine::SimulationEngine(
    LogLevel logLevel)
    : logLevel(logLevel),
    canBus(
        canBitrate,
        logLevel
    ),
    ethernetBus(
        ethernetLinkRate
    ),
    ethernetNodeA(
        MacAddress{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 }
    ),
    ethernetNodeB(
        MacAddress{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x02 }
    ),
    udsServer(dtcManager),
    udsTransport(udsServer)
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

    initializeEthernetRuntime();

    updateUdsVehicleData();
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
    // Automatic drive cycle
    // ==================================================

    updateAutomaticDriveCycle();

    // ==================================================
    // Apply vehicle controls
    //
    // Manual scenarios keep their original behavior.
    // Automatic driving uses a separate bounded control
    // loop so verification scenarios are not modified.
    // ==================================================

    if (automaticDriveCycleEnabled)
    {
        applyAutomaticDriveControl();
    }
    else
    {
        applyManualDriveControl();
    }

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

        updateUdsVehicleData();

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

        processEthernetBus();
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
        // Update UDS runtime data
        // --------------------------------------------------

        updateUdsVehicleData();

        // --------------------------------------------------
        // CAN processing
        // --------------------------------------------------

        processCanBus(
            currentTimeMs
        );

        processEthernetBus();

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
    // A direct user-selected scenario becomes a manual
    // override and stops the automatic drive cycle.
    automaticDriveCycleEnabled =
        false;

    if (
        scenario ==
        DrivingScenario::Cruise
        )
    {
        // Hold approximately the current road speed.
        // Clamp an abnormally high speed so Cruise can
        // also recover from a previous runaway state.
        manualCruiseTargetKmh =
            std::clamp(
                vehicleModel.getVehicleSpeedKmh(),
                20.0f,
                80.0f
            );
    }

    scenarioController.startScenario(
        scenario,
        currentTimeMs,
        eventLogger
    );
}

// ==================================================
// Automatic drive cycle
// ==================================================

void SimulationEngine::startAutomaticDriveCycle()
{
    automaticDriveCycleEnabled =
        true;

    setAutomaticDrivePhase(
        AutomaticDrivePhase::Acceleration
    );

    eventLogger.log(
        currentTimeMs,
        "DRIVE",
        "Automatic drive cycle started"
    );
}

void SimulationEngine::stopAutomaticDriveCycle()
{
    automaticDriveCycleEnabled =
        false;
}

bool SimulationEngine::isAutomaticDriveCycleEnabled() const
{
    return automaticDriveCycleEnabled;
}

DrivingScenario SimulationEngine::getCurrentScenario() const
{
    return scenarioController.getCurrentScenario();
}

void SimulationEngine::setAutomaticDrivePhase(
    AutomaticDrivePhase phase)
{
    if (
        automaticDrivePhase ==
        phase
        )
    {
        automaticPhaseStartTimeMs =
            currentTimeMs;

        return;
    }

    automaticDrivePhase =
        phase;

    automaticPhaseStartTimeMs =
        currentTimeMs;

    switch (automaticDrivePhase)
    {
    case AutomaticDrivePhase::Acceleration:

        eventLogger.log(
            currentTimeMs,
            "DRIVE",
            "Acceleration phase"
        );

        break;

    case AutomaticDrivePhase::HighCruise:

        eventLogger.log(
            currentTimeMs,
            "DRIVE",
            "High-speed cruise phase"
        );

        break;

    case AutomaticDrivePhase::GentleDeceleration:

        eventLogger.log(
            currentTimeMs,
            "DRIVE",
            "Gentle deceleration phase"
        );

        break;

    case AutomaticDrivePhase::LowCruise:

        eventLogger.log(
            currentTimeMs,
            "DRIVE",
            "Low-speed cruise phase"
        );

        break;
    }
}

void SimulationEngine::updateAutomaticDriveCycle()
{
    if (!automaticDriveCycleEnabled)
    {
        return;
    }

    const float speedKmh =
        vehicleModel.getVehicleSpeedKmh();

    const double phaseElapsedMs =
        currentTimeMs -
        automaticPhaseStartTimeMs;

    switch (automaticDrivePhase)
    {
    case AutomaticDrivePhase::Acceleration:

        if (
            speedKmh >=
            automaticHighSpeedKmh
            )
        {
            setAutomaticDrivePhase(
                AutomaticDrivePhase::HighCruise
            );
        }

        break;

    case AutomaticDrivePhase::HighCruise:

        if (
            phaseElapsedMs >=
            automaticHighCruiseDurationMs
            )
        {
            setAutomaticDrivePhase(
                AutomaticDrivePhase::GentleDeceleration
            );
        }

        break;

    case AutomaticDrivePhase::GentleDeceleration:

        if (
            speedKmh <=
            automaticLowSpeedKmh
            )
        {
            setAutomaticDrivePhase(
                AutomaticDrivePhase::LowCruise
            );
        }

        break;

    case AutomaticDrivePhase::LowCruise:

        if (
            phaseElapsedMs >=
            automaticLowCruiseDurationMs
            )
        {
            setAutomaticDrivePhase(
                AutomaticDrivePhase::Acceleration
            );
        }

        break;
    }
}

void SimulationEngine::applyAutomaticDriveControl()
{
    vehicleModel.setFrontLeftWheelSlip(
        false
    );

    vehicleModel.setCoasting(
        false
    );

    switch (automaticDrivePhase)
    {
    case AutomaticDrivePhase::Acceleration:

        vehicleModel.setThrottle(
            automaticAccelerationThrottlePercent
        );

        vehicleModel.setBrake(
            0.0f
        );

        break;

    case AutomaticDrivePhase::HighCruise:

        // The current simplified VehicleModel has no
        // aerodynamic drag or rolling resistance.
        // Zero throttle therefore preserves speed.
        vehicleModel.setThrottle(
            0.0f
        );

        vehicleModel.setBrake(
            0.0f
        );

        break;

    case AutomaticDrivePhase::GentleDeceleration:

        vehicleModel.setThrottle(
            0.0f
        );

        vehicleModel.setBrake(
            automaticDecelerationBrakePercent
        );

        break;

    case AutomaticDrivePhase::LowCruise:

        vehicleModel.setThrottle(
            0.0f
        );

        vehicleModel.setBrake(
            0.0f
        );

        break;
    }
}

void SimulationEngine::applyManualDriveControl()
{
    const DrivingScenario scenario =
        scenarioController.getCurrentScenario();

    const float currentSpeedKmh =
        vehicleModel.getVehicleSpeedKmh();

    constexpr float accelerationTargetKmh =
        110.0f;

    constexpr float accelerationDeadBandKmh =
        1.0f;

    constexpr float cruiseDeadBandKmh =
        0.5f;

    constexpr float recoveryFloorKmh =
        45.0f;

    // ==================================================
    // Manual Acceleration
    //
    // A slower, more road-like acceleration command.
    // The vehicle converges to approximately 110 km/h
    // instead of increasing speed indefinitely.
    // ==================================================

    if (
        scenario ==
        DrivingScenario::Acceleration
        )
    {
        vehicleModel.setFrontLeftWheelSlip(
            false
        );

        if (
            currentSpeedKmh <
            accelerationTargetKmh -
            accelerationDeadBandKmh
            )
        {
            vehicleModel.setThrottle(
                45.0f
            );

            vehicleModel.setBrake(
                0.0f
            );

            vehicleModel.setCoasting(
                false
            );
        }
        else if (
            currentSpeedKmh >
            accelerationTargetKmh +
            accelerationDeadBandKmh
            )
        {
            vehicleModel.setThrottle(
                0.0f
            );

            vehicleModel.setBrake(
                0.0f
            );

            vehicleModel.setCoasting(
                true
            );
        }
        else
        {
            vehicleModel.setThrottle(
                0.0f
            );

            vehicleModel.setBrake(
                0.0f
            );

            vehicleModel.setCoasting(
                false
            );
        }

        return;
    }

    // ==================================================
    // Manual Cruise
    // ==================================================

    if (
        scenario ==
        DrivingScenario::Cruise
        )
    {
        vehicleModel.setFrontLeftWheelSlip(
            false
        );

        vehicleModel.setCoasting(
            false
        );

        if (
            currentSpeedKmh <
            manualCruiseTargetKmh -
            cruiseDeadBandKmh
            )
        {
            vehicleModel.setThrottle(
                12.0f
            );

            vehicleModel.setBrake(
                0.0f
            );
        }
        else if (
            currentSpeedKmh >
            manualCruiseTargetKmh +
            cruiseDeadBandKmh
            )
        {
            vehicleModel.setThrottle(
                0.0f
            );

            vehicleModel.setBrake(
                10.0f
            );
        }
        else
        {
            vehicleModel.setThrottle(
                0.0f
            );

            vehicleModel.setBrake(
                0.0f
            );
        }

        return;
    }

    // ==================================================
    // Manual Recovery
    //
    // Recovery now represents gentle coast-down after a
    // maneuver. It decelerates naturally toward a usable
    // road speed and then holds instead of falling to 0.
    // ==================================================

    if (
        scenario ==
        DrivingScenario::Recovery
        )
    {
        vehicleModel.setFrontLeftWheelSlip(
            false
        );

        vehicleModel.setThrottle(
            0.0f
        );

        vehicleModel.setBrake(
            0.0f
        );

        vehicleModel.setCoasting(
            currentSpeedKmh >
            recoveryFloorKmh
        );

        return;
    }

    // ==================================================
    // Idle / Hard Braking
    // ==================================================

    scenarioController.update(
        vehicleModel
    );
}

// ==================================================
// Runtime UDS request submission
// ==================================================

void SimulationEngine::submitUdsRequest(
    const UdsRequest& request)
{
    if (udsTransactionPending)
    {
        throw std::logic_error(
            "A UDS runtime transaction is already pending."
        );
    }

    // A new transaction replaces any previously
    // completed response evidence.
    udsResponseAvailable =
        false;

    completedUdsResponse =
        UdsResponse{};

    pendingUdsRequest =
        request;

    receivedUdsRequestFrames.clear();
    receivedUdsResponseFrames.clear();

    pendingUdsResponseFrames.clear();

    pendingUdsRequestFrames =
        udsTransport.createRequestFrames(
            pendingUdsRequest
        );

    if (pendingUdsRequestFrames.empty())
    {
        throw std::runtime_error(
            "UDS runtime request produced no ISO-TP CAN frames."
        );
    }

    udsTransactionPending =
        true;

    // Only one ISO-TP frame is submitted at a time.
    //
    // This is intentional. Multiple ISO-TP frames with
    // the same CAN identifier must preserve transport
    // ordering and must not be inserted simultaneously
    // into the arbitration queue.
    queuePendingUdsRequestFrames(
        currentTimeMs
    );
}

// ==================================================
// Runtime UDS transaction state
// ==================================================

bool SimulationEngine::hasPendingUdsTransaction() const
{
    return udsTransactionPending;
}

bool SimulationEngine::hasCompletedUdsResponse() const
{
    return udsResponseAvailable;
}

const UdsResponse&
SimulationEngine::getCompletedUdsResponse() const
{
    if (!udsResponseAvailable)
    {
        throw std::logic_error(
            "No completed UDS runtime response is available."
        );
    }

    return completedUdsResponse;
}

void SimulationEngine::clearCompletedUdsResponse()
{
    udsResponseAvailable =
        false;

    completedUdsResponse =
        UdsResponse{};
}

// ==================================================
// Runtime Ethernet frame submission
// ==================================================

bool SimulationEngine::submitEthernetFrame(
    const MacAddress& sourceMac,
    const MacAddress& destinationMac,
    std::uint16_t etherType,
    const std::vector<std::uint8_t>& payload)
{
    if (
        sourceMac ==
        ethernetNodeA.getMacAddress()
        )
    {
        return ethernetNodeA.transmit(
            ethernetBus,
            destinationMac,
            etherType,
            payload,
            currentTimeMs
        );
    }

    if (
        sourceMac ==
        ethernetNodeB.getMacAddress()
        )
    {
        return ethernetNodeB.transmit(
            ethernetBus,
            destinationMac,
            etherType,
            payload,
            currentTimeMs
        );
    }

    throw std::invalid_argument(
        "Ethernet source MAC is not configured in SimulationEngine."
    );
}

// ==================================================
// Runtime UDS request CAN queue
// ==================================================

void SimulationEngine::queuePendingUdsRequestFrames(
    double requestTimeMs)
{
    if (pendingUdsRequestFrames.empty())
    {
        return;
    }

    const CanFrame frame =
        pendingUdsRequestFrames.front();

    pendingUdsRequestFrames.erase(
        pendingUdsRequestFrames.begin()
    );

    canBus.transmit(
        frame,
        requestTimeMs
    );

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout
            << "["
            << requestTimeMs
            << " ms] UDS REQUEST -> 0x"
            << std::hex
            << frame.arbitrationId
            << std::dec
            << "\n";
    }
}

// ==================================================
// Runtime UDS response CAN queue
// ==================================================

void SimulationEngine::queueUdsResponseFrames(
    const UdsResponse& response,
    double requestTimeMs)
{
    pendingUdsResponseFrames =
        udsTransport.createResponseFrames(
            response
        );

    if (pendingUdsResponseFrames.empty())
    {
        throw std::runtime_error(
            "UDS runtime response produced no ISO-TP CAN frames."
        );
    }

    const CanFrame frame =
        pendingUdsResponseFrames.front();

    pendingUdsResponseFrames.erase(
        pendingUdsResponseFrames.begin()
    );

    canBus.transmit(
        frame,
        requestTimeMs
    );

    if (
        logLevel ==
        LogLevel::Verbose
        )
    {
        std::cout
            << "["
            << requestTimeMs
            << " ms] UDS RESPONSE -> 0x"
            << std::hex
            << frame.arbitrationId
            << std::dec
            << "\n";
    }
}

// ==================================================
// Runtime UDS request completeness
// ==================================================

bool SimulationEngine::isUdsRequestComplete() const
{
    if (receivedUdsRequestFrames.empty())
    {
        return false;
    }

    try
    {
        udsTransport.decodeRequestFrames(
            receivedUdsRequestFrames
        );

        return true;
    }
    catch (
        const std::exception&
        )
    {
        return false;
    }
}

// ==================================================
// Runtime UDS response completeness
// ==================================================

bool SimulationEngine::isUdsResponseComplete() const
{
    if (receivedUdsResponseFrames.empty())
    {
        return false;
    }

    try
    {
        udsTransport.decodeResponseFrames(
            receivedUdsResponseFrames
        );

        return true;
    }
    catch (
        const std::exception&
        )
    {
        return false;
    }
}

// ==================================================
// Runtime UDS request frame processing
// ==================================================

void SimulationEngine::processUdsRequestFrame(
    const CanFrame& frame,
    double eventTimeMs)
{
    receivedUdsRequestFrames.push_back(
        frame
    );

    // ==================================================
    // Complete request
    // ==================================================

    if (isUdsRequestComplete())
    {
        const UdsRequest request =
            udsTransport.decodeRequestFrames(
                receivedUdsRequestFrames
            );

        // Ensure current vehicle values are available
        // immediately before diagnostic processing.
        updateUdsVehicleData();

        const UdsResponse response =
            udsServer.processRequest(
                request
            );

        pendingUdsRequestFrames.clear();

        queueUdsResponseFrames(
            response,
            eventTimeMs
        );

        return;
    }

    // ==================================================
    // Continue ISO-TP request
    // ==================================================

    if (!pendingUdsRequestFrames.empty())
    {
        queuePendingUdsRequestFrames(
            eventTimeMs
        );
    }
}

// ==================================================
// Runtime UDS response frame processing
// ==================================================

void SimulationEngine::processUdsResponseFrame(
    const CanFrame& frame)
{
    receivedUdsResponseFrames.push_back(
        frame
    );

    // ==================================================
    // Complete response
    // ==================================================

    if (isUdsResponseComplete())
    {
        completedUdsResponse =
            udsTransport.decodeResponseFrames(
                receivedUdsResponseFrames
            );

        pendingUdsResponseFrames.clear();

        udsTransactionPending =
            false;

        udsResponseAvailable =
            true;

        return;
    }

    // ==================================================
    // Continue ISO-TP response
    // ==================================================

    if (!pendingUdsResponseFrames.empty())
    {
        const CanFrame nextFrame =
            pendingUdsResponseFrames.front();

        pendingUdsResponseFrames.erase(
            pendingUdsResponseFrames.begin()
        );

        const double requestTimeMs =
            canBus.getBusyUntilMs();

        canBus.transmit(
            nextFrame,
            requestTimeMs
        );

        if (
            logLevel ==
            LogLevel::Verbose
            )
        {
            std::cout
                << "["
                << requestTimeMs
                << " ms] UDS RESPONSE -> 0x"
                << std::hex
                << nextFrame.arbitrationId
                << std::dec
                << "\n";
        }
    }
}

// ==================================================
// Runtime CAN routing
// ==================================================

void SimulationEngine::processReceivedCanFrame(
    const CanFrame& frame,
    double eventTimeMs)
{
    // ==================================================
    // UDS physical request
    // ==================================================

    if (
        frame.arbitrationId ==
        udsRequestCanId
        )
    {
        processUdsRequestFrame(
            frame,
            eventTimeMs
        );

        return;
    }

    // ==================================================
    // UDS physical response
    // ==================================================

    if (
        frame.arbitrationId ==
        udsResponseCanId
        )
    {
        processUdsResponseFrame(
            frame
        );

        return;
    }

    // ==================================================
    // Existing vehicle traffic
    // ==================================================

    dashboardEcu.receiveFrame(
        frame
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

    // ==================================================
    // Diagnostic fault monitoring
    // ==================================================

    if (!frontLeftReading.valid)
    {
        dtcManager.reportFault(
            DtcDefinitions::FrontLeftWheelSpeedSensor,
            eventTimeMs
        );
    }
    else
    {
        dtcManager.reportHealthy(
            DtcDefinitions::FrontLeftWheelSpeedSensor,
            eventTimeMs
        );
    }

    if (!frontRightReading.valid)
    {
        dtcManager.reportFault(
            DtcDefinitions::FrontRightWheelSpeedSensor,
            eventTimeMs
        );
    }
    else
    {
        dtcManager.reportHealthy(
            DtcDefinitions::FrontRightWheelSpeedSensor,
            eventTimeMs
        );
    }

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
        : vehicleState.speedKmh < 60.0f
        ? 3
        : vehicleState.speedKmh < 80.0f
        ? 4
        : 5;

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
// UDS runtime vehicle data
// ==================================================

void SimulationEngine::updateUdsVehicleData()
{
    UdsVehicleData data;

    data.vehicleSpeedKmh =
        vehicleModel
        .getVehicleSpeedKmh();

    data.engineRpm =
        vehicleState.engineRpm;

    data.steeringAngleDeg =
        vehicleModel
        .getSteeringAngleDeg();

    udsServer.setVehicleData(
        data
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

        // The frame has completed transmission at
        // busyUntilMs. Any diagnostic response generated
        // as a result of this frame therefore requests the
        // bus no earlier than that completion time.
        const double frameCompletionTimeMs =
            canBus.getBusyUntilMs();

        processReceivedCanFrame(
            frame,
            frameCompletionTimeMs
        );

        busTimeMs =
            canBus.getBusyUntilMs();
    }
}

// ==================================================
// Runtime UDS reset
// ==================================================

void SimulationEngine::resetUdsRuntimeState()
{
    udsTransactionPending =
        false;

    udsResponseAvailable =
        false;

    pendingUdsRequest =
        UdsRequest{};

    completedUdsResponse =
        UdsResponse{};

    pendingUdsRequestFrames.clear();
    receivedUdsRequestFrames.clear();

    pendingUdsResponseFrames.clear();
    receivedUdsResponseFrames.clear();
}

// ==================================================
// Runtime Ethernet processing
// ==================================================

void SimulationEngine::processEthernetBus()
{
    if (
        currentTimeMs <
        ethernetBus.getBusyUntilMs()
        )
    {
        return;
    }

    ethernetNodeA.processReceivedFrames(
        ethernetBus
    );

    ethernetNodeB.processReceivedFrames(
        ethernetBus
    );
}

// ==================================================
// Runtime Ethernet initialization
// ==================================================

void SimulationEngine::initializeEthernetRuntime()
{
    ethernetBus.registerNode(
        ethernetNodeA.getMacAddress()
    );

    ethernetBus.registerNode(
        ethernetNodeB.getMacAddress()
    );
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

    automaticDriveCycleEnabled =
        false;

    automaticDrivePhase =
        AutomaticDrivePhase::Acceleration;

    automaticPhaseStartTimeMs =
        0.0;

    manualCruiseTargetKmh =
        0.0f;

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

    dtcManager =
        DtcManager{};

    powertrainEcu =
        PowertrainEcu{};

    absEcu =
        AbsEcu{};

    steeringEcu =
        SteeringEcu{};

    dashboardEcu =
        DashboardEcu{};

    canBus =
        VirtualCanBus(
            canBitrate,
            logLevel
        );

    ethernetBus =
        VirtualEthernetBus(
            ethernetLinkRate
        );

    ethernetNodeA.clearReceivedFrames();
    ethernetNodeB.clearReceivedFrames();

    initializeEthernetRuntime();

    resetUdsRuntimeState();

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

    // Reset UDS session and cached diagnostic data.
    // The UdsServer still references this same dtcManager object.
    udsServer.reset();

    updateUdsVehicleData();
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

const VirtualEthernetBus&
SimulationEngine::getEthernetBus() const
{
    return ethernetBus;
}

const EthernetNode&
SimulationEngine::getEthernetNodeA() const
{
    return ethernetNodeA;
}

const EthernetNode&
SimulationEngine::getEthernetNodeB() const
{
    return ethernetNodeB;
}

const DtcManager&
SimulationEngine::getDtcManager() const
{
    return dtcManager;
}

UdsServer&
SimulationEngine::getUdsServer()
{
    return udsServer;
}

const UdsServer&
SimulationEngine::getUdsServer() const
{
    return udsServer;
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