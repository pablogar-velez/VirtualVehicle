#pragma once

#include "../VirtualVehicle.h"
#include "../can/VirtualCanBus.h"

class SteeringEcu
{
public:
    void setSteeringState(
        const SteeringState& state
    );

    void transmitSteeringState(
        VirtualCanBus& bus,
        double requestTimeMs
    );

private:
    SteeringState steeringState{};
};