#include "Asserts.h"

#include <AalTest/FailedTestException.h>
#include <AalTest/SkipTestException.h>

namespace AalTest
{
    void Fail(const std::source_location& location)
    {
        throw FailedTestException(location);
    }

    void Skip(const std::source_location& location)
    {
        throw SkipTestException(location);
    }

    void IsTrue(bool value, const std::source_location& location)
    {
        if (!value)
            throw ValueMismatchTestException(Stringify(true), Stringify(value), location);
    }

    void IsFalse(bool value, const std::source_location& location)
    {
        if (value)
            throw ValueMismatchTestException(Stringify(false), Stringify(value), location);
    }
}
