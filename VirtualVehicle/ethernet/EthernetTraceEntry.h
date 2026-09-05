#pragma once

#include "EthernetFrame.h"

struct EthernetTraceEntry
{
    EthernetFrame frame{};

    double requestTimeMs{ 0.0 };
    double startTimeMs{ 0.0 };
    double completionTimeMs{ 0.0 };
    double transmissionTimeMs{ 0.0 };
    double waitingTimeMs{ 0.0 };
};

