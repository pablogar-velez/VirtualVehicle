#include "IsoTpProtocol.h"

#include <stdexcept>

namespace
{
    constexpr std::uint8_t FrameTypeMask =
        0xF0;

    constexpr std::uint8_t LowNibbleMask =
        0x0F;

    constexpr std::uint8_t SingleFramePci =
        0x00;

    constexpr std::uint8_t FirstFramePci =
        0x10;

    constexpr std::uint8_t ConsecutiveFramePci =
        0x20;

    constexpr std::uint8_t FlowControlPci =
        0x30;
}

std::vector<CanFrame> IsoTpProtocol::segment(
    std::uint32_t arbitrationId,
    const std::vector<std::uint8_t>& payload)
{
    validateArbitrationId(
        arbitrationId
    );

    if (payload.empty())
    {
        throw std::invalid_argument(
            "ISO-TP payload must not be empty."
        );
    }

    if (
        payload.size() >
        MaximumPayloadLength
        )
    {
        throw std::invalid_argument(
            "ISO-TP payload exceeds 4095 bytes."
        );
    }

    std::vector<CanFrame> frames;

    // ========================================================
    // Single Frame
    // ========================================================

    if (
        payload.size() <=
        SingleFrameMaximumPayload
        )
    {
        CanFrame frame =
            createBaseFrame(
                arbitrationId
            );

        frame.data[0] =
            static_cast<std::uint8_t>(
                SingleFramePci |
                static_cast<std::uint8_t>(
                    payload.size()
                    )
                );

        for (
            std::size_t index = 0;
            index < payload.size();
            ++index
            )
        {
            frame.data[index + 1] =
                payload[index];
        }

        frames.push_back(
            frame
        );

        return frames;
    }

    // ========================================================
    // First Frame
    // ========================================================

    CanFrame firstFrame =
        createBaseFrame(
            arbitrationId
        );

    const std::uint16_t totalLength =
        static_cast<std::uint16_t>(
            payload.size()
            );

    firstFrame.data[0] =
        static_cast<std::uint8_t>(
            FirstFramePci |
            static_cast<std::uint8_t>(
                (totalLength >> 8) &
                LowNibbleMask
                )
            );

    firstFrame.data[1] =
        static_cast<std::uint8_t>(
            totalLength & 0xFF
            );

    std::size_t payloadIndex = 0;

    for (
        std::size_t frameIndex = 0;
        frameIndex <
        FirstFramePayloadCapacity;
        ++frameIndex
        )
    {
        firstFrame.data[
            frameIndex + 2
        ] =
            payload[payloadIndex];

            ++payloadIndex;
    }

    frames.push_back(
        firstFrame
    );

    // ========================================================
    // Consecutive Frames
    // ========================================================

    std::uint8_t sequenceNumber =
        0x01;

    while (
        payloadIndex <
        payload.size()
        )
    {
        CanFrame consecutiveFrame =
            createBaseFrame(
                arbitrationId
            );

        consecutiveFrame.data[0] =
            static_cast<std::uint8_t>(
                ConsecutiveFramePci |
                (
                    sequenceNumber &
                    LowNibbleMask
                    )
                );

        std::size_t dataIndex = 1;

        while (
            dataIndex <
            ClassicCanPayloadSize &&
            payloadIndex <
            payload.size()
            )
        {
            consecutiveFrame.data[
                dataIndex
            ] =
                payload[payloadIndex];

                ++dataIndex;
                ++payloadIndex;
        }

        frames.push_back(
            consecutiveFrame
        );

        sequenceNumber =
            static_cast<std::uint8_t>(
                (
                    sequenceNumber + 1
                    ) &
                LowNibbleMask
                );
    }

    return frames;
}

IsoTpReassemblyResult IsoTpProtocol::reassemble(
    const std::vector<CanFrame>& frames)
{
    IsoTpReassemblyResult result;

    if (frames.empty())
    {
        result.error =
            "No ISO-TP frames were provided.";

        return result;
    }

    const std::uint32_t arbitrationId =
        frames.front().arbitrationId;

    for (const CanFrame& frame : frames)
    {
        if (
            frame.arbitrationId !=
            arbitrationId
            )
        {
            result.error =
                "ISO-TP frame arbitration IDs do not match.";

            return result;
        }
    }

    const IsoTpFrameInfo firstInfo =
        parseFrame(
            frames.front()
        );

    if (!firstInfo.valid)
    {
        result.error =
            firstInfo.error;

        return result;
    }

    // ========================================================
    // Single Frame
    // ========================================================

    if (
        firstInfo.type ==
        IsoTpFrameType::SingleFrame
        )
    {
        if (frames.size() != 1)
        {
            result.error =
                "Single Frame message contains unexpected additional frames.";

            return result;
        }

        result.payload.reserve(
            firstInfo.payloadLength
        );

        for (
            std::size_t index = 0;
            index <
            firstInfo.payloadLength;
            ++index
            )
        {
            result.payload.push_back(
                frames.front()
                .data[index + 1]
            );
        }

        result.success = true;

        return result;
    }

    // ========================================================
    // First Frame validation
    // ========================================================

    if (
        firstInfo.type !=
        IsoTpFrameType::FirstFrame
        )
    {
        result.error =
            "ISO-TP message must begin with Single Frame or First Frame.";

        return result;
    }

    const std::size_t expectedPayloadLength =
        firstInfo.payloadLength;

    if (
        expectedPayloadLength <=
        SingleFrameMaximumPayload
        )
    {
        result.error =
            "First Frame declares an invalid payload length.";

        return result;
    }

    result.payload.reserve(
        expectedPayloadLength
    );

    for (
        std::size_t index = 0;
        index <
        FirstFramePayloadCapacity &&
        result.payload.size() <
        expectedPayloadLength;
        ++index
        )
    {
        result.payload.push_back(
            frames.front()
            .data[index + 2]
        );
    }

    // ========================================================
    // Consecutive Frames
    // ========================================================

    std::uint8_t expectedSequenceNumber =
        0x01;

    for (
        std::size_t frameIndex = 1;
        frameIndex < frames.size();
        ++frameIndex
        )
    {
        if (
            result.payload.size() >=
            expectedPayloadLength
            )
        {
            result.error =
                "Unexpected additional ISO-TP frames.";

            result.payload.clear();

            return result;
        }

        const IsoTpFrameInfo frameInfo =
            parseFrame(
                frames[frameIndex]
            );

        if (!frameInfo.valid)
        {
            result.error =
                frameInfo.error;

            result.payload.clear();

            return result;
        }

        if (
            frameInfo.type !=
            IsoTpFrameType::ConsecutiveFrame
            )
        {
            result.error =
                "Expected ISO-TP Consecutive Frame.";

            result.payload.clear();

            return result;
        }

        if (
            frameInfo.sequenceNumber !=
            expectedSequenceNumber
            )
        {
            result.error =
                "ISO-TP Consecutive Frame sequence number mismatch.";

            result.payload.clear();

            return result;
        }

        for (
            std::size_t dataIndex = 1;
            dataIndex <
            ClassicCanPayloadSize &&
            result.payload.size() <
            expectedPayloadLength;
            ++dataIndex
            )
        {
            result.payload.push_back(
                frames[frameIndex]
                .data[dataIndex]
            );
        }

        expectedSequenceNumber =
            static_cast<std::uint8_t>(
                (
                    expectedSequenceNumber + 1
                    ) &
                LowNibbleMask
                );
    }

    // ========================================================
    // Completion validation
    // ========================================================

    if (
        result.payload.size() !=
        expectedPayloadLength
        )
    {
        result.error =
            "ISO-TP message is incomplete.";

        result.payload.clear();

        return result;
    }

    result.success = true;

    return result;
}

CanFrame IsoTpProtocol::createFlowControlFrame(
    std::uint32_t arbitrationId,
    IsoTpFlowStatus flowStatus,
    std::uint8_t blockSize,
    std::uint8_t separationTimeMinimum)
{
    validateArbitrationId(
        arbitrationId
    );

    const std::uint8_t flowStatusValue =
        static_cast<std::uint8_t>(
            flowStatus
            );

    if (
        flowStatusValue >
        static_cast<std::uint8_t>(
            IsoTpFlowStatus::Overflow
            )
        )
    {
        throw std::invalid_argument(
            "Invalid ISO-TP Flow Status."
        );
    }

    CanFrame frame =
        createBaseFrame(
            arbitrationId
        );

    frame.data[0] =
        static_cast<std::uint8_t>(
            FlowControlPci |
            (
                flowStatusValue &
                LowNibbleMask
                )
            );

    frame.data[1] =
        blockSize;

    frame.data[2] =
        separationTimeMinimum;

    return frame;
}

IsoTpFrameInfo IsoTpProtocol::parseFrame(
    const CanFrame& frame)
{
    IsoTpFrameInfo info;

    if (
        frame.dlc == 0 ||
        frame.dlc >
        ClassicCanPayloadSize
        )
    {
        info.error =
            "Invalid CAN DLC for ISO-TP frame.";

        return info;
    }

    const std::uint8_t pci =
        frame.data[0];

    const std::uint8_t frameType =
        static_cast<std::uint8_t>(
            (
                pci &
                FrameTypeMask
                ) >> 4
            );

    switch (frameType)
    {
    case 0x0:
    {
        info.type =
            IsoTpFrameType::SingleFrame;

        info.payloadLength =
            static_cast<std::uint16_t>(
                pci &
                LowNibbleMask
                );

        if (
            info.payloadLength == 0 ||
            info.payloadLength >
            SingleFrameMaximumPayload
            )
        {
            info.error =
                "Invalid ISO-TP Single Frame payload length.";

            return info;
        }

        if (
            static_cast<std::size_t>(
                info.payloadLength + 1
                ) >
            frame.dlc
            )
        {
            info.error =
                "Single Frame DLC is smaller than declared payload length.";

            return info;
        }

        info.valid = true;

        return info;
    }

    case 0x1:
    {
        info.type =
            IsoTpFrameType::FirstFrame;

        if (frame.dlc < ClassicCanPayloadSize)
        {
            info.error =
                "First Frame requires an 8-byte Classic CAN frame.";

            return info;
        }

        info.payloadLength =
            static_cast<std::uint16_t>(
                (
                    static_cast<std::uint16_t>(
                        pci &
                        LowNibbleMask
                        ) << 8
                    )
                |
                frame.data[1]
                );

        if (
            info.payloadLength <=
            SingleFrameMaximumPayload ||
            info.payloadLength >
            MaximumPayloadLength
            )
        {
            info.error =
                "Invalid ISO-TP First Frame payload length.";

            return info;
        }

        info.valid = true;

        return info;
    }

    case 0x2:
    {
        info.type =
            IsoTpFrameType::ConsecutiveFrame;

        info.sequenceNumber =
            static_cast<std::uint8_t>(
                pci &
                LowNibbleMask
                );

        info.valid = true;

        return info;
    }

    case 0x3:
    {
        info.type =
            IsoTpFrameType::FlowControl;

        if (frame.dlc < 3)
        {
            info.error =
                "Flow Control frame requires at least 3 bytes.";

            return info;
        }

        const std::uint8_t flowStatus =
            static_cast<std::uint8_t>(
                pci &
                LowNibbleMask
                );

        if (
            flowStatus >
            static_cast<std::uint8_t>(
                IsoTpFlowStatus::Overflow
                )
            )
        {
            info.error =
                "Unsupported ISO-TP Flow Status.";

            return info;
        }

        info.flowStatus =
            static_cast<IsoTpFlowStatus>(
                flowStatus
                );

        info.blockSize =
            frame.data[1];

        info.separationTimeMinimum =
            frame.data[2];

        info.valid = true;

        return info;
    }

    default:
    {
        info.type =
            IsoTpFrameType::Unknown;

        info.error =
            "Unknown ISO-TP frame type.";

        return info;
    }
    }
}

void IsoTpProtocol::validateArbitrationId(
    std::uint32_t arbitrationId)
{
    if (arbitrationId > 0x7FF)
    {
        throw std::invalid_argument(
            "Initial ISO-TP implementation supports only standard 11-bit CAN identifiers."
        );
    }
}

CanFrame IsoTpProtocol::createBaseFrame(
    std::uint32_t arbitrationId)
{
    CanFrame frame;

    frame.arbitrationId =
        arbitrationId;

    frame.format =
        CanFrameFormat::Standard;

    frame.type =
        CanFrameType::Data;

    frame.dlc =
        static_cast<std::uint8_t>(
            ClassicCanPayloadSize
            );

    frame.data.fill(
        0x00
    );

    return frame;
}