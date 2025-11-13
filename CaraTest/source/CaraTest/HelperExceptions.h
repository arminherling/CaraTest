#pragma once

#include <CaraTest/API.h>
#include <exception>
#include <source_location>
#include <string>
#include <filesystem>

namespace CaraTest
{
    class FailedTestException : public std::exception
    {
    public:
        FailedTestException(const std::source_location& sourceLocation);

        std::source_location location;
    };

    class SkipTestException : public std::exception
    {
    public:
        SkipTestException(const std::source_location& sourceLocation);

        std::source_location location;
    };
    
    class CARATEST_API ValueMismatchTestException : public std::exception
    {
    public:
        enum class OutputMode
        {
            Diff,
            All
        };

        ValueMismatchTestException(
            const std::string& expected,
            const std::string& actual,
            const std::source_location& sourceLocation,
            OutputMode outputMode);

        std::string expectedValue;
        std::string actualValue;
        std::source_location location;
        OutputMode outputMode;
    };

    class CARATEST_API SnapshotCreatedTestException : public std::exception
    {
    public:
        SnapshotCreatedTestException(
            const std::string& expected,
            const std::string& actual,
            const std::filesystem::path& filePath,
            const std::source_location& sourceLocation);

        std::string expectedValue;
        std::string actualValue;
        std::filesystem::path filePath;
        std::source_location location;
    };
}
