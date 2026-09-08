#pragma once

#include <cstdint>
#include <vector>

#include "../VirtualVehicle.h"

#include "../can/VirtualCanBus.h"
#include "../can/CanTraceEntry.h"
#include "../can/CanStatistics.h"

#include "../ethernet/EthernetNode.h"
#include "../ethernet/VirtualEthernetBus.h"

#include "../ecu/PowertrainEcu.h"
#include "../ecu/AbsEcu.h"
#include "../ecu/SteeringEcu.h"
#include "../ecu/DashboardEcu.h"

#include "../sensor/WheelSpeedSensor.h"

#include "../diagnostics/DtcManager.h"
#include "../diagnostics/UdsServer.h"
#include "../diagnostics/UdsTransport.h"
#include "../diagnostics/UdsRequest.h"
#include "../diagnostics/UdsResponse.h"

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

    void injectAbsCanCommunicationFault();
    void clearAbsCanCommunicationFault();

    void injectPowertrainCanCommunicationFault();
    void clearPowertrainCanCommunicationFault();

    void injectSteeringCanCommunicationFault();
    void clearSteeringCanCommunicationFault();

    void injectEthernetNodeACommunicationFault();
    void clearEthernetNodeACommunicationFault();

    bool isAbsCanCommunicationFaultActive() const;
    bool isPowertrainCanCommunicationFaultActive() const;
    bool isSteeringCanCommunicationFaultActive() const;
    bool isEthernetNodeACommunicationFaultActive() const;

    void runDemo();
    void printResults() const;

    void startScenario(
        DrivingScenario scenario
    );

    void startAutomaticDriveCycle();
    void stopAutomaticDriveCycle();

    bool isAutomaticDriveCycleEnabled() const;

    DrivingScenario getCurrentScenario() const;

    void submitUdsRequest(
        const UdsRequest& request
    );

    bool hasPendingUdsTransaction() const;

    bool hasCompletedUdsResponse() const;

    const UdsResponse&
        getCompletedUdsResponse() const;

    void clearCompletedUdsResponse();

    bool submitEthernetFrame(
        const MacAddress& sourceMac,
        const MacAddress& destinationMac,
        std::uint16_t etherType,
        const std::vector<std::uint8_t>& payload
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

    const VirtualEthernetBus&
        getEthernetBus() const;

    const EthernetNode&
        getEthernetNodeA() const;

    const EthernetNode&
        getEthernetNodeB() const;

    const DtcManager&
        getDtcManager() const;

    UdsServer&
        getUdsServer();

    const UdsServer&
        getUdsServer() const;

private:
    static constexpr std::uint32_t canBitrate =
        500000;

    static constexpr std::uint64_t ethernetLinkRate =
        100000000;

    static constexpr std::uint32_t udsRequestCanId =
        0x7E0;

    static constexpr std::uint32_t udsResponseCanId =
        0x7E8;

    static constexpr float automaticHighSpeedKmh =
        100.0f;

    static constexpr float automaticLowSpeedKmh =
        70.0f;

    static constexpr float automaticAccelerationThrottlePercent =
        55.0f;

    static constexpr float automaticDecelerationBrakePercent =
        12.0f;

    static constexpr double automaticHighCruiseDurationMs =
        3500.0;

    static constexpr double automaticLowCruiseDurationMs =
        2200.0;

    LogLevel logLevel;

    VirtualCanBus canBus;

    VirtualEthernetBus ethernetBus;
    EthernetNode ethernetNodeA;
    EthernetNode ethernetNodeB;

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
    UdsTransport udsTransport;

    VehicleState vehicleState{};
    SteeringState steeringState{};

    double currentTimeMs{ 0.0 };

    double nextAbsTxMs{ 0.0 };
    double nextPowertrainTxMs{ 0.0 };
    double nextSteeringTxMs{ 0.0 };

    bool previousAbsActive{ false };
    bool previousBrakeApplied{ false };
    bool vehicleWasMoving{ false };

    bool absCanCommunicationFaultActive{ false };
    bool powertrainCanCommunicationFaultActive{ false };
    bool steeringCanCommunicationFaultActive{ false };
    bool ethernetNodeACommunicationFaultActive{ false };

    AbsHealthStatus previousAbsHealthStatus{
        AbsHealthStatus::Healthy
    };

    bool udsTransactionPending{ false };
    bool udsResponseAvailable{ false };

    UdsRequest pendingUdsRequest{};
    UdsResponse completedUdsResponse{};

    std::vector<CanFrame> pendingUdsRequestFrames;
    std::vector<CanFrame> receivedUdsRequestFrames;
    std::vector<CanFrame> pendingUdsResponseFrames;
    std::vector<CanFrame> receivedUdsResponseFrames;

    enum class AutomaticDrivePhase
    {
        Acceleration,
        HighCruise,
        GentleDeceleration,
        LowCruise
    };

    bool automaticDriveCycleEnabled{ false };

    AutomaticDrivePhase automaticDrivePhase{
        AutomaticDrivePhase::Acceleration
    };

    double automaticPhaseStartTimeMs{ 0.0 };

    float manualCruiseTargetKmh{ 0.0f };

    void updateAutomaticDriveCycle();
    void applyAutomaticDriveControl();
    void applyManualDriveControl();

    void setAutomaticDrivePhase(
        AutomaticDrivePhase phase
    );

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

    void processReceivedCanFrame(
        const CanFrame& frame,
        double eventTimeMs
    );

    void queuePendingUdsRequestFrames(
        double requestTimeMs
    );

    void processUdsRequestFrame(
        const CanFrame& frame,
        double eventTimeMs
    );

    void processUdsResponseFrame(
        const CanFrame& frame
    );

    void queueUdsResponseFrames(
        const UdsResponse& response,
        double requestTimeMs
    );

    bool isUdsRequestComplete() const;

    bool isUdsResponseComplete() const;

    void processEthernetBus();

    void initializeEthernetRuntime();

    double getNextInternalEventTime(
        double targetTimeMs
    ) const;

    void advancePhysicsTo(
        double newTimeMs
    );

    void updateUdsVehicleData();

    void resetUdsRuntimeState();
};
