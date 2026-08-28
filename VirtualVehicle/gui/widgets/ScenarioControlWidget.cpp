#include "ScenarioControlWidget.h"

#include <QHBoxLayout>
#include <QPushButton>

ScenarioControlWidget::ScenarioControlWidget(
    QWidget* parent)
    : QGroupBox(
        "Scenario Control",
        parent
    )
{
    QHBoxLayout* layout =
        new QHBoxLayout(this);

    accelerationButton =
        new QPushButton(
            "Acceleration"
        );

    cruiseButton =
        new QPushButton(
            "Cruise"
        );

    hardBrakingButton =
        new QPushButton(
            "Hard Braking"
        );

    recoveryButton =
        new QPushButton(
            "Recovery"
        );

    layout->addWidget(
        accelerationButton
    );

    layout->addWidget(
        cruiseButton
    );

    layout->addWidget(
        hardBrakingButton
    );

    layout->addWidget(
        recoveryButton
    );
}

QPushButton*
ScenarioControlWidget::getAccelerationButton() const
{
    return accelerationButton;
}

QPushButton*
ScenarioControlWidget::getCruiseButton() const
{
    return cruiseButton;
}

QPushButton*
ScenarioControlWidget::getHardBrakingButton() const
{
    return hardBrakingButton;
}

QPushButton*
ScenarioControlWidget::getRecoveryButton() const
{
    return recoveryButton;
}