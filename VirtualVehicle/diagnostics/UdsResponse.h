#pragma once

#include <cstdint>
#include <vector>

struct UdsResponse
{
    bool positive{ false };

    std::uint8_t serviceId{};
    std::vector<std::uint8_t> payload;

    bool isPositive() const
    {
        return positive;
    }

    bool isNegative() const
    {
        return !positive;
    }
};