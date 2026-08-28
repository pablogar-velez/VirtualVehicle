#pragma once

#include <cstddef>
#include <vector>

#include <QGroupBox>

#include "../../simulation/EventLogger.h"

class QTableWidget;

class EventLogWidget : public QGroupBox
{
public:
    explicit EventLogWidget(
        QWidget* parent = nullptr
    );

    void updateEvents(
        const std::vector<VehicleEvent>& events
    );

    void clear();

private:
    QTableWidget* eventTable{};

    std::size_t displayedEventCount{ 0 };
};