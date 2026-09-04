#include "UdsTransport.h"

#include <stdexcept>

#include "../transport/IsoTpProtocol.h"
#include "UdsService.h"

UdsTransport::UdsTransport(
    UdsServer& udsServer,
    std::uint32_t requestCanId,
    std::uint32_t responseCanId)
    : udsServer(udsServer),
    requestCanId(requestCanId),
    responseCanId(responseCanId)
{
    if (
        requestCanId > 0x7FF ||
        responseCanId > 0x7FF
        )
    {
        throw std::invalid_argument(
            "UDS transport requires standard 11-bit CAN identifiers."
        );
    }
}

std::vector<CanFrame>
UdsTransport::createRequestFrames(
    const UdsRequest& request) const
{
    const std::vector<std::uint8_t> payload =
        serializeRequest(
            request
        );

    return IsoTpProtocol::segment(
        requestCanId,
        payload
    );
}

std::vector<CanFrame>
UdsTransport::createResponseFrames(
    const UdsResponse& response) const
{
    const std::vector<std::uint8_t> payload =
        serializeResponse(
            response
        );

    return IsoTpProtocol::segment(
        responseCanId,
        payload
    );
}

UdsRequest
UdsTransport::decodeRequestFrames(
    const std::vector<CanFrame>& frames) const
{
    validateFrameIdentifier(
        frames,
        requestCanId
    );

    const IsoTpReassemblyResult result =
        IsoTpProtocol::reassemble(
            frames
        );

    if (!result.success)
    {
        throw std::runtime_error(
            "Failed to reassemble UDS request: " +
            result.error
        );
    }

    return deserializeRequest(
        result.payload
    );
}

UdsResponse
UdsTransport::decodeResponseFrames(
    const std::vector<CanFrame>& frames) const
{
    validateFrameIdentifier(
        frames,
        responseCanId
    );

    const IsoTpReassemblyResult result =
        IsoTpProtocol::reassemble(
            frames
        );

    if (!result.success)
    {
        throw std::runtime_error(
            "Failed to reassemble UDS response: " +
            result.error
        );
    }

    return deserializeResponse(
        result.payload
    );
}

std::vector<CanFrame>
UdsTransport::processRequestFrames(
    const std::vector<CanFrame>& requestFrames)
{
    const UdsRequest request =
        decodeRequestFrames(
            requestFrames
        );

    const UdsResponse response =
        udsServer.processRequest(
            request
        );

    return createResponseFrames(
        response
    );
}

UdsResponse
UdsTransport::processRequest(
    const UdsRequest& request)
{
    const std::vector<CanFrame> requestFrames =
        createRequestFrames(
            request
        );

    const std::vector<CanFrame> responseFrames =
        processRequestFrames(
            requestFrames
        );

    return decodeResponseFrames(
        responseFrames
    );
}

std::uint32_t
UdsTransport::getRequestCanId() const
{
    return requestCanId;
}

std::uint32_t
UdsTransport::getResponseCanId() const
{
    return responseCanId;
}

std::vector<std::uint8_t>
UdsTransport::serializeRequest(
    const UdsRequest& request)
{
    std::vector<std::uint8_t> payload;

    payload.reserve(
        request.payload.size() + 1
    );

    payload.push_back(
        request.serviceId
    );

    payload.insert(
        payload.end(),
        request.payload.begin(),
        request.payload.end()
    );

    return payload;
}

std::vector<std::uint8_t>
UdsTransport::serializeResponse(
    const UdsResponse& response)
{
    std::vector<std::uint8_t> payload;

    payload.reserve(
        response.payload.size() + 1
    );

    payload.push_back(
        response.serviceId
    );

    payload.insert(
        payload.end(),
        response.payload.begin(),
        response.payload.end()
    );

    return payload;
}

UdsRequest
UdsTransport::deserializeRequest(
    const std::vector<std::uint8_t>& payload)
{
    if (payload.empty())
    {
        throw std::invalid_argument(
            "UDS request payload is empty."
        );
    }

    UdsRequest request;

    request.serviceId =
        payload.front();

    request.payload.assign(
        payload.begin() + 1,
        payload.end()
    );

    return request;
}

UdsResponse
UdsTransport::deserializeResponse(
    const std::vector<std::uint8_t>& payload)
{
    if (payload.empty())
    {
        throw std::invalid_argument(
            "UDS response payload is empty."
        );
    }

    UdsResponse response;

    response.serviceId =
        payload.front();

    response.payload.assign(
        payload.begin() + 1,
        payload.end()
    );

    response.positive =
        response.serviceId !=
        UdsService::NegativeResponse;

    return response;
}

void
UdsTransport::validateFrameIdentifier(
    const std::vector<CanFrame>& frames,
    std::uint32_t expectedCanId)
{
    if (frames.empty())
    {
        throw std::invalid_argument(
            "UDS transport received no CAN frames."
        );
    }

    for (const CanFrame& frame : frames)
    {
        if (
            frame.arbitrationId !=
            expectedCanId
            )
        {
            throw std::invalid_argument(
                "UDS transport received a CAN frame with an unexpected identifier."
            );
        }
    }
}