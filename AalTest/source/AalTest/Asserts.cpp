#include "Asserts.h"

#include <AalTest/FailedTestException.h>
#include <AalTest/SkipTestException.h>

namespace AalTest
{
    void Fail(const std::source_location& location)
    {
        HANDLE_AALTEST_FAILURE();
        throw FailedTestException(location);
    }

    void Skip(const std::source_location& location)
    {
        throw SkipTestException(location);
    }

    void IsTrue(bool value, const std::source_location& location)
    {
        AreEqual(true, value, location);
    }

    void IsFalse(bool value, const std::source_location& location)
    {
        AreEqual(false, value, location);
    }
}
