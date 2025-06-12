#pragma once

#include <CaraTest/API.h>
#include <CaraTest/TestRunnerOutputBase.h>
#include <CaraTest/TestSuite.h>
#include <CaraTest/TestSuiteResult.h>
#include <QList>

namespace CaraTest
{
    class CARATEST_API TestRunner
    {
    public:
        enum class OutputMode
        {
            None,
            WindowsConsole
        };

        TestRunner(int argc, char* argv[], OutputMode output = OutputMode::WindowsConsole);

        [[nodiscard]] int run(const TestSuite& suite);
        [[nodiscard]] int run(const QList<TestSuite>& suites);

    private:
        std::unique_ptr<TestRunnerOutputBase> m_output;

        TestSuiteResult runInternal(const TestSuite& suite);
    };
}
