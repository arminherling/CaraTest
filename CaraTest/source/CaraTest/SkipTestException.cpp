#include "SkipTestException.h"

namespace CaraTest
{
    SkipTestException::SkipTestException(const std::source_location& sourceLocation)
        : location{ sourceLocation }
    {
    }
}
