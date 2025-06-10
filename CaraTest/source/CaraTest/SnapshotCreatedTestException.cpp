#include "SnapshotCreatedTestException.h"

namespace CaraTest
{
    SnapshotCreatedTestException::SnapshotCreatedTestException(
        const QString& expected,
        const QString& actual,
        const QString& filePath,
        const std::source_location& sourceLocation)
        : expectedValue{ expected }
        , actualValue{ actual }
        , filePath{ filePath }
        , location{ sourceLocation }
    {
    }
}
