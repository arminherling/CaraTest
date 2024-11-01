#include "ValueMismatchTestException.h"

namespace AalTest
{
    ValueMismatchTestException::ValueMismatchTestException(
        const QString& expected,
        const QString& actual,
        const std::source_location& sourceLocation,
        OutputMode outputMode)
        : expectedValue{ expected }
        , actualValue{ actual }
        , location{ sourceLocation }
        , outputMode { outputMode }
    {
    }
}
