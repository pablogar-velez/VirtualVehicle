#pragma once

#include <QGroupBox>

class QPushButton;

class ScenarioControlWidget : public QGroupBox
{
public:
    explicit ScenarioControlWidget(
        QWidget* parent = nullptr
    );

    QPushButton* getAccelerationButton() const;
    QPushButton* getCruiseButton() const;
    QPushButton* getHardBrakingButton() const;
    QPushButton* getRecoveryButton() const;

private:
    QPushButton* accelerationButton{};
    QPushButton* cruiseButton{};
    QPushButton* hardBrakingButton{};
    QPushButton* recoveryButton{};
};