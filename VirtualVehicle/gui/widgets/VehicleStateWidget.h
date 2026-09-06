#pragma once

#include <QGroupBox>

#include "../../VirtualVehicle.h"
#include "AutomotiveGaugeWidget.h"

class QLabel;
class GearIndicatorWidget;

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
    QLabel* brakeValueLabel{};
    GearIndicatorWidget* gearIndicatorWidget{};

    AutomotiveGaugeWidget* speedGauge{};
    AutomotiveGaugeWidget* rpmGauge{};

    void updateBrakeStyle(
        bool brakeApplied
    );
};
