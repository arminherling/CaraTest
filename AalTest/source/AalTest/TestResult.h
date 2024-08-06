#pragma once

#include <QList>

namespace AalTest
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
