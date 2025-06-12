#include "TestRunner.h"
#include <CaraTest/TestRunnerNullOutput.h>
#include <CaraTest/TestRunnerWindowsConsoleOutput.h>
#include <CaraTest/FailureBehavior.h>

namespace
{
    using namespace CaraTest;

    std::unique_ptr<TestRunnerOutputBase> CreateOutput(TestRunner::OutputMode output)
    {
        switch (output)
        {
            using enum TestRunner::OutputMode;

            case WindowsConsole:
                return std::make_unique<TestRunnerWindowsConsoleOutput>();
            case None:
            default:
                return std::make_unique<TestRunnerNullOutput>();
        }
    }

    void ParseAndSetFailureBehavior(int argc, char* argv[])
    {
        if(argc >= 2)
        {
            for (int i = 1; i < argc; i++)
            {
                const auto argument = QString::fromLocal8Bit(argv[i]);
                if (argument.compare("--break_on_fail", Qt::CaseSensitivity::CaseInsensitive) == 0)
                {
                    SetFailureBehavior(FailureBehavior::DebugBreak);
                } 
                else if (argument.compare("--break_and_rerun_on_fail", Qt::CaseSensitivity::CaseInsensitive) == 0)
                {
                    SetFailureBehavior(FailureBehavior::DebugBreakAndRerun);
                }
            }
        }
    }
}

namespace CaraTest
{
    TestRunner::TestRunner(int argc, char* argv[], OutputMode output)
        : m_output{ CreateOutput(output) }
    {
        ParseAndSetFailureBehavior(argc, argv);
    }

    int TestRunner::run(const TestSuite& suite)
    {
        const auto result = runInternal(suite);

        m_output->writeTestRunnerResult(result);

        if (result.failedTestCount != 0)
        {
            return 1; // some tests failed
        }
        else
        {
            return 0; // all tests passed
        }
    }

    int TestRunner::run(const QList<TestSuite>& suites)
    {
        QList<TestSuiteResult> results;
        bool someFailedTests = false;
        for (const auto& suite : suites)
        {
            const auto result = runInternal(suite);
            results << result;
            if (result.failedTestCount != 0)
            {
                someFailedTests = true;
            }

            m_output->writeTestRunnerResult(result);
            m_output->writeEmptyLine();
        }
        m_output->writeEmptyLine();
        m_output->writeTestRunnerTotalResult(results);

        if (someFailedTests)
        {
            return 1; // some tests failed
        }
        else
        {
            return 0; // all tests passed
        }
    }

    TestSuiteResult TestRunner::runInternal(const TestSuite& suite)
    {
        const auto& tests = suite.tests();
        auto currentTest = 1;
        auto totalTestCount = 0;
        for (const auto& test : tests)
        {
            totalTestCount += test->testCount();
        }
        const auto totalCountString = QString::number(totalTestCount);

        m_output->writeSuiteName(suite.name());
        const auto suiteStartTime = std::chrono::high_resolution_clock::now();

        for (const auto& test : tests)
        {
            const auto resultPosition = test->writeHeader(m_output, currentTest, totalTestCount);
            test->run(m_output, totalCountString.length(), currentTest);
            test->writeResult(m_output, resultPosition);
        }

        const auto suiteEndTime = std::chrono::high_resolution_clock::now();
        const auto suiteDuration = suiteEndTime - suiteStartTime;

        return { suite.passedTests(), suite.skippedTests(), suite.failedTests(), totalTestCount, suiteDuration };
    }
}
