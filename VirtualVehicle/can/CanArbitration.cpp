#include "CanArbitration.h"

#include <bitset>
#include <iostream>

std::uint32_t CanArbitration::selectWinner(
    std::uint32_t idA,
    std::uint32_t idB)
{
    for (int bit = 10; bit >= 0; --bit)
    {
        const bool bitA = (idA >> bit) & 0x1;
        const bool bitB = (idB >> bit) & 0x1;

        if (bitA == bitB)
        {
            continue;
        }

        // CAN:
        // 0 = dominant
        // 1 = recessive

        if (!bitA && bitB)
        {
            return idA;
        }

        if (bitA && !bitB)
        {
            return idB;
        }
    }

    return idA;
}

void CanArbitration::printComparison(
    std::uint32_t idA,
    std::uint32_t idB)
{
    std::bitset<11> bitsA(idA);
    std::bitset<11> bitsB(idB);

    std::cout << "[CAN ARBITRATION]\n";
    std::cout << "-----------------\n";

    std::cout << "ID 0x"
        << std::hex
        << idA
        << std::dec
        << " -> "
        << bitsA
        << "\n";

    std::cout << "ID 0x"
        << std::hex
        << idB
        << std::dec
        << " -> "
        << bitsB
        << "\n\n";

    for (int bit = 10; bit >= 0; --bit)
    {
        const bool bitA = (idA >> bit) & 0x1;
        const bool bitB = (idB >> bit) & 0x1;

        std::cout << "Bit "
            << (10 - bit)
            << ": "
            << bitA
            << " vs "
            << bitB;

        if (bitA == bitB)
        {
            std::cout << " -> equal\n";
            continue;
        }

        if (!bitA && bitB)
        {
            std::cout
                << " -> ID 0x"
                << std::hex
                << idA
                << std::dec
                << " wins (0 dominant)\n";

            break;
        }

        if (bitA && !bitB)
        {
            std::cout
                << " -> ID 0x"
                << std::hex
                << idB
                << std::dec
                << " wins (0 dominant)\n";

            break;
        }
    }

    const std::uint32_t winner =
        selectWinner(idA, idB);

    std::cout << "\nWinner: 0x"
        << std::hex
        << winner
        << std::dec
        << "\n\n";
}