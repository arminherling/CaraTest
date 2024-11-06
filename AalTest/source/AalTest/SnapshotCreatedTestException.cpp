#include "SnapshotCreatedTestException.h"

namespace AalTest
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
