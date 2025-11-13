#include "HelperExceptions.h"

namespace CaraTest
{
    FailedTestException::FailedTestException(const std::source_location& sourceLocation)
        : location{ sourceLocation }
    {
    }

    SkipTestException::SkipTestException(const std::source_location& sourceLocation)
        : location{ sourceLocation }
    {
    }

    ValueMismatchTestException::ValueMismatchTestException(
        const std::string& expected,
        const std::string& actual,
        const std::source_location& sourceLocation,
        OutputMode outputMode)
        : expectedValue{ expected }
        , actualValue{ actual }
        , location{ sourceLocation }
        , outputMode{ outputMode }
    {
    }

    SnapshotCreatedTestException::SnapshotCreatedTestException(
        const std::string& expected,
        const std::string& actual,
        const std::filesystem::path& filePath,
        const std::source_location& sourceLocation)
        : expectedValue{ expected }
        , actualValue{ actual }
        , filePath{ filePath }
        , location{ sourceLocation }
    {
    }
}