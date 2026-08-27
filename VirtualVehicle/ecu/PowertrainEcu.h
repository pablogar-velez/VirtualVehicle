#pragma once

#include "../VirtualVehicle.h"
#include "../can/VirtualCanBus.h"

class PowertrainEcu
{
public:
    PowertrainEcu();

    void setVehicleState(const VehicleState& state);

    void transmitVehicleState(
        VirtualCanBus& bus,
        double requestTimeMs
    );

private:
    VehicleState vehicleState;
};