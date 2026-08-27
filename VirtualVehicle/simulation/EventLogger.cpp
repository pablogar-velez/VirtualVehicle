#include "EventLogger.h"

#include <iomanip>
#include <iostream>

void EventLogger::log(
    double timeMs,
    const std::string& category,
    const std::string& message)
{
    events.push_back(
        VehicleEvent{
            timeMs,
            category,
            message
        }
    );
}

void EventLogger::printLog() const
{
    std::cout << "\n";
    std::cout << "VEHICLE EVENT LOG\n";
    std::cout << "=================\n\n";

    for (const VehicleEvent& event : events)
    {
        std::cout
            << "["
            << std::fixed
            << std::setprecision(3)
            << std::setw(8)
            << event.timeMs
            << " ms] "

            << std::left
            << std::setw(12)
            << event.category

            << event.message
            << "\n";
    }
}

const std::vector<VehicleEvent>&
EventLogger::getEvents() const
{
    return events;
}