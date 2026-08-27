#pragma once

#include "CanFrame.h"

struct PendingCanFrame
{
    CanFrame frame{};
    double requestTimeMs{};
};