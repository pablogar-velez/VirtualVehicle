#pragma once

#include <QGroupBox>

#include "../../VirtualVehicle.h"

class QLabel;

class VehicleStateWidget : public QGroupBox
{
public:
    explicit VehicleStateWidget(
        QWidget* parent = nullptr
    );

    void updateState(
        const VehicleState& state
    );

private:
    QLabel* speedValueLabel{};
    QLabel* rpmValueLabel{};
    QLabel* gearValueLabel{};
    QLabel* brakeValueLabel{};
};