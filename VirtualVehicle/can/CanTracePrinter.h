#pragma once

#include <cstddef>
#include <vector>

#include "CanTraceEntry.h"

class CanTracePrinter
{
public:
    static void printLast(
        const std::vector<CanTraceEntry>& trace,
        std::size_t count
    );
};