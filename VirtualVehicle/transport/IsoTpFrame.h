#pragma once

#include <cstdint>
#include <string>

enum class IsoTpFrameType : std::uint8_t
{
    SingleFrame = 0x0,
    FirstFrame = 0x1,
    ConsecutiveFrame = 0x2,
    FlowControl = 0x3,
    Unknown = 0xFF
};

enum class IsoTpFlowStatus : std::uint8_t
{
    ContinueToSend = 0x0,
    Wait = 0x1,
    Overflow = 0x2
};

struct IsoTpFrameInfo
{
    bool valid{ false };

    IsoTpFrameType type{
        IsoTpFrameType::Unknown
    };

    std::uint16_t payloadLength{ 0 };

    std::uint8_t sequenceNumber{ 0 };

    IsoTpFlowStatus flowStatus{
        IsoTpFlowStatus::ContinueToSend
    };

    std::uint8_t blockSize{ 0 };

    std::uint8_t separationTimeMinimum{ 0 };

    std::string error;
};