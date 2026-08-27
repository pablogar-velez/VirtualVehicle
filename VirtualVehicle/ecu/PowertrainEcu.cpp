#include "PowertrainEcu.h"

#include "../can/VehicleStateCodec.h"

PowertrainEcu::PowertrainEcu()
    : vehicleState{}
{
}

void PowertrainEcu::setVehicleState(const VehicleState& state)
{
    vehicleState = state;
}

void PowertrainEcu::transmitVehicleState(
    VirtualCanBus& bus,
    double requestTimeMs)
{
    const CanFrame frame =
        VehicleStateCodec::encode(vehicleState);

    bus.transmit(
        frame,
        requestTimeMs
    );
}