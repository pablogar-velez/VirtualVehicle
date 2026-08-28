# ============================================================
# VirtualVehicle executable
# ============================================================

# ------------------------------------------------------------
# Qt
# ------------------------------------------------------------

find_package(
    Qt6
    REQUIRED
    COMPONENTS
    Widgets
)

# ------------------------------------------------------------
# Executable
# ------------------------------------------------------------

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
)

# ------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------

target_compile_features(
    VirtualVehicle
    PRIVATE
    cxx_std_20
)

# ------------------------------------------------------------
# Qt libraries
# ------------------------------------------------------------

target_link_libraries(
    VirtualVehicle
    PRIVATE
    Qt6::Widgets
)