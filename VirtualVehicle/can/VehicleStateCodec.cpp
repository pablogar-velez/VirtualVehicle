#include "VehicleStateCodec.h"
#include "CanMessageDefinitions.h"

CanFrame VehicleStateCodec::encode(const VehicleState& state)
{
    CanFrame frame{};

    frame.arbitrationId =
        CanMessageDefinitions::POWERTRAIN_STATE.id;
    frame.dlc =
        CanMessageDefinitions::POWERTRAIN_STATE.dlc;
    frame.format = CanFrameFormat::Standard;
    frame.type = CanFrameType::Data;

    const std::uint16_t speedRaw =
        static_cast<std::uint16_t>(state.speedKmh * 10.0f);

    frame.data[0] = static_cast<std::uint8_t>(speedRaw & 0xFF);
    frame.data[1] = static_cast<std::uint8_t>((speedRaw >> 8) & 0xFF);

    frame.data[2] = static_cast<std::uint8_t>(state.engineRpm & 0xFF);
    frame.data[3] = static_cast<std::uint8_t>((state.engineRpm >> 8) & 0xFF);

    frame.data[4] = state.gear;
    frame.data[5] = state.brakeApplied ? 1 : 0;

    return frame;
}

VehicleState VehicleStateCodec::decode(const CanFrame& frame)
{
    VehicleState state{};

    const std::uint16_t speedRaw =
        static_cast<std::uint16_t>(frame.data[0]) |
        (static_cast<std::uint16_t>(frame.data[1]) << 8);

    state.speedKmh = static_cast<float>(speedRaw) / 10.0f;

    state.engineRpm =
        static_cast<std::uint16_t>(frame.data[2]) |
        (static_cast<std::uint16_t>(frame.data[3]) << 8);

    state.gear = frame.data[4];
    state.brakeApplied = frame.data[5] != 0;

    return state;
}