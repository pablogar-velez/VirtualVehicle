#pragma once

#include <QGroupBox>

class QPushButton;

class FaultInjectionWidget : public QGroupBox
{
public:
    explicit FaultInjectionWidget(
        QWidget* parent = nullptr
    );

    QPushButton* getFrontLeftDropoutButton() const;
    QPushButton* getFrontRightDropoutButton() const;

    QPushButton* getClearFrontLeftFaultButton() const;
    QPushButton* getClearFrontRightFaultButton() const;

private:
    QPushButton* frontLeftDropoutButton{};
    QPushButton* frontRightDropoutButton{};

    QPushButton* clearFrontLeftFaultButton{};
    QPushButton* clearFrontRightFaultButton{};
};