#include "DtcManager.h"
#include "DtcDefinitions.h"

DtcManager::DtcManager()
{
    dtcs.push_back(
        {
            DtcDefinitions::FrontLeftWheelSpeedSensor,
            "Front Left Wheel Speed Sensor",
            "Front-left wheel-speed sensor signal is invalid.",
            DtcStatus::Inactive,
            0.0,
            0.0,
            0
        }
    );

    dtcs.push_back(
        {
            DtcDefinitions::FrontRightWheelSpeedSensor,
            "Front Right Wheel Speed Sensor",
            "Front-right wheel-speed sensor signal is invalid.",
            DtcStatus::Inactive,
            0.0,
            0.0,
            0
        }
    );

    dtcs.push_back(
        {
            DtcDefinitions::AbsCanCommunication,
            "ABS CAN Communication",
            "ABS ECU periodic CAN transmission is unavailable.",
            DtcStatus::Inactive,
            0.0,
            0.0,
            0
        }
    );

    dtcs.push_back(
        {
            DtcDefinitions::PowertrainCanCommunication,
            "Powertrain CAN Communication",
            "Powertrain ECU periodic CAN transmission is unavailable.",
            DtcStatus::Inactive,
            0.0,
            0.0,
            0
        }
    );

    dtcs.push_back(
        {
            DtcDefinitions::SteeringCanCommunication,
            "Steering CAN Communication",
            "Steering ECU periodic CAN transmission is unavailable.",
            DtcStatus::Inactive,
            0.0,
            0.0,
            0
        }
    );

    dtcs.push_back(
        {
            DtcDefinitions::EthernetNodeACommunication,
            "Ethernet Node A Communication",
            "Automotive Ethernet Node A communication is unavailable.",
            DtcStatus::Inactive,
            0.0,
            0.0,
            0
        }
    );
}

void DtcManager::reportFault(
    std::uint32_t code,
    double currentTimeMs
)
{
    Dtc* dtc =
        findDtcMutable(code);

    if (dtc == nullptr)
    {
        return;
    }

    const bool wasActive =
        dtc->isActive();

    if (!wasActive)
    {
        if (dtc->occurrenceCount == 0)
        {
            dtc->firstDetectedTimeMs =
                currentTimeMs;
        }

        ++dtc->occurrenceCount;
    }

    dtc->status =
        DtcStatus::Active;

    dtc->lastDetectedTimeMs =
        currentTimeMs;
}

void DtcManager::reportHealthy(
    std::uint32_t code,
    double currentTimeMs
)
{
    Dtc* dtc =
        findDtcMutable(code);

    if (dtc == nullptr)
    {
        return;
    }

    if (dtc->status == DtcStatus::Active)
    {
        dtc->status =
            DtcStatus::Inactive;

        dtc->lastDetectedTimeMs =
            currentTimeMs;
    }
}

bool DtcManager::clearDtc(
    std::uint32_t code
)
{
    Dtc* dtc =
        findDtcMutable(code);

    if (dtc == nullptr)
    {
        return false;
    }

    dtc->status =
        DtcStatus::Cleared;

    dtc->firstDetectedTimeMs =
        0.0;

    dtc->lastDetectedTimeMs =
        0.0;

    dtc->occurrenceCount =
        0;

    return true;
}

void DtcManager::clearAll()
{
    for (Dtc& dtc : dtcs)
    {
        dtc.status =
            DtcStatus::Cleared;

        dtc.firstDetectedTimeMs =
            0.0;

        dtc.lastDetectedTimeMs =
            0.0;

        dtc.occurrenceCount =
            0;
    }
}

bool DtcManager::hasDtc(
    std::uint32_t code
) const
{
    return findDtc(code) != nullptr;
}

bool DtcManager::isDtcActive(
    std::uint32_t code
) const
{
    const Dtc* dtc =
        findDtc(code);

    if (dtc == nullptr)
    {
        return false;
    }

    return dtc->isActive();
}

const Dtc* DtcManager::findDtc(
    std::uint32_t code
) const
{
    for (const Dtc& dtc : dtcs)
    {
        if (dtc.code == code)
        {
            return &dtc;
        }
    }

    return nullptr;
}

const std::vector<Dtc>&
DtcManager::getDtcs() const
{
    return dtcs;
}

Dtc* DtcManager::findDtcMutable(
    std::uint32_t code
)
{
    for (Dtc& dtc : dtcs)
    {
        if (dtc.code == code)
        {
            return &dtc;
        }
    }

    return nullptr;
}
