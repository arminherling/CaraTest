#pragma once

#include <CaraTest/API.h>
#include <CaraTest/TestSuite.h>

namespace 
{
    static std::string ToTestName(const std::source_location& location)
    {
        std::filesystem::path fullPath = location.file_name();
        auto name = fullPath.filename().string() + std::string(" - Line: ") + std::to_string(location.line());
        return name;
    }
}

namespace CaraTest
{
    /// <summary>
    /// Creates or retrieves a test suite with the given name.
    /// If a suite with the specified name does not exist, a new one is created and registered.
    /// </summary>
    CARATEST_API [[nodiscard]] TestSuitePtr getSuite(const std::string& name);
    
    /// <summary>
    /// Creates or retrieves a test suite based on the calling source location.
    /// If a suite with the specified name does not exist, a new one is created and registered.
    /// </summary>
    CARATEST_API [[nodiscard]] TestSuitePtr getSuite(const std::source_location& location = std::source_location::current());

    /// <summary>
    /// Runs all registered test suites and their tests.
    /// </summary>
    CARATEST_API [[nodiscard]] int runAll(int argc, char* argv[]);

    // Helper overloads for simple tests
    template<typename TFunction>
    [[nodiscard]] TestPtr addTest(const std::string& name, TFunction&& testFunction)
    {
        auto suite = getSuite("Global");
        return suite->add(name, std::forward<TFunction>(testFunction));
    }

    template<typename TFunction>
    [[nodiscard]] TestPtr addTest(TFunction&& testFunction, const std::source_location& location = std::source_location::current())
    {
        auto suite = getSuite(location);
        const auto name = ToTestName(location);
        return suite->add(name, std::forward<TFunction>(testFunction));
    }

    // Helper overloads for parameterized tests
    template<typename TFunction, typename TData>
    [[nodiscard]] TestPtr addTest(const std::string& name, TFunction&& function, TData&& data)
    {
        auto suite = getSuite("Global");
        return suite->add(name, std::forward<TFunction>(function), std::forward<TData>(data));
    }

    template<typename TFunction, typename TData>
    [[nodiscard]] TestPtr addTest(TFunction&& function, TData&& data, const std::source_location& location = std::source_location::current())
    {
        auto suite = getSuite(location);
        const auto name = ToTestName(location);
        return suite->add(name, std::forward<TFunction>(function), std::forward<TData>(data));
    }
}
