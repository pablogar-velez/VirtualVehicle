#include "UdsServer.h"

#include <cmath>

#include "DtcDefinitions.h"
#include "UdsService.h"

// ============================================================
// Construction
// ============================================================

UdsServer::UdsServer(
    DtcManager& dtcManager
)
    :
    dtcManager(dtcManager)
{
}

// ============================================================
// Main Request Dispatcher
// ============================================================

UdsResponse UdsServer::processRequest(
    const UdsRequest& request
)
{
    switch (request.serviceId)
    {
    case UdsService::DiagnosticSessionControl:
        return processDiagnosticSessionControl(request);

    case UdsService::ClearDiagnosticInformation:
        return processClearDiagnosticInformation(request);

    case UdsService::ReadDtcInformation:
        return processReadDtcInformation(request);

    case UdsService::ReadDataByIdentifier:
        return processReadDataByIdentifier(request);

    default:
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::ServiceNotSupported
        );
    }
}

// ============================================================
// Vehicle Data
// ============================================================

void UdsServer::setVehicleData(
    const UdsVehicleData& newVehicleData
)
{
    vehicleData = newVehicleData;
}

// ============================================================
// Reset
// ============================================================

void UdsServer::reset()
{
    currentSession =
        UdsService::DefaultSession;

    vehicleData =
        UdsVehicleData{};
}

// ============================================================
// Session Access
// ============================================================

std::uint8_t UdsServer::getCurrentSession() const
{
    return currentSession;
}

// ============================================================
// Diagnostic Session Control - 0x10
// ============================================================

UdsResponse UdsServer::processDiagnosticSessionControl(
    const UdsRequest& request
)
{
    if (request.payload.size() != 1)
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::IncorrectMessageLength
        );
    }

    const std::uint8_t requestedSession =
        request.payload[0];

    if (
        requestedSession != UdsService::DefaultSession &&
        requestedSession != UdsService::ExtendedSession
        )
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::SubFunctionNotSupported
        );
    }

    currentSession =
        requestedSession;

    UdsResponse response =
        createPositiveResponse(request.serviceId);

    response.payload.push_back(
        currentSession
    );

    return response;
}

// ============================================================
// Clear Diagnostic Information - 0x14
// ============================================================

UdsResponse UdsServer::processClearDiagnosticInformation(
    const UdsRequest& request
)
{
    // Simplified VirtualVehicle implementation:
    //
    // FF FF FF = clear all registered DTCs
    //
    // Full ISO 14229 groupOfDTC behavior is outside
    // the current baseline.

    if (request.payload.size() != 3)
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::IncorrectMessageLength
        );
    }

    const bool clearAllRequested =
        request.payload[0] == 0xFF &&
        request.payload[1] == 0xFF &&
        request.payload[2] == 0xFF;

    if (!clearAllRequested)
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::RequestOutOfRange
        );
    }

    dtcManager.clearAll();

    return createPositiveResponse(
        request.serviceId
    );
}

// ============================================================
// Read DTC Information - 0x19
// ============================================================

UdsResponse UdsServer::processReadDtcInformation(
    const UdsRequest& request
)
{
    if (request.payload.size() != 1)
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::IncorrectMessageLength
        );
    }

    const std::uint8_t subFunction =
        request.payload[0];

    if (subFunction != UdsService::ReportActiveDtcs)
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::SubFunctionNotSupported
        );
    }

    UdsResponse response =
        createPositiveResponse(request.serviceId);

    response.payload.push_back(
        subFunction
    );

    std::uint8_t activeDtcCount = 0;

    for (const Dtc& dtc : dtcManager.getDtcs())
    {
        if (dtc.isActive())
        {
            ++activeDtcCount;
        }
    }

    response.payload.push_back(
        activeDtcCount
    );

    for (const Dtc& dtc : dtcManager.getDtcs())
    {
        if (!dtc.isActive())
        {
            continue;
        }

        response.payload.push_back(
            static_cast<std::uint8_t>(
                (dtc.code >> 16) & 0xFF
                )
        );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                (dtc.code >> 8) & 0xFF
                )
        );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                dtc.code & 0xFF
                )
        );
    }

    return response;
}

// ============================================================
// Read Data By Identifier - 0x22
// ============================================================

UdsResponse UdsServer::processReadDataByIdentifier(
    const UdsRequest& request
)
{
    if (request.payload.size() != 2)
    {
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::IncorrectMessageLength
        );
    }

    const std::uint16_t did =
        static_cast<std::uint16_t>(
            (
                static_cast<std::uint16_t>(
                    request.payload[0]
                    ) << 8
                )
            |
            request.payload[1]
            );

    UdsResponse response =
        createPositiveResponse(request.serviceId);

    response.payload.push_back(
        request.payload[0]
    );

    response.payload.push_back(
        request.payload[1]
    );

    switch (did)
    {
        // --------------------------------------------------------
        // Vehicle Speed
        // --------------------------------------------------------

    case UdsService::VehicleSpeedDid:
    {
        const std::uint16_t encodedSpeed =
            static_cast<std::uint16_t>(
                std::lround(
                    vehicleData.vehicleSpeedKmh * 10.0f
                )
                );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                (encodedSpeed >> 8) & 0xFF
                )
        );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                encodedSpeed & 0xFF
                )
        );

        return response;
    }

    // --------------------------------------------------------
    // Engine RPM
    // --------------------------------------------------------

    case UdsService::EngineRpmDid:
    {
        const std::uint16_t rpm =
            vehicleData.engineRpm;

        response.payload.push_back(
            static_cast<std::uint8_t>(
                (rpm >> 8) & 0xFF
                )
        );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                rpm & 0xFF
                )
        );

        return response;
    }

    // --------------------------------------------------------
    // Steering Angle
    // --------------------------------------------------------

    case UdsService::SteeringAngleDid:
    {
        const std::int16_t encodedAngle =
            static_cast<std::int16_t>(
                std::lround(
                    vehicleData.steeringAngleDeg * 10.0f
                )
                );

        const std::uint16_t rawAngle =
            static_cast<std::uint16_t>(
                encodedAngle
                );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                (rawAngle >> 8) & 0xFF
                )
        );

        response.payload.push_back(
            static_cast<std::uint8_t>(
                rawAngle & 0xFF
                )
        );

        return response;
    }

    default:
        return createNegativeResponse(
            request.serviceId,
            UdsService::NegativeResponseCode::RequestOutOfRange
        );
    }
}

// ============================================================
// Positive Response
// ============================================================

UdsResponse UdsServer::createPositiveResponse(
    std::uint8_t requestServiceId
) const
{
    UdsResponse response;

    response.positive =
        true;

    response.serviceId =
        static_cast<std::uint8_t>(
            requestServiceId +
            UdsService::PositiveResponseOffset
            );

    return response;
}

// ============================================================
// Negative Response
// ============================================================

UdsResponse UdsServer::createNegativeResponse(
    std::uint8_t requestServiceId,
    std::uint8_t negativeResponseCode
) const
{
    UdsResponse response;

    response.positive =
        false;

    response.serviceId =
        UdsService::NegativeResponse;

    response.payload.push_back(
        requestServiceId
    );

    response.payload.push_back(
        negativeResponseCode
    );

    return response;
}