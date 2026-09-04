#pragma once

#include <cstdint>
#include <vector>

#include "../VirtualVehicle.h"

#include "../can/VirtualCanBus.h"
#include "../can/CanTraceEntry.h"
#include "../can/CanStatistics.h"

#include "../ecu/PowertrainEcu.h"
#include "../ecu/AbsEcu.h"
#include "../ecu/SteeringEcu.h"
#include "../ecu/DashboardEcu.h"

#include "../sensor/WheelSpeedSensor.h"

#include "../diagnostics/DtcManager.h"

#include "VehicleModel.h"
#include "ScenarioController.h"
#include "EventLogger.h"
#include "LogLevel.h"

class SimulationEngine
{
public:
    explicit SimulationEngine(
        LogLevel logLevel = LogLevel::Events
    );

    // ==================================================
    // Interactive simulation
    // ==================================================

    void update(
        double deltaTimeMs
    );

    void reset();

    // ==================================================
    // Fault injection
    // ==================================================

    void setFrontLeftWheelSensorFault(
        SensorFault fault
    );

    void setFrontRightWheelSensorFault(
        SensorFault fault
    );

    void clearFrontLeftWheelSensorFault();

    void clearFrontRightWheelSensorFault();

    // ==================================================
    // Demo
    // ==================================================

    void runDemo();

    void printResults() const;

    // ==================================================
    // Scenario control
    // ==================================================

    void startScenario(
        DrivingScenario scenario
    );

    // ==================================================
    // State access
    // ==================================================

    double getCurrentTimeMs() const;

    const VehicleModel&
        getVehicleModel() const;

    const VehicleState&
        getVehicleState() const;

    const SteeringState&
        getSteeringState() const;

    const AbsState&
        getAbsState() const;

    // ==================================================
    // Diagnostics access
    // ==================================================

    const DtcManager&
        getDtcManager() const;

    // ==================================================
    // Event access
    // ==================================================

    const std::vector<VehicleEvent>&
        getEvents() const;

    // ==================================================
    // CAN trace access
    // ==================================================

    const std::vector<CanTraceEntry>&
        getCanTrace() const;

    // ==================================================
    // CAN statistics access
    // ==================================================

    const CanStatistics&
        getCanStatistics() const;

    std::uint32_t
        getCanBitrate() const;

private:
    // ==================================================
    // Configuration
    // ==================================================

    static constexpr std::uint32_t canBitrate =
        500000;

    LogLevel logLevel;

    // ==================================================
    // CAN network
    // ==================================================

    VirtualCanBus canBus;

    // ==================================================
    // ECUs
    // ==================================================

    PowertrainEcu powertrainEcu;
    AbsEcu absEcu;
    SteeringEcu steeringEcu;
    DashboardEcu dashboardEcu;

    // ==================================================
    // Simulation
    // ==================================================

    VehicleModel vehicleModel;
    ScenarioController scenarioController;
    EventLogger eventLogger;

    // ==================================================
    // Sensors
    // ==================================================

    WheelSpeedSensor frontLeftWheelSensor;
    WheelSpeedSensor frontRightWheelSensor;

    // ==================================================
    // Diagnostics
    // ==================================================

    DtcManager dtcManager;

    // ==================================================
    // ECU application states
    // ==================================================

    VehicleState vehicleState{};
    SteeringState steeringState{};

    // ==================================================
    // Timing
    // ==================================================

    double currentTimeMs{ 0.0 };

    double nextAbsTxMs{ 0.0 };
    double nextPowertrainTxMs{ 0.0 };
    double nextSteeringTxMs{ 0.0 };

    // ==================================================
    // Event detection
    // ==================================================

    bool previousAbsActive{ false };
    bool previousBrakeApplied{ false };
    bool vehicleWasMoving{ false };

    AbsHealthStatus previousAbsHealthStatus{
        AbsHealthStatus::Healthy
    };

    // ==================================================
    // Internal processing
    // ==================================================

    void updateBrakeEvents();

    void updateVehicleEvents();

    void updateSensors();

    void processAbsEcu(
        double eventTimeMs
    );

    void processPowertrainEcu(
        double eventTimeMs
    );

    void processSteeringEcu(
        double eventTimeMs
    );

    void processCanBus(
        double eventTimeMs
    );

    double getNextInternalEventTime(
        double targetTimeMs
    ) const;

    void advancePhysicsTo(
        double newTimeMs
    );
};