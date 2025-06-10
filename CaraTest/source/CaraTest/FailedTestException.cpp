#include "FailedTestException.h"

namespace CaraTest
{
    FailedTestException::FailedTestException(const std::source_location& sourceLocation)
        : location{ sourceLocation }
    {
    }
}