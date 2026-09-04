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
#include "../diagnostics/UdsServer.h"

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

    void update(double deltaTimeMs);
    void reset();

    void setFrontLeftWheelSensorFault(
        SensorFault fault
    );

    void setFrontRightWheelSensorFault(
        SensorFault fault
    );

    void clearFrontLeftWheelSensorFault();
    void clearFrontRightWheelSensorFault();

    void runDemo();
    void printResults() const;

    void startScenario(
        DrivingScenario scenario
    );

    double getCurrentTimeMs() const;

    const VehicleModel&
        getVehicleModel() const;

    const VehicleState&
        getVehicleState() const;

    const SteeringState&
        getSteeringState() const;

    const AbsState&
        getAbsState() const;

    const std::vector<VehicleEvent>&
        getEvents() const;

    const std::vector<CanTraceEntry>&
        getCanTrace() const;

    const CanStatistics&
        getCanStatistics() const;

    std::uint32_t
        getCanBitrate() const;

    const DtcManager&
        getDtcManager() const;

    UdsServer&
        getUdsServer();

    const UdsServer&
        getUdsServer() const;

private:
    static constexpr std::uint32_t canBitrate =
        500000;

    LogLevel logLevel;

    VirtualCanBus canBus;

    PowertrainEcu powertrainEcu;
    AbsEcu absEcu;
    SteeringEcu steeringEcu;
    DashboardEcu dashboardEcu;

    VehicleModel vehicleModel;
    ScenarioController scenarioController;
    EventLogger eventLogger;

    WheelSpeedSensor frontLeftWheelSensor;
    WheelSpeedSensor frontRightWheelSensor;

    DtcManager dtcManager;
    UdsServer udsServer;

    VehicleState vehicleState{};
    SteeringState steeringState{};

    double currentTimeMs{ 0.0 };

    double nextAbsTxMs{ 0.0 };
    double nextPowertrainTxMs{ 0.0 };
    double nextSteeringTxMs{ 0.0 };

    bool previousAbsActive{ false };
    bool previousBrakeApplied{ false };
    bool vehicleWasMoving{ false };

    AbsHealthStatus previousAbsHealthStatus{
        AbsHealthStatus::Healthy
    };

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

    void updateUdsVehicleData();
};