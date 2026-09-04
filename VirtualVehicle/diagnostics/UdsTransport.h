#pragma once

#include <cstdint>
#include <vector>

#include "UdsRequest.h"
#include "UdsResponse.h"
#include "UdsServer.h"

#include "../can/CanFrame.h"

class UdsTransport
{
public:
    static constexpr std::uint32_t
        DefaultRequestCanId = 0x7E0;

    static constexpr std::uint32_t
        DefaultResponseCanId = 0x7E8;

    explicit UdsTransport(
        UdsServer& udsServer,
        std::uint32_t requestCanId =
        DefaultRequestCanId,
        std::uint32_t responseCanId =
        DefaultResponseCanId
    );

    std::vector<CanFrame> createRequestFrames(
        const UdsRequest& request
    ) const;

    std::vector<CanFrame> createResponseFrames(
        const UdsResponse& response
    ) const;

    UdsRequest decodeRequestFrames(
        const std::vector<CanFrame>& frames
    ) const;

    UdsResponse decodeResponseFrames(
        const std::vector<CanFrame>& frames
    ) const;

    std::vector<CanFrame> processRequestFrames(
        const std::vector<CanFrame>& requestFrames
    );

    UdsResponse processRequest(
        const UdsRequest& request
    );

    std::uint32_t getRequestCanId() const;

    std::uint32_t getResponseCanId() const;

private:
    UdsServer& udsServer;

    std::uint32_t requestCanId;
    std::uint32_t responseCanId;

    static std::vector<std::uint8_t>
        serializeRequest(
            const UdsRequest& request
        );

    static std::vector<std::uint8_t>
        serializeResponse(
            const UdsResponse& response
        );

    static UdsRequest deserializeRequest(
        const std::vector<std::uint8_t>& payload
    );

    static UdsResponse deserializeResponse(
        const std::vector<std::uint8_t>& payload
    );

    static void validateFrameIdentifier(
        const std::vector<CanFrame>& frames,
        std::uint32_t expectedCanId
    );
};