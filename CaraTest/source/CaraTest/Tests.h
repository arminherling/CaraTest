#pragma once

#include <CaraTest/TestRunnerOutputBase.h>
#include <CaraTest/HelperExceptions.h>
#include <CaraTest/Stringify.h>
#include <CaraTest/API.h>
#include <string>
#include <memory>
#include <vector>
#include <source_location>

namespace CaraTest
{
#if defined(_MSC_VER)
#define TRIGGER_DEBUG_BREAK() __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
#define TRIGGER_DEBUG_BREAK() __builtin_trap();
#else
#define TRIGGER_DEBUG_BREAK() static_assert(false, "Debug break not supported for this compiler");
#endif

#define HANDLE_CARATEST_FAILURE()                                                   \
    do {                                                                            \
        if(CaraTest::getFailureBehavior() == CaraTest::FailureBehavior::DebugBreak) \
        {                                                                           \
           TRIGGER_DEBUG_BREAK();                                                   \
        }                                                                           \
    } while(false)

    enum class FailureBehavior
    {
        Default,
        DebugBreak,
        DebugBreakAndRerun
    };

    CARATEST_API FailureBehavior& getFailureBehavior();
    CARATEST_API void setFailureBehavior(FailureBehavior value);





    /// <summary>
    /// Base class for tests
    /// </summary>
    class Test
    {
    public:
        virtual Position writeHeader(const TestRunnerOutputBaseUPtr& output, int currentTest, int totalTestCount) const = 0;
        virtual void run(const TestRunnerOutputBaseUPtr& output, int headerIndentation, int& currentTest) = 0;
        virtual void writeResult(const TestRunnerOutputBaseUPtr& output, const Position& position) const {};
        virtual const std::string& name() const = 0;
        virtual int testCount() const = 0;
        TestResult result() const { return m_result; }
        void addResult(TestResultKind result) { m_result.data.push_back(result); }

    private:
        TestResult m_result;
    };

    using TestPtr = std::shared_ptr<Test>;




    /// <summary>
    /// Simple test without parameters
    /// </summary>
    template<typename TFunction>
    class SimpleTest : public Test
    {
    public:
        SimpleTest(TFunction&& function, const std::string& name)
            : m_function{ function }
            , m_name{ name }
        {
        }

        Position writeHeader(const TestRunnerOutputBaseUPtr& output, int currentTest, int totalTestCount) const override
        {
            return output->writeTestHeader(currentTest, totalTestCount, name(), false);
        }

        void run(const TestRunnerOutputBaseUPtr& output, int headerIndentation, int& currentTest) override
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

        void writeResult(const TestRunnerOutputBaseUPtr& output, const Position& position) const override
        {
            output->updateTestResult(position, result().data.front());
        }

        const std::string& name() const override
        {
            return m_name;
        }

        int testCount() const override
        {
            return 1;
        }

    private:
        void rerunTest() const
        {
            if (getFailureBehavior() == FailureBehavior::DebugBreakAndRerun)
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
        std::string m_name;
    };



    /// <summary>
    /// Parameterized test with data set
    /// </summary>
    template<typename TFunction, typename TData>
    class ParameterizedTest : public Test
    {
    public:
        ParameterizedTest(TFunction&& function, TData&& data, const std::string& name)
            : m_function{ function }
            , m_name{ name }
            , m_data{ data }
        {
        }

        Position writeHeader(const TestRunnerOutputBaseUPtr& output, int currentTest, int totalTestCount) const override
        {
            return output->writeTestHeader(currentTest, totalTestCount, name(), true);
        }

        void run(const TestRunnerOutputBaseUPtr& output, int headerIndentation, int& currentTest) override
        {
            auto totalSubTestCount = m_data.size();
            auto testResult = TestResultKind::Passed;
            if (totalSubTestCount == 0)
                return;

            for (const auto& parameters : m_data)
            {
                Position resultPosition;
                try
                {
                    const auto stringifiedParameters = sanitize(stringifyAndMaybeQuote(parameters));
                    resultPosition = output->writeSubTestHeader(headerIndentation, currentTest, totalSubTestCount, stringifiedParameters);

                    std::apply(m_function, parameters);

                    output->updateTestResult(resultPosition, TestResultKind::Passed);
                    output->writeTestPassedMessage();
                    addResult(TestResultKind::Passed);
                }
                catch (SkipTestException& e)
                {
                    output->updateTestResult(resultPosition, TestResultKind::Skipped);
                    output->writeTestSkippedMessage(e);
                    addResult(TestResultKind::Skipped);
                }
                catch (FailedTestException& e)
                {
                    rerunTest(parameters);
                    output->updateTestResult(resultPosition, TestResultKind::Failed);
                    output->writeTestFailedMessage(e);
                    addResult(TestResultKind::Failed);
                }
                catch (ValueMismatchTestException& e)
                {
                    rerunTest(parameters);
                    output->updateTestResult(resultPosition, TestResultKind::Failed);
                    output->writeTestValueMismatchMessage(e);
                    addResult(TestResultKind::Failed);
                }
                catch (SnapshotCreatedTestException& e)
                {
                    rerunTest(parameters);
                    output->updateTestResult(resultPosition, TestResultKind::Failed);
                    output->writeSnapshotCreatedMessage(e);
                    addResult(TestResultKind::Failed);
                }
                currentTest++;
            }
        }

        const std::string& name() const override
        {
            return m_name;
        }

        int testCount() const override
        {
            return m_data.size();
        }

    private:
        template<typename TPara>
        void rerunTest(const TPara& parameters) const
        {
            if (getFailureBehavior() == FailureBehavior::DebugBreakAndRerun)
            {
                try
                {
                    TRIGGER_DEBUG_BREAK();
                    std::apply(m_function, parameters);
                }
                catch (...) {}
            }
        }

        TFunction m_function;
        TData m_data;
        std::string m_name;
    };
}
