#pragma once

#include <cstdint>

#include <QGroupBox>

#include "../../can/CanStatistics.h"

class QLabel;
class QTableWidget;

class StatisticsWidget : public QGroupBox
{
public:
    explicit StatisticsWidget(
        QWidget* parent = nullptr
    );

    void updateStatistics(
        const CanStatistics& statistics,
        std::uint32_t bitrate,
        double simulationTimeMs
    );

private:
    QLabel* bitrateValueLabel{};
    QLabel* simulationTimeValueLabel{};

    QLabel* framesValueLabel{};
    QLabel* arbitrationsValueLabel{};

    QLabel* utilizationValueLabel{};

    QLabel* averageWaitValueLabel{};
    QLabel* maximumWaitValueLabel{};

    QTableWidget* messageTable{};

    void updateMessageStatistics(
        const CanStatistics& statistics
    );
};