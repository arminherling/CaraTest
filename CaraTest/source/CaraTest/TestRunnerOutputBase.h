#pragma once

#include <memory>
#include <vector>
#include <string>

namespace CaraTest
{
    struct Position
    {
        int x;
        int y;
    };

    enum class TestResultKind
    {
        Invalid,
        Skipped,
        Failed,
        Passed
    };

    struct TestResult
    {
        std::vector<TestResultKind> data;
    };

    class TestSuiteResult;
    class SkipTestException;
    class FailedTestException;
    class ValueMismatchTestException;
    class SnapshotCreatedTestException;

    class TestRunnerOutputBase
    {
    public:
        virtual ~TestRunnerOutputBase() = default;

        virtual void writeSuiteName(const std::string& name) = 0;
        virtual Position writeTestHeader(int currentTest, int totalTests, const std::string& testName, bool hasSubTests) = 0;
        virtual Position writeSubTestHeader(int indentation, int currentTest, int totalTests, const std::string& parameters) = 0;
        virtual void updateTestResult(const Position& position, TestResultKind result) = 0;
        virtual void writeTestPassedMessage() = 0;
        virtual void writeTestSkippedMessage(SkipTestException& e) = 0;
        virtual void writeTestFailedMessage(FailedTestException& e) = 0;
        virtual void writeTestValueMismatchMessage(ValueMismatchTestException& e) = 0;
        virtual void writeSnapshotCreatedMessage(SnapshotCreatedTestException& e) = 0;
        virtual void writeTestRunnerResult(const TestSuiteResult& result) = 0;
        virtual void writeTestRunnerTotalResult(const std::vector<TestSuiteResult>& results) = 0;
        virtual void writeEmptyLine() = 0;
    };

    using TestRunnerOutputBaseUPtr = std::unique_ptr<TestRunnerOutputBase>;
}
