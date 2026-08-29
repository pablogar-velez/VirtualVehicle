#pragma once

#include <string>

enum class TestStatus
{
    NotRun,
    Running,
    Passed,
    Failed
};

struct TestResult
{
    TestStatus status{
        TestStatus::NotRun
    };

    // Total simulated time when the test finished.
    double executionTimeMs{
        0.0
    };

    // Time measured between stimulus and expected response.
    double responseTimeMs{
        0.0
    };

    // Requirement limit for the response.
    // 0.0 means that this test does not use a timing criterion.
    double maximumAllowedResponseTimeMs{
        0.0
    };

    std::string expected;
    std::string actual;
    std::string message;

    bool passed() const
    {
        return status ==
            TestStatus::Passed;
    }

    bool failed() const
    {
        return status ==
            TestStatus::Failed;
    }

    bool hasTimingRequirement() const
    {
        return maximumAllowedResponseTimeMs >
            0.0;
    }

    bool responseTimeWithinLimit() const
    {
        if (!hasTimingRequirement())
        {
            return true;
        }

        return responseTimeMs <=
            maximumAllowedResponseTimeMs;
    }
};