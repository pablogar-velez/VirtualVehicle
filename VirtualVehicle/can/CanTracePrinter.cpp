#include "CanTracePrinter.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "CanMessageRegistry.h"

void CanTracePrinter::printLast(
    const std::vector<CanTraceEntry>& trace,
    std::size_t count)
{
    std::cout << "\n";
    std::cout << "CAN TRACE\n";
    std::cout << "=========\n\n";

    if (trace.empty())
    {
        std::cout << "No CAN frames recorded.\n";
        return;
    }

    const std::size_t startIndex =
        trace.size() > count
        ? trace.size() - count
        : 0;

    std::cout
        << std::left
        << std::setw(12) << "TX Start"
        << std::setw(10) << "CAN ID"
        << std::setw(24) << "Message"
        << std::setw(8) << "DLC"
        << std::setw(14) << "Wait"
        << "Data"
        << "\n";

    std::cout
        << "------------------------------------------------------------------------------------------\n";

    for (
        std::size_t i = startIndex;
        i < trace.size();
        ++i
        )
    {
        const CanTraceEntry& entry =
            trace[i];

        const std::string messageName =
            CanMessageRegistry::getMessageName(
                entry.arbitrationId
            );

        // TX start
        std::cout
            << std::fixed
            << std::setprecision(3)
            << std::setw(12)
            << entry.txStartTimeMs;

        // CAN ID
        std::cout
            << "0x"
            << std::hex
            << std::uppercase
            << std::setw(8)
            << entry.arbitrationId
            << std::dec
            << std::nouppercase;

        // Message name
        std::cout
            << std::setw(24)
            << messageName;

        // DLC
        std::cout
            << std::setw(8)
            << static_cast<int>(
                entry.dlc
                );

        // Wait time
        std::cout
            << std::setw(14)
            << entry.waitingTimeMs;

        // Data bytes
        for (
            std::size_t byteIndex = 0;
            byteIndex < entry.dlc;
            ++byteIndex
            )
        {
            std::cout
                << std::hex
                << std::uppercase
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(
                    entry.data[byteIndex]
                    )
                << " ";

            std::cout
                << std::setfill(' ');
        }

        std::cout
            << std::dec
            << std::nouppercase
            << "\n";
    }
}