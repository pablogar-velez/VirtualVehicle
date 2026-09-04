#pragma once

#include <cstdint>

namespace UdsService
{
    // ========================================================
    // Service Identifiers
    // ========================================================

    inline constexpr std::uint8_t DiagnosticSessionControl = 0x10;
    inline constexpr std::uint8_t ClearDiagnosticInformation = 0x14;
    inline constexpr std::uint8_t ReadDtcInformation = 0x19;
    inline constexpr std::uint8_t ReadDataByIdentifier = 0x22;

    inline constexpr std::uint8_t NegativeResponse = 0x7F;

    inline constexpr std::uint8_t PositiveResponseOffset = 0x40;

    // ========================================================
    // Diagnostic Sessions
    // ========================================================

    inline constexpr std::uint8_t DefaultSession = 0x01;
    inline constexpr std::uint8_t ExtendedSession = 0x03;

    // ========================================================
    // Read DTC Information Subfunctions
    // ========================================================

    inline constexpr std::uint8_t ReportActiveDtcs = 0x02;

    // ========================================================
    // Data Identifiers
    // ========================================================

    inline constexpr std::uint16_t VehicleSpeedDid = 0xF100;
    inline constexpr std::uint16_t EngineRpmDid = 0xF101;
    inline constexpr std::uint16_t SteeringAngleDid = 0xF102;

    // ========================================================
    // Negative Response Codes
    // ========================================================

    namespace NegativeResponseCode
    {
        inline constexpr std::uint8_t ServiceNotSupported = 0x11;
        inline constexpr std::uint8_t SubFunctionNotSupported = 0x12;
        inline constexpr std::uint8_t IncorrectMessageLength = 0x13;
        inline constexpr std::uint8_t RequestOutOfRange = 0x31;
    }
}