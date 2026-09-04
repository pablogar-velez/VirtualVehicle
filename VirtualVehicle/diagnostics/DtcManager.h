#pragma once

#include <cstdint>
#include <vector>

#include "Dtc.h"

class DtcManager
{
public:
    DtcManager();

    void reportFault(
        std::uint32_t code,
        double currentTimeMs
    );

    void reportHealthy(
        std::uint32_t code,
        double currentTimeMs
    );

    bool clearDtc(
        std::uint32_t code
    );

    void clearAll();

    bool hasDtc(
        std::uint32_t code
    ) const;

    bool isDtcActive(
        std::uint32_t code
    ) const;

    const Dtc* findDtc(
        std::uint32_t code
    ) const;

    const std::vector<Dtc>&
        getDtcs() const;

private:
    std::vector<Dtc> dtcs;

    Dtc* findDtcMutable(
        std::uint32_t code
    );
};