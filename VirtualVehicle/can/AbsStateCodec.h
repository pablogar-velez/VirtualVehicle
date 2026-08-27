#pragma once

#include "../VirtualVehicle.h"
#include "CanFrame.h"

class AbsStateCodec
{
public:
    static CanFrame encode(const AbsState& state);
    static AbsState decode(const CanFrame& frame);
};