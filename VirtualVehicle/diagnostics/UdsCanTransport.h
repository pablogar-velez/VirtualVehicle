#pragma once

#include <vector>

#include "../can/CanFrame.h"
#include "../can/VirtualCanBus.h"

#include "UdsRequest.h"
#include "UdsResponse.h"
#include "UdsServer.h"
#include "UdsTransport.h"

struct UdsCanTransactionResult
{
    UdsResponse response;

    std::vector<CanFrame>
        requestFrames;

    std::vector<CanFrame>
        responseFrames;

    double startTimeMs{
        0.0
    };

    double completionTimeMs{
        0.0
    };
};

class UdsCanTransport
{
public:
    UdsCanTransport(
        VirtualCanBus& canBus,
        UdsServer& udsServer
    );

    UdsCanTransactionResult processRequest(
        const UdsRequest& request,
        double startTimeMs = 0.0
    );

    UdsTransport&
        getUdsTransport();

    const UdsTransport&
        getUdsTransport() const;

private:
    VirtualCanBus& canBus;

    UdsTransport udsTransport;

    std::vector<CanFrame> transmitFrames(
        const std::vector<CanFrame>& frames,
        double& currentTimeMs
    );
};