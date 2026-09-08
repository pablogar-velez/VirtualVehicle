# ============================================================
# VirtualVehicle executable
# ============================================================

find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Widgets
)

add_executable(
    VirtualVehicle

    # ========================================================
    # Main
    # ========================================================

    "VirtualVehicle.cpp"
    "VirtualVehicle.h"

    # ========================================================
    # CAN
    # ========================================================

    "can/CanFrame.h"
    "can/PendingCanFrame.h"
    "can/CanArbitration.h"
    "can/CanArbitration.cpp"
    "can/CanTiming.h"
    "can/CanTiming.cpp"
    "can/CanStatistics.h"
    "can/CanStatistics.cpp"
    "can/CanMessageRegistry.h"
    "can/CanMessageRegistry.cpp"
    "can/CanMessageDefinitions.h"
    "can/VehicleStateCodec.h"
    "can/VehicleStateCodec.cpp"
    "can/AbsStateCodec.h"
    "can/AbsStateCodec.cpp"
    "can/SteeringStateCodec.h"
    "can/SteeringStateCodec.cpp"
    "can/VirtualCanBus.h"
    "can/VirtualCanBus.cpp"
    "can/CanTraceEntry.h"
    "can/CanTracePrinter.h"
    "can/CanTracePrinter.cpp"

    # ========================================================
    # ECUs
    # ========================================================

    "ecu/PowertrainEcu.h"
    "ecu/PowertrainEcu.cpp"
    "ecu/AbsEcu.h"
    "ecu/AbsEcu.cpp"
    "ecu/SteeringEcu.h"
    "ecu/SteeringEcu.cpp"
    "ecu/DashboardEcu.h"
    "ecu/DashboardEcu.cpp"

    # ========================================================
    # Sensors
    # ========================================================

    "sensor/SensorFault.h"
    "sensor/WheelSpeedSensor.h"
    "sensor/WheelSpeedSensor.cpp"

    # ========================================================
    # Diagnostics / DTC
    # ========================================================

    "diagnostics/DtcStatus.h"
    "diagnostics/Dtc.h"
    "diagnostics/DtcDefinitions.h"
    "diagnostics/DtcManager.h"
    "diagnostics/DtcManager.cpp"

    # ========================================================
    # UDS Diagnostics
    # ========================================================

    "diagnostics/UdsService.h"
    "diagnostics/UdsRequest.h"
    "diagnostics/UdsResponse.h"
    "diagnostics/UdsServer.h"
    "diagnostics/UdsServer.cpp"
    "diagnostics/UdsTransport.h"
    "diagnostics/UdsTransport.cpp"
    "diagnostics/UdsCanTransport.h"
    "diagnostics/UdsCanTransport.cpp"

    # ========================================================
    # Transport
    # ========================================================

    "transport/IsoTpFrame.h"
    "transport/IsoTpProtocol.h"
    "transport/IsoTpProtocol.cpp"

    # ========================================================
    # Automotive Ethernet
    # ========================================================

    "ethernet/MacAddress.h"
    "ethernet/EthernetFrame.h"
    "ethernet/EthernetTiming.h"
    "ethernet/EthernetTiming.cpp"
    "ethernet/EthernetTraceEntry.h"
    "ethernet/EthernetStatistics.h"
    "ethernet/EthernetStatistics.cpp"
    "ethernet/EthernetNode.h"
    "ethernet/EthernetNode.cpp"
    "ethernet/VirtualEthernetBus.h"
    "ethernet/VirtualEthernetBus.cpp"

    # ========================================================
    # Simulation
    # ========================================================

    "simulation/VehicleModel.h"
    "simulation/VehicleModel.cpp"
    "simulation/EventLogger.h"
    "simulation/EventLogger.cpp"
    "simulation/ScenarioController.h"
    "simulation/ScenarioController.cpp"
    "simulation/SimulationEngine.h"
    "simulation/SimulationEngine.cpp"
    "simulation/LogLevel.h"

    # ========================================================
    # GUI
    # ========================================================

    "gui/MainWindow.h"
    "gui/MainWindow.cpp"

    # ========================================================
    # GUI Widgets
    # ========================================================

    "gui/widgets/VehicleStateWidget.h"
    "gui/widgets/VehicleStateWidget.cpp"
    "gui/widgets/AbsStateWidget.h"
    "gui/widgets/AbsStateWidget.cpp"
    "gui/widgets/ScenarioControlWidget.h"
    "gui/widgets/ScenarioControlWidget.cpp"
    "gui/widgets/FaultInjectionWidget.h"
    "gui/widgets/FaultInjectionWidget.cpp"
    "gui/widgets/EventLogWidget.h"
    "gui/widgets/EventLogWidget.cpp"
    "gui/widgets/CanMonitorWidget.h"
    "gui/widgets/CanMonitorWidget.cpp"
    "gui/widgets/StatisticsWidget.h"
    "gui/widgets/StatisticsWidget.cpp"
    "gui/widgets/TestRunnerWidget.h"
    "gui/widgets/TestRunnerWidget.cpp"

    # ========================================================
    # Automated Tests
    # ========================================================

    "test/TestResult.h"
    "test/TestCase.h"
    "test/TestConfig.h"
    "test/TestRunner.h"
    "test/TestRunner.cpp"
    "test/SystemTests.cpp"
    "test/VehicleTests.cpp"
    "test/AbsTests.cpp"
    "test/SensorTests.cpp"
    "test/PowertrainTests.cpp"
    "test/SteeringTests.cpp"
    "test/CanTests.cpp"
    "test/DiagnosticsTests.cpp"
    "test/DtcTests.cpp"
    "test/MessageTests.cpp"
    "test/ValidationTests.cpp"

    # ========================================================
    # Diagnostic / Transport Tests
    # ========================================================

    "test/UdsTests.cpp"
    "test/IsoTpTests.cpp"
    "test/UdsTransportTest.cpp"
    "test/UdsCanTransportTests.cpp"
    "test/UdsRuntimeIntegrationTests.cpp"

    # ========================================================
    # Automotive Ethernet Tests
    # ========================================================

    "test/EthernetTest.cpp"
 "gui/widgets/AutomotiveGaugeWidget.h" "test/DiagnosticFaultExpansionTests.cpp")

target_compile_features(
    VirtualVehicle
    PRIVATE
        cxx_std_20
)

target_link_libraries(
    VirtualVehicle
    PRIVATE
        Qt6::Widgets
)
