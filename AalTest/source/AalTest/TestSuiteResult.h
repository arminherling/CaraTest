#pragma once

#include <chrono>

namespace AalTest
{
    struct TestSuiteResult
    {
        int passedTestCount;
        int skippedTestCount;
        int failedTestCount;
        int totalTestCount;
        std::chrono::nanoseconds duration;
    };
}
