#include "Asserts.h"

#include <CaraTest/FailedTestException.h>
#include <CaraTest/SkipTestException.h>

namespace CaraTest
{
    void Fail(const std::source_location& location)
    {
        HANDLE_CARATEST_FAILURE();
        throw FailedTestException(location);
    }

    void Skip(const std::source_location& location)
    {
        throw SkipTestException(location);
    }

    void IsTrue(bool value, const std::source_location& location)
    {
        if (value != true)
        {
            HANDLE_CARATEST_FAILURE();
            throw ValueMismatchTestException(Stringify(true), Stringify(value), location, ValueMismatchTestException::OutputMode::All);
        }
    }

    void IsFalse(bool value, const std::source_location& location)
    {
        if (value != false)
        {
            HANDLE_CARATEST_FAILURE();
            throw ValueMismatchTestException(Stringify(false), Stringify(value), location, ValueMismatchTestException::OutputMode::All);
        }
    }
}
