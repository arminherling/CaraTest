#include "SkipTestException.h"

namespace AalTest
{
    SkipTestException::SkipTestException(const std::source_location& sourceLocation)
        : location{ sourceLocation }
    {
    }
}
