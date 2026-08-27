#pragma once

#include <string>
#include <vector>

struct VehicleEvent
{
    double timeMs{};
    std::string category;
    std::string message;
};

class EventLogger
{
public:
    void log(
        double timeMs,
        const std::string& category,
        const std::string& message
    );

    void printLog() const;

    const std::vector<VehicleEvent>&
        getEvents() const;

private:
    std::vector<VehicleEvent> events;
};