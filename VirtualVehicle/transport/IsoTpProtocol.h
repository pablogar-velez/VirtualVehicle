#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "../can/CanFrame.h"
#include "IsoTpFrame.h"

struct IsoTpReassemblyResult
{
    bool success{ false };

    std::vector<std::uint8_t> payload;

    std::string error;
};

class IsoTpProtocol
{
public:
    static constexpr std::size_t
        ClassicCanPayloadSize = 8;

    static constexpr std::size_t
        SingleFrameMaximumPayload = 7;

    static constexpr std::size_t
        FirstFramePayloadCapacity = 6;

    static constexpr std::size_t
        ConsecutiveFramePayloadCapacity = 7;

    static constexpr std::size_t
        MaximumPayloadLength = 4095;

    static std::vector<CanFrame> segment(
        std::uint32_t arbitrationId,
        const std::vector<std::uint8_t>& payload
    );

    static IsoTpReassemblyResult reassemble(
        const std::vector<CanFrame>& frames
    );

    static CanFrame createFlowControlFrame(
        std::uint32_t arbitrationId,
        IsoTpFlowStatus flowStatus =
        IsoTpFlowStatus::ContinueToSend,
        std::uint8_t blockSize = 0,
        std::uint8_t separationTimeMinimum = 0
    );

    static IsoTpFrameInfo parseFrame(
        const CanFrame& frame
    );

private:
    static void validateArbitrationId(
        std::uint32_t arbitrationId
    );

    static CanFrame createBaseFrame(
        std::uint32_t arbitrationId
    );
};