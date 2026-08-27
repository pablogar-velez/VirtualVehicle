#include "AbsStateCodec.h"
#include "CanMessageDefinitions.h"

CanFrame AbsStateCodec::encode(const AbsState& state)
{
    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::ABS_WHEEL_STATE.id;

    frame.format = CanFrameFormat::Standard;
    frame.type = CanFrameType::Data;

    frame.dlc =
        CanMessageDefinitions::ABS_WHEEL_STATE.dlc;

    // ==================================================
    // Wheel speeds
    // ==================================================

    const std::uint16_t frontLeftRaw =
        static_cast<std::uint16_t>(
            state.wheelSpeedFrontLeft * 10.0f
            );

    const std::uint16_t frontRightRaw =
        static_cast<std::uint16_t>(
            state.wheelSpeedFrontRight * 10.0f
            );

    frame.data[0] =
        static_cast<std::uint8_t>(
            frontLeftRaw & 0xFF
            );

    frame.data[1] =
        static_cast<std::uint8_t>(
            (frontLeftRaw >> 8) & 0xFF
            );

    frame.data[2] =
        static_cast<std::uint8_t>(
            frontRightRaw & 0xFF
            );

    frame.data[3] =
        static_cast<std::uint8_t>(
            (frontRightRaw >> 8) & 0xFF
            );

    // ==================================================
    // Status byte
    // ==================================================
    //
    // Bit 0 = ABS active
    // Bit 1 = Front Left Sensor valid
    // Bit 2 = Front Right Sensor valid
    // Bit 3 = ABS degraded
    //

    std::uint8_t statusByte = 0;

    if (state.absActive)
    {
        statusByte |= (1 << 0);
    }

    if (state.frontLeftSensorValid)
    {
        statusByte |= (1 << 1);
    }

    if (state.frontRightSensorValid)
    {
        statusByte |= (1 << 2);
    }

    if (
        state.healthStatus ==
        AbsHealthStatus::Degraded
        )
    {
        statusByte |= (1 << 3);
    }

    frame.data[4] = statusByte;

    return frame;
}

AbsState AbsStateCodec::decode(const CanFrame& frame)
{
    AbsState state{};

    // ==================================================
    // Wheel speeds
    // ==================================================

    const std::uint16_t frontLeftRaw =
        static_cast<std::uint16_t>(
            frame.data[0]
            ) |
        (
            static_cast<std::uint16_t>(
                frame.data[1]
                ) << 8
            );

    const std::uint16_t frontRightRaw =
        static_cast<std::uint16_t>(
            frame.data[2]
            ) |
        (
            static_cast<std::uint16_t>(
                frame.data[3]
                ) << 8
            );

    state.wheelSpeedFrontLeft =
        static_cast<float>(
            frontLeftRaw
            ) / 10.0f;

    state.wheelSpeedFrontRight =
        static_cast<float>(
            frontRightRaw
            ) / 10.0f;

    // ==================================================
    // Status byte
    // ==================================================

    const std::uint8_t statusByte =
        frame.data[4];

    state.absActive =
        (statusByte & (1 << 0)) != 0;

    state.frontLeftSensorValid =
        (statusByte & (1 << 1)) != 0;

    state.frontRightSensorValid =
        (statusByte & (1 << 2)) != 0;

    const bool degraded =
        (statusByte & (1 << 3)) != 0;

    state.healthStatus =
        degraded
        ? AbsHealthStatus::Degraded
        : AbsHealthStatus::Healthy;

    return state;
}