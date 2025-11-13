#pragma once

#include <CaraTest/API.h>
#include <CaraTest/TestRunnerOutputBase.h>
#include <CaraTest/TestSuite.h>

namespace CaraTest
{
    class CARATEST_API TestRunner
    {
    public:
        enum class OutputMode
        {
            None,
            Console
        };

        TestRunner(int argc, char* argv[], OutputMode output = OutputMode::Console);

        [[nodiscard]] int run(const TestSuite& suite);
        [[nodiscard]] int run(const std::vector<TestSuitePtr>& suites);

    private:
        TestRunnerOutputBaseUPtr m_output;

        TestSuiteResult runInternal(const TestSuite& suite);
    };
}
