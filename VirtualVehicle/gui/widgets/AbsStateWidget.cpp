#include "AbsStateWidget.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>

AbsStateWidget::AbsStateWidget(
    QWidget* parent)
    : QGroupBox(
        "ABS / Wheel State",
        parent
    )
{
    QGridLayout* layout =
        new QGridLayout(this);

    QLabel* frontLeftLabel =
        new QLabel(
            "Front Left Wheel"
        );

    QLabel* frontRightLabel =
        new QLabel(
            "Front Right Wheel"
        );

    QLabel* absActiveLabel =
        new QLabel(
            "ABS"
        );

    QLabel* absHealthLabel =
        new QLabel(
            "Health"
        );

    frontLeftWheelValueLabel =
        new QLabel(
            "0.0 km/h"
        );

    frontRightWheelValueLabel =
        new QLabel(
            "0.0 km/h"
        );

    absActiveValueLabel =
        new QLabel(
            "INACTIVE"
        );

    absHealthValueLabel =
        new QLabel(
            "HEALTHY"
        );

    QFont valueFont;

    valueFont.setPointSize(15);
    valueFont.setBold(true);

    frontLeftWheelValueLabel->setFont(
        valueFont
    );

    frontRightWheelValueLabel->setFont(
        valueFont
    );

    absActiveValueLabel->setFont(
        valueFont
    );

    absHealthValueLabel->setFont(
        valueFont
    );

    layout->addWidget(
        frontLeftLabel,
        0,
        0
    );

    layout->addWidget(
        frontLeftWheelValueLabel,
        0,
        1
    );

    layout->addWidget(
        frontRightLabel,
        1,
        0
    );

    layout->addWidget(
        frontRightWheelValueLabel,
        1,
        1
    );

    layout->addWidget(
        absActiveLabel,
        2,
        0
    );

    layout->addWidget(
        absActiveValueLabel,
        2,
        1
    );

    layout->addWidget(
        absHealthLabel,
        3,
        0
    );

    layout->addWidget(
        absHealthValueLabel,
        3,
        1
    );
}

void AbsStateWidget::updateState(
    const AbsState& state)
{
    if (
        state.frontLeftSensorValid
        )
    {
        frontLeftWheelValueLabel->setText(
            QString::number(
                state.wheelSpeedFrontLeft,
                'f',
                1
            ) +
            " km/h"
        );
    }
    else
    {
        frontLeftWheelValueLabel->setText(
            "INVALID"
        );
    }

    if (
        state.frontRightSensorValid
        )
    {
        frontRightWheelValueLabel->setText(
            QString::number(
                state.wheelSpeedFrontRight,
                'f',
                1
            ) +
            " km/h"
        );
    }
    else
    {
        frontRightWheelValueLabel->setText(
            "INVALID"
        );
    }

    absActiveValueLabel->setText(
        state.absActive
        ? "ACTIVE"
        : "INACTIVE"
    );

    absHealthValueLabel->setText(
        state.healthStatus ==
        AbsHealthStatus::Healthy
        ? "HEALTHY"
        : "DEGRADED"
    );
}