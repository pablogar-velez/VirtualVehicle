#include "UdsCanTransport.h"

#include <algorithm>
#include <stdexcept>

UdsCanTransport::UdsCanTransport(
    VirtualCanBus& canBus,
    UdsServer& udsServer)
    : canBus(canBus),
    udsTransport(udsServer)
{
}

UdsCanTransactionResult
UdsCanTransport::processRequest(
    const UdsRequest& request,
    double startTimeMs)
{
    if (startTimeMs < 0.0)
    {
        throw std::invalid_argument(
            "UDS CAN transaction start time must not be negative."
        );
    }

    UdsCanTransactionResult result;

    result.startTimeMs =
        startTimeMs;

    double currentTimeMs =
        std::max(
            startTimeMs,
            canBus.getBusyUntilMs()
        );

    // ==================================================
    // Diagnostic Client
    // UDS Request -> ISO-TP Frames
    // ==================================================

    const std::vector<CanFrame>
        generatedRequestFrames =
        udsTransport
        .createRequestFrames(
            request
        );

    // ==================================================
    // Request Frames -> Virtual CAN Bus
    // ==================================================

    result.requestFrames =
        transmitFrames(
            generatedRequestFrames,
            currentTimeMs
        );

    // ==================================================
    // ECU Side
    // Reassemble Request -> UDS Server -> ISO-TP Response
    // ==================================================

    const std::vector<CanFrame>
        generatedResponseFrames =
        udsTransport
        .processRequestFrames(
            result.requestFrames
        );

    // ==================================================
    // Response Frames -> Virtual CAN Bus
    // ==================================================

    result.responseFrames =
        transmitFrames(
            generatedResponseFrames,
            currentTimeMs
        );

    // ==================================================
    // Diagnostic Client Side
    // Reassemble final response
    // ==================================================

    result.response =
        udsTransport
        .decodeResponseFrames(
            result.responseFrames
        );

    result.completionTimeMs =
        currentTimeMs;

    return result;
}

UdsTransport&
UdsCanTransport::getUdsTransport()
{
    return udsTransport;
}

const UdsTransport&
UdsCanTransport::getUdsTransport() const
{
    return udsTransport;
}

std::vector<CanFrame>
UdsCanTransport::transmitFrames(
    const std::vector<CanFrame>& frames,
    double& currentTimeMs)
{
    if (frames.empty())
    {
        throw std::invalid_argument(
            "UDS CAN transport cannot transmit an empty frame sequence."
        );
    }

    std::vector<CanFrame>
        receivedFrames;

    receivedFrames.reserve(
        frames.size()
    );

    for (const CanFrame& frame : frames)
    {
        currentTimeMs =
            std::max(
                currentTimeMs,
                canBus.getBusyUntilMs()
            );

        canBus.transmit(
            frame,
            currentTimeMs
        );

        if (!canBus.hasFrame())
        {
            throw std::runtime_error(
                "Virtual CAN Bus did not retain the transmitted diagnostic frame."
            );
        }

        const CanFrame receivedFrame =
            canBus.receive(
                currentTimeMs
            );

        receivedFrames.push_back(
            receivedFrame
        );

        currentTimeMs =
            canBus.getBusyUntilMs();
    }

    return receivedFrames;
}