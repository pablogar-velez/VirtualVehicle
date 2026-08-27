#pragma once

#include "../VirtualVehicle.h"
#include "CanFrame.h"

class VehicleStateCodec
{
public:
    static CanFrame encode(const VehicleState& state);
    static VehicleState decode(const CanFrame& frame);
};