#include "VehicleStateWidget.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>

VehicleStateWidget::VehicleStateWidget(
    QWidget* parent)
    : QGroupBox(
        "Vehicle State",
        parent
    )
{
    QGridLayout* layout =
        new QGridLayout(this);

    QLabel* speedLabel =
        new QLabel("Speed");

    QLabel* rpmLabel =
        new QLabel("RPM");

    QLabel* gearLabel =
        new QLabel("Gear");

    QLabel* brakeLabel =
        new QLabel("Brake");

    speedValueLabel =
        new QLabel("0.0 km/h");

    rpmValueLabel =
        new QLabel("0");

    gearValueLabel =
        new QLabel("0");

    brakeValueLabel =
        new QLabel("RELEASED");

    QFont valueFont;

    valueFont.setPointSize(15);
    valueFont.setBold(true);

    speedValueLabel->setFont(valueFont);
    rpmValueLabel->setFont(valueFont);
    gearValueLabel->setFont(valueFont);
    brakeValueLabel->setFont(valueFont);

    layout->addWidget(
        speedLabel,
        0,
        0
    );

    layout->addWidget(
        speedValueLabel,
        0,
        1
    );

    layout->addWidget(
        rpmLabel,
        1,
        0
    );

    layout->addWidget(
        rpmValueLabel,
        1,
        1
    );

    layout->addWidget(
        gearLabel,
        2,
        0
    );

    layout->addWidget(
        gearValueLabel,
        2,
        1
    );

    layout->addWidget(
        brakeLabel,
        3,
        0
    );

    layout->addWidget(
        brakeValueLabel,
        3,
        1
    );
}

void VehicleStateWidget::updateState(
    const VehicleState& state)
{
    speedValueLabel->setText(
        QString::number(
            state.speedKmh,
            'f',
            1
        ) +
        " km/h"
    );

    rpmValueLabel->setText(
        QString::number(
            state.engineRpm
        )
    );

    gearValueLabel->setText(
        QString::number(
            static_cast<int>(
                state.gear
                )
        )
    );

    brakeValueLabel->setText(
        state.brakeApplied
        ? "APPLIED"
        : "RELEASED"
    );
}