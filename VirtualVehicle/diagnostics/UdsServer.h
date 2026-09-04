#pragma once

#include <cstdint>

#include "DtcManager.h"
#include "UdsRequest.h"
#include "UdsResponse.h"

struct UdsVehicleData
{
    float vehicleSpeedKmh{ 0.0f };
    std::uint16_t engineRpm{ 0 };
    float steeringAngleDeg{ 0.0f };
};

class UdsServer
{
public:
    explicit UdsServer(
        DtcManager& dtcManager
    );

    UdsResponse processRequest(
        const UdsRequest& request
    );

    void setVehicleData(
        const UdsVehicleData& vehicleData
    );

    void reset();

    std::uint8_t getCurrentSession() const;

private:
    DtcManager& dtcManager;

    UdsVehicleData vehicleData{};

    std::uint8_t currentSession{ 0x01 };

    UdsResponse processDiagnosticSessionControl(
        const UdsRequest& request
    );

    UdsResponse processClearDiagnosticInformation(
        const UdsRequest& request
    );

    UdsResponse processReadDtcInformation(
        const UdsRequest& request
    );

    UdsResponse processReadDataByIdentifier(
        const UdsRequest& request
    );

    UdsResponse createPositiveResponse(
        std::uint8_t requestServiceId
    ) const;

    UdsResponse createNegativeResponse(
        std::uint8_t requestServiceId,
        std::uint8_t negativeResponseCode
    ) const;
};