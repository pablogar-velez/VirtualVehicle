#include "SteeringStateCodec.h"

#include "CanMessageDefinitions.h"

CanFrame SteeringStateCodec::encode(
    const SteeringState& state)
{
    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::STEERING_STATE.id;

    frame.dlc =
        CanMessageDefinitions::STEERING_STATE.dlc;

    frame.format = CanFrameFormat::Standard;
    frame.type = CanFrameType::Data;

    const std::int16_t angleRaw =
        static_cast<std::int16_t>(
            state.steeringAngleDeg * 10.0f
            );

    const std::int16_t torqueRaw =
        static_cast<std::int16_t>(
            state.steeringTorqueNm * 100.0f
            );

    frame.data[0] =
        static_cast<std::uint8_t>(
            angleRaw & 0xFF
            );

    frame.data[1] =
        static_cast<std::uint8_t>(
            (angleRaw >> 8) & 0xFF
            );

    frame.data[2] =
        static_cast<std::uint8_t>(
            torqueRaw & 0xFF
            );

    frame.data[3] =
        static_cast<std::uint8_t>(
            (torqueRaw >> 8) & 0xFF
            );

    return frame;
}

SteeringState SteeringStateCodec::decode(
    const CanFrame& frame)
{
    SteeringState state{};

    const std::int16_t angleRaw =
        static_cast<std::int16_t>(
            static_cast<std::uint16_t>(
                frame.data[0]
                ) |
            (
                static_cast<std::uint16_t>(
                    frame.data[1]
                    ) << 8
                )
            );

    const std::int16_t torqueRaw =
        static_cast<std::int16_t>(
            static_cast<std::uint16_t>(
                frame.data[2]
                ) |
            (
                static_cast<std::uint16_t>(
                    frame.data[3]
                    ) << 8
                )
            );

    state.steeringAngleDeg =
        static_cast<float>(
            angleRaw
            ) / 10.0f;

    state.steeringTorqueNm =
        static_cast<float>(
            torqueRaw
            ) / 100.0f;

    return state;
}