#pragma once

#include <QList>

namespace CaraTest
{
    enum class TestResultKind
    {
        Invalid,
        Skipped,
        Failed,
        Passed
    };

    struct TestResult
    {
        QList<TestResultKind> data;
    };
}
