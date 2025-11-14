#pragma once

#include <CaraTest/API.h>
#include <CaraTest/Tests.h>
#include <functional>
#include <chrono>
#include <numeric>

namespace CaraTest
{
    struct TestSuiteResult
    {
        int passedTestCount;
        int skippedTestCount;
        int failedTestCount;
        int totalTestCount;
        std::chrono::nanoseconds duration;
    };

    class CARATEST_API TestSuite
    {
    public:
        TestSuite(const std::string& name);
        TestSuite(const std::source_location& location = std::source_location::current());

        template<typename TFunction>
        TestPtr add(const std::string& testName, TFunction&& testFunction)
        {
            auto test = std::make_shared<
                SimpleTest<
                decltype(std::function(std::forward<TFunction>(testFunction)))>>(
                    std::function(std::forward<TFunction>(testFunction)),
                    testName);

            m_tests.push_back(test);
            return test;
        }

        template<typename TFunction, typename TData>
        TestPtr add(const std::string& testName, TFunction&& function, TData&& data)
        {
            auto test = std::make_shared<
                ParameterizedTest<
                decltype(std::function(std::forward<TFunction>(function))),
                typename decltype(std::function(std::forward<TData>(data)))::result_type>>(
                    std::function(std::forward<TFunction>(function)),
                    std::function(std::forward<TData>(data))(),
                    testName);

            m_tests.push_back(test);
            return test;
        }

        const std::string& name() const { return m_name; }
        const std::vector<TestPtr>& tests() const { return m_tests; }

        int passedTests() const
        {
            return std::accumulate(m_tests.begin(), m_tests.end(), 0, [](int sum, const auto& test) {
                const auto& result = test->result();
                return sum + static_cast<int>(std::count(result.data.begin(), result.data.end(), TestResultKind::Passed));
            });
        }

        int failedTests() const
        {
            return std::accumulate(m_tests.begin(), m_tests.end(), 0, [](int sum, const auto& test) {
                const auto& result = test->result();
                return sum + static_cast<int>(std::count(result.data.begin(), result.data.end(), TestResultKind::Failed));
            });
        }

        int skippedTests() const
        {
            return std::accumulate(m_tests.begin(), m_tests.end(), 0, [](int sum, const auto& test) {
                const auto& result = test->result();
                return sum + static_cast<int>(std::count(result.data.begin(), result.data.end(), TestResultKind::Skipped));
            });
        }

    private:
        std::string m_name;
        std::vector<TestPtr> m_tests;
    };

    using TestSuitePtr = std::shared_ptr<TestSuite>;

    std::string toSuiteName(const std::source_location& location);
}
