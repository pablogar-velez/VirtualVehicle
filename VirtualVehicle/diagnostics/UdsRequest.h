#pragma once

#include <cstdint>
#include <vector>

struct UdsRequest
{
    std::uint8_t serviceId{};
    std::vector<std::uint8_t> payload;
};