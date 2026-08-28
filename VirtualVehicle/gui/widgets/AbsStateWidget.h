#pragma once

#include <QGroupBox>

#include "../../VirtualVehicle.h"

class QLabel;

class AbsStateWidget : public QGroupBox
{
public:
    explicit AbsStateWidget(
        QWidget* parent = nullptr
    );

    void updateState(
        const AbsState& state
    );

private:
    QLabel* frontLeftWheelValueLabel{};
    QLabel* frontRightWheelValueLabel{};
    QLabel* absActiveValueLabel{};
    QLabel* absHealthValueLabel{};
};