#pragma once

#include "../VirtualVehicle.h"
#include "../can/CanFrame.h"

class DashboardEcu
{
public:
    void receiveFrame(const CanFrame& frame);

    void displayVehicleState() const;

private:
    VehicleState vehicleState{};
    AbsState absState{};
    SteeringState steeringState{};
};