#pragma once

#include "../VirtualVehicle.h"
#include "CanFrame.h"

class SteeringStateCodec
{
public:
    static CanFrame encode(const SteeringState& state);
    static SteeringState decode(const CanFrame& frame);
};