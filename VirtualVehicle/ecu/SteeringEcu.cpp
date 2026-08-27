#include "SteeringEcu.h"

#include "../can/SteeringStateCodec.h"

void SteeringEcu::setSteeringState(
    const SteeringState& state)
{
    steeringState = state;
}

void SteeringEcu::transmitSteeringState(
    VirtualCanBus& bus,
    double requestTimeMs)
{
    const CanFrame frame =
        SteeringStateCodec::encode(
            steeringState
        );

    bus.transmit(
        frame,
        requestTimeMs
    );
}