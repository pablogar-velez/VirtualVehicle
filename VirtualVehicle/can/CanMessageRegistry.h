#pragma once

#include <cstdint>
#include <string>

class CanMessageRegistry
{
public:
    static std::string getMessageName(
        std::uint32_t arbitrationId
    );
};