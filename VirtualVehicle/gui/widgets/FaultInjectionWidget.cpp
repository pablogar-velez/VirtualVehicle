#include "FaultInjectionWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

FaultInjectionWidget::FaultInjectionWidget(
    QWidget* parent)
    : QGroupBox(
        "Fault Injection",
        parent
    )
{
    QGridLayout* layout =
        new QGridLayout(this);

    // ==================================================
    // Front Left
    // ==================================================

    QLabel* frontLeftLabel =
        new QLabel(
            "Front Left Wheel Sensor"
        );

    frontLeftDropoutButton =
        new QPushButton(
            "Inject Dropout"
        );

    clearFrontLeftFaultButton =
        new QPushButton(
            "Clear Fault"
        );

    // ==================================================
    // Front Right
    // ==================================================

    QLabel* frontRightLabel =
        new QLabel(
            "Front Right Wheel Sensor"
        );

    frontRightDropoutButton =
        new QPushButton(
            "Inject Dropout"
        );

    clearFrontRightFaultButton =
        new QPushButton(
            "Clear Fault"
        );

    // ==================================================
    // Layout
    // ==================================================

    layout->addWidget(
        frontLeftLabel,
        0,
        0
    );

    layout->addWidget(
        frontRightLabel,
        0,
        1
    );

    layout->addWidget(
        frontLeftDropoutButton,
        1,
        0
    );

    layout->addWidget(
        frontRightDropoutButton,
        1,
        1
    );

    layout->addWidget(
        clearFrontLeftFaultButton,
        2,
        0
    );

    layout->addWidget(
        clearFrontRightFaultButton,
        2,
        1
    );
}

QPushButton*
FaultInjectionWidget::getFrontLeftDropoutButton() const
{
    return frontLeftDropoutButton;
}

QPushButton*
FaultInjectionWidget::getFrontRightDropoutButton() const
{
    return frontRightDropoutButton;
}

QPushButton*
FaultInjectionWidget::getClearFrontLeftFaultButton() const
{
    return clearFrontLeftFaultButton;
}

QPushButton*
FaultInjectionWidget::getClearFrontRightFaultButton() const
{
    return clearFrontRightFaultButton;
}