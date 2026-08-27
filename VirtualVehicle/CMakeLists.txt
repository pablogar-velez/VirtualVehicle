# CMakeList.txt: proyecto de CMake para VirtualVehicle, incluya el origen y defina
# la lógica específica del proyecto aquí.
#

# Agregue un origen al ejecutable de este proyecto.
add_executable(
    VirtualVehicle

    "VirtualVehicle.cpp"
    "VirtualVehicle.h"

    "can/CanFrame.h"

    "can/VehicleStateCodec.h"
    "can/VehicleStateCodec.cpp"

    "can/VirtualCanBus.h"
    "can/VirtualCanBus.cpp"

    "ecu/PowertrainEcu.h"
    "ecu/PowertrainEcu.cpp"

    "ecu/DashboardEcu.h"
    "ecu/DashboardEcu.cpp"
    "ecu/AbsEcu.h" 
    "ecu/AbsEcu.cpp" 
    "can/AbsStateCodec.h" 
    "can/AbsStateCodec.cpp"

 "can/CanArbitration.h" "can/CanArbitration.cpp" "can/PendingCanFrame.h" "can/CanTiming.h" "can/CanTiming.cpp" "can/CanStatistics.h" "can/CanStatistics.cpp" "can/CanMessageRegistry.h" "can/CanMessageRegistry.cpp" "can/CanMessageDefinitions.h" "can/SteeringStateCodec.h" "can/SteeringStateCodec.cpp" "ecu/SteeringEcu.h" "ecu/SteeringEcu.cpp" "sensor/SensorFault.h" "sensor/WheelSpeedSensor.h" "sensor/WheelSpeedSensor.cpp" "simulation/VehicleModel.h" "simulation/VehicleModel.cpp" "simulation/EventLogger.h" "simulation/EventLogger.cpp" "simulation/ScenarioController.h" "simulation/ScenarioController.cpp" "simulation/LogLevel.h" "can/CanTraceEntry.h" "can/CanTracePrinter.h" "can/CanTracePrinter.cpp" "simulation/SimulationEngine.h" "simulation/SimulationEngine.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VirtualVehicle PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.
