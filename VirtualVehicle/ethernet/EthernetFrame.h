#pragma once

#include <cstdint>
#include <vector>

#include "MacAddress.h"

struct EthernetFrame
{
    MacAddress sourceMac{};
    MacAddress destinationMac{};

    std::uint16_t etherType{};

    std::vector<std::uint8_t>
        payload;

    bool operator==(
        const EthernetFrame& other) const
    {
        return
            sourceMac ==
            other.sourceMac
            &&
            destinationMac ==
            other.destinationMac
            &&
            etherType ==
            other.etherType
            &&
            payload ==
            other.payload;
    }

    bool operator!=(
        const EthernetFrame& other) const
    {
        return
            !(
                *this ==
                other
                );
    }
};