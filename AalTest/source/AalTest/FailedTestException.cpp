#include "FailedTestException.h"

namespace AalTest
{
    FailedTestException::FailedTestException(const std::source_location& sourceLocation)
        : location{ sourceLocation }
    {
    }
}