#include "Asserts.h"

namespace CaraTest
{
    void fail(const std::source_location& location)
    {
        HANDLE_CARATEST_FAILURE();
        throw FailedTestException(location);
    }

    void skip(const std::source_location& location)
    {
        throw SkipTestException(location);
    }

    void isTrue(bool value, const std::source_location& location)
    {
        if (value != true)
        {
            HANDLE_CARATEST_FAILURE();
            throw ValueMismatchTestException(stringify(true), stringify(value), location, ValueMismatchTestException::OutputMode::All);
        }
    }

    void isFalse(bool value, const std::source_location& location)
    {
        if (value != false)
        {
            HANDLE_CARATEST_FAILURE();
            throw ValueMismatchTestException(stringify(false), stringify(value), location, ValueMismatchTestException::OutputMode::All);
        }
    }
}
