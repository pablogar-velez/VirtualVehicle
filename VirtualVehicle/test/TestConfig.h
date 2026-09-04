#pragma once

namespace TestConfig
{
    inline constexpr double simulationStepMs =
        10.0;

    inline constexpr double sensorFaultMaximumResponseTimeMs =
        20.0;

    inline constexpr double sensorFaultObservationWindowMs =
        500.0;

    inline constexpr double speedToleranceKmh =
        0.01;

    inline constexpr double brakeReleasedTolerancePercent =
        0.01;
}
