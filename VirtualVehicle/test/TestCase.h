#pragma once

#include <string>

#include "TestResult.h"

// ============================================================
// Test Case
// ============================================================

struct TestCase
{
    // ========================================================
    // Identification
    // ========================================================

    std::string id;
    std::string name;
    std::string description;

    // ========================================================
    // Requirement traceability
    // ========================================================

    std::string requirementId;

    // ========================================================
    // Test definition
    // ========================================================

    std::string precondition;
    std::string stimulus;
    std::string expectedResult;

    // ========================================================
    // Execution result
    // ========================================================

    TestResult result;
};