#pragma once

#include <QGroupBox>

#include "../../VirtualVehicle.h"

class QLabel;
class VehicleDiagramWidget;

class AbsStateWidget : public QGroupBox
{
public:
    explicit AbsStateWidget(
        QWidget* parent = nullptr
    );

    void updateState(
        const AbsState& state,
        bool brakeApplied,
        float throttlePercent,
        bool coasting,
        float vehicleSpeedKmh
    );

private:
    VehicleDiagramWidget* vehicleDiagramWidget{};

    QLabel* frontLeftWheelValueLabel{};
    QLabel* frontRightWheelValueLabel{};
    QLabel* rearLeftWheelValueLabel{};
    QLabel* rearRightWheelValueLabel{};

    QLabel* absActiveValueLabel{};
    QLabel* absHealthValueLabel{};

    void updateStatusStyle(
        QLabel* label,
        const char* status
    );
};
