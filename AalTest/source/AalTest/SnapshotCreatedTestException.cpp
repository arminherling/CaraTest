#include "SnapshotCreatedTestException.h"

namespace AalTest
{
    SnapshotCreatedTestException::SnapshotCreatedTestException(
        const QString& filePath,
        const std::source_location& sourceLocation)
        : filePath{ filePath }
        , location{ sourceLocation }
    {
    }
}
