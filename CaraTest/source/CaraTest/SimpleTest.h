#pragma once

#include <CaraTest/FailureBehavior.h>
#include <CaraTest/TestBase.h>
#include <CaraTest/TestResult.h>
#include <CaraTest/TestRunnerOutputBase.h>

#include <memory>
#include <QString>

namespace CaraTest
{
    template<typename TFunction>
    class SimpleTest : public TestBase
    {
    public:
        SimpleTest(TFunction&& function, const QString& testName)
            : m_function{ function }
            , m_testName{ testName }
        {
        }

        QPoint writeHeader(const std::unique_ptr<TestRunnerOutputBase>& output, int currentTest, int totalTestCount) const override
        {
            return output->writeTestHeader(currentTest, totalTestCount, testName(), false);
        }

        void run(const std::unique_ptr<TestRunnerOutputBase>& output, int headerIndentation, int& currentTest) override
        {
            try
            {
                m_function();

                addResult(TestResultKind::Passed);
                output->writeTestPassedMessage();
            }
            catch (SkipTestException& e)
            {
                addResult(TestResultKind::Skipped);
                output->writeTestSkippedMessage(e);
            }
            catch (FailedTestException& e)
            {
                rerunTest();
                addResult(TestResultKind::Failed);
                output->writeTestFailedMessage(e);
            }
            catch (ValueMismatchTestException& e)
            {
                rerunTest();
                addResult(TestResultKind::Failed);
                output->writeTestValueMismatchMessage(e);
            }
            catch (SnapshotCreatedTestException& e)
            {
                rerunTest();
                addResult(TestResultKind::Failed);
                output->writeSnapshotCreatedMessage(e);
            }
            currentTest++;
        }

        void writeResult(const std::unique_ptr<TestRunnerOutputBase>& output, const QPoint& position) const override
        {
            output->updateTestResult(position, result().data.first());
        }

        const QString& testName() const override
        {
            return m_testName;
        }

        int testCount() const override
        {
            return 1;
        }

    private:
        void rerunTest() const
        {
            if (GetFailureBehavior() == FailureBehavior::DebugBreakAndRerun)
            {
                try
                {
                    TRIGGER_DEBUG_BREAK();
                    m_function();
                }
                catch (...) {}
            }
        }

        TFunction m_function;
        QString m_testName;
    };
}
