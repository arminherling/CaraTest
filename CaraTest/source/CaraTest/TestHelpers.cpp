#include <CaraTest/TestHelpers.h>
#include <CaraTest/TestRunner.h>
#include <unordered_map>

namespace CaraTest
{
    static std::unordered_map<std::string, TestSuitePtr> g_allTestSuites;

    TestSuitePtr getSuite(const std::string& name)
    {
        auto it = g_allTestSuites.find(name);
        if (it != g_allTestSuites.end())
        {
            return it->second;
        }

        auto newSuite = std::make_shared<TestSuite>(name);
        g_allTestSuites[name] = newSuite;
        return newSuite;
    }

    TestSuitePtr getSuite(const std::source_location& location)
    {
        const auto name = toSuiteName(location);
        return getSuite(name);
    }

    int runAll(int argc, char* argv[])
    {
        TestRunner runner{ argc, argv };
        std::vector<TestSuitePtr> suites;
        for (const auto& [name, suite] : g_allTestSuites)
        {
            suites.push_back(suite);
        }
        return runner.run(suites);
    }
}
