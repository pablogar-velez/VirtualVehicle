#pragma once

#include <array>
#include <cstdint>

enum class CanFrameFormat
{
    Standard,
    Extended
};

enum class CanFrameType
{
    Data,
    Remote
};

struct CanFrame
{
    std::uint32_t arbitrationId{};
    CanFrameFormat format{ CanFrameFormat::Standard };
    CanFrameType type{ CanFrameType::Data };
    std::uint8_t dlc{};
    std::array<std::uint8_t, 8> data{};
};