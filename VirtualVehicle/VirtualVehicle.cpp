#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

#include "VirtualVehicle.h"

#include "can/VirtualCanBus.h"
#include "can/CanMessageDefinitions.h"

#include "ecu/PowertrainEcu.h"
#include "ecu/AbsEcu.h"
#include "ecu/SteeringEcu.h"
#include "ecu/DashboardEcu.h"

#include "sensor/WheelSpeedSensor.h"

int main()
{
    std::cout << std::fixed
        << std::setprecision(3);

    std::cout << "=================================\n";
    std::cout << "     VIRTUAL VEHICLE PLATFORM    \n";
    std::cout << "=================================\n\n";

    // ==================================================
    // CAN network
    // ==================================================

    constexpr std::uint32_t canBitrate = 500000;

    VirtualCanBus canBus(canBitrate);

    PowertrainEcu powertrainEcu;
    AbsEcu absEcu;
    SteeringEcu steeringEcu;
    DashboardEcu dashboardEcu;

    // ==================================================
    // Virtual sensors
    // ==================================================

    WheelSpeedSensor frontLeftWheelSensor;
    WheelSpeedSensor frontRightWheelSensor;

    // True physical values.
    // Later these will come from the vehicle simulation.
    frontLeftWheelSensor.setTrueSpeed(82.1f);
    frontRightWheelSensor.setTrueSpeed(82.4f);


    // ==================================================
    // Initial vehicle state
    // ==================================================

    VehicleState vehicle{};

    vehicle.speedKmh = 82.5f;
    vehicle.engineRpm = 2140;
    vehicle.gear = 3;
    vehicle.brakeApplied = false;

    // ==================================================
    // Steering state
    // ==================================================

    SteeringState steeringState{};

    steeringState.steeringAngleDeg = 3.2f;
    steeringState.steeringTorqueNm = 1.45f;

    // ==================================================
    // Configure ECUs
    // ==================================================

    powertrainEcu.setVehicleState(vehicle);

    steeringEcu.setSteeringState(
        steeringState
    );

    // ==================================================
    // Simulation configuration
    // ==================================================

    constexpr double simulationDurationMs = 100.0;

    constexpr double absPeriodMs =
        CanMessageDefinitions::ABS_WHEEL_STATE.periodMs;

    constexpr double powertrainPeriodMs =
        CanMessageDefinitions::POWERTRAIN_STATE.periodMs;

    constexpr double steeringPeriodMs =
        CanMessageDefinitions::STEERING_STATE.periodMs;

    double currentTimeMs = 0.0;

    double nextAbsTxMs = 0.0;
    double nextPowertrainTxMs = 0.0;
    double nextSteeringTxMs = 0.0;

    // ==================================================
    // Event-driven simulation
    // ==================================================

    while (
        currentTimeMs <= simulationDurationMs ||
        canBus.hasFrame()
        )
    {
        // --------------------------------------------------
        // ABS ECU periodic event
        // --------------------------------------------------

        if (
            currentTimeMs <= simulationDurationMs &&
            currentTimeMs >= nextAbsTxMs
            )
        {
            // Read wheel-speed sensors.

            const WheelSpeedReading frontLeftReading =
                frontLeftWheelSensor.readSpeed();

            const WheelSpeedReading frontRightReading =
                frontRightWheelSensor.readSpeed();

            // The ABS ECU is now responsible for validating
            // and processing the sensor readings.

            absEcu.updateSensors(
                frontLeftReading,
                frontRightReading
            );

            std::cout
                << "["
                << currentTimeMs
                << " ms] ABS ECU request -> 0x"
                << std::hex
                << CanMessageDefinitions::ABS_WHEEL_STATE.id
                << std::dec
                << "\n";

            absEcu.transmitAbsState(
                canBus,
                currentTimeMs
            );

            nextAbsTxMs += absPeriodMs;
        }

        // --------------------------------------------------
        // Powertrain ECU periodic event
        // --------------------------------------------------

        if (
            currentTimeMs <= simulationDurationMs &&
            currentTimeMs >= nextPowertrainTxMs
            )
        {
            std::cout
                << "["
                << currentTimeMs
                << " ms] POWERTRAIN ECU request -> 0x"
                << std::hex
                << CanMessageDefinitions::POWERTRAIN_STATE.id
                << std::dec
                << "\n";

            powertrainEcu.transmitVehicleState(
                canBus,
                currentTimeMs
            );

            nextPowertrainTxMs +=
                powertrainPeriodMs;
        }

        // --------------------------------------------------
        // Steering ECU periodic event
        // --------------------------------------------------

        if (
            currentTimeMs <= simulationDurationMs &&
            currentTimeMs >= nextSteeringTxMs
            )
        {
            std::cout
                << "["
                << currentTimeMs
                << " ms] STEERING ECU request -> 0x"
                << std::hex
                << CanMessageDefinitions::STEERING_STATE.id
                << std::dec
                << "\n";

            steeringEcu.transmitSteeringState(
                canBus,
                currentTimeMs
            );

            nextSteeringTxMs +=
                steeringPeriodMs;
        }

        // --------------------------------------------------
        // CAN bus transmission
        // --------------------------------------------------

        if (
            canBus.hasFrame() &&
            currentTimeMs >= canBus.getBusyUntilMs()
            )
        {
            const CanFrame frame =
                canBus.receive(currentTimeMs);

            dashboardEcu.receiveFrame(
                frame
            );

            // Move simulation directly to the end
            // of the current CAN transmission.

            currentTimeMs =
                canBus.getBusyUntilMs();

            continue;
        }

        // --------------------------------------------------
        // Determine next event
        // --------------------------------------------------

        double nextEventMs =
            std::numeric_limits<double>::infinity();

        // ABS next event

        if (
            nextAbsTxMs <= simulationDurationMs &&
            nextAbsTxMs > currentTimeMs
            )
        {
            nextEventMs =
                std::min(
                    nextEventMs,
                    nextAbsTxMs
                );
        }

        // Powertrain next event

        if (
            nextPowertrainTxMs <= simulationDurationMs &&
            nextPowertrainTxMs > currentTimeMs
            )
        {
            nextEventMs =
                std::min(
                    nextEventMs,
                    nextPowertrainTxMs
                );
        }

        // Steering next event

        if (
            nextSteeringTxMs <= simulationDurationMs &&
            nextSteeringTxMs > currentTimeMs
            )
        {
            nextEventMs =
                std::min(
                    nextEventMs,
                    nextSteeringTxMs
                );
        }

        // CAN transmission completion

        if (
            canBus.getBusyUntilMs() > currentTimeMs
            )
        {
            nextEventMs =
                std::min(
                    nextEventMs,
                    canBus.getBusyUntilMs()
                );
        }

        // If the bus is already free and something is
        // pending, don't advance time.

        if (
            canBus.hasFrame() &&
            canBus.getBusyUntilMs() <= currentTimeMs
            )
        {
            continue;
        }

        // No more scheduled events.

        if (
            nextEventMs ==
            std::numeric_limits<double>::infinity()
            )
        {
            break;
        }

        currentTimeMs = nextEventMs;
    }

    // ==================================================
    // Final dashboard
    // ==================================================

    std::cout << "\n";

    dashboardEcu.displayVehicleState();

    // ==================================================
    // CAN statistics
    // ==================================================

    canBus.printStatistics(
        currentTimeMs
    );

    return 0;
}