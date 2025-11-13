#pragma once

#include <CaraTest/Asserts.h>
#include <CaraTest/TestRunnerOutputBase.h>

namespace CaraTest
{
    class TestRunnerNullOutput : public TestRunnerOutputBase
    {
    public:
        TestRunnerNullOutput() = default;
        ~TestRunnerNullOutput() override = default;

        void writeSuiteName(const std::string& name) override {};
        Position writeTestHeader(int currentTest, int totalTests, const std::string& testName, bool hasSubTests) override { return Position(); };
        Position writeSubTestHeader(int indentation, int currentTest, int totalTests, const std::string& parameters) override { return Position(); };
        void updateTestResult(const Position& position, TestResultKind result) override {};
        void writeTestPassedMessage() override {};
        void writeTestSkippedMessage(SkipTestException& e) override {};
        void writeTestFailedMessage(FailedTestException& e) override {};
        void writeTestValueMismatchMessage(ValueMismatchTestException& e) override {};
        void writeSnapshotCreatedMessage(SnapshotCreatedTestException& e) override {};
        void writeTestRunnerResult(const TestSuiteResult& result) override {};
        void writeTestRunnerTotalResult(const std::vector<TestSuiteResult>& results) override {};
        void writeEmptyLine() override {};
    };
}
