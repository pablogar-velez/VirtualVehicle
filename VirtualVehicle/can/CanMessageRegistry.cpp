#include "CanMessageRegistry.h"

#include "CanMessageDefinitions.h"

std::string CanMessageRegistry::getMessageName(
    std::uint32_t arbitrationId)
{
    switch (arbitrationId)
    {
    case CanMessageDefinitions::ABS_WHEEL_STATE.id:
        return "ABS_WHEEL_STATE";

    case CanMessageDefinitions::POWERTRAIN_STATE.id:
        return "POWERTRAIN_STATE";

    case CanMessageDefinitions::STEERING_STATE.id:
        return "STEERING_STATE";

    default:
        return "UNKNOWN_MESSAGE";
    }
}