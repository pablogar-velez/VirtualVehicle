#pragma once

#include <cstdint>

class CanArbitration
{
public:
    static std::uint32_t selectWinner(
        std::uint32_t idA,
        std::uint32_t idB
    );

    static void printComparison(
        std::uint32_t idA,
        std::uint32_t idB
    );
};