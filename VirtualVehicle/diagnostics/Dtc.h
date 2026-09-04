#pragma once

#include <cstdint>
#include <string>

#include "DtcStatus.h"

struct Dtc
{
    std::uint32_t code{};
    std::string name;
    std::string description;

    DtcStatus status{ DtcStatus::Inactive };

    double firstDetectedTimeMs{ 0.0 };
    double lastDetectedTimeMs{ 0.0 };

    std::uint32_t occurrenceCount{ 0 };

    bool isActive() const
    {
        return status == DtcStatus::Active;
    }
};