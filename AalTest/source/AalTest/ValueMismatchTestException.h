#pragma once

#include <AalTest/API.h>
#include <exception>
#include <QString>
#include <source_location>

namespace AalTest
{
    class AALTEST_API ValueMismatchTestException : public std::exception
    {
    public:
        enum class OutputMode
        {
            Diff,
            All
        };

        ValueMismatchTestException(
            const QString& expected, 
            const QString& actual, 
            const std::source_location& sourceLocation, 
            OutputMode outputMode);

        QString expectedValue;
        QString actualValue;
        std::source_location location;
        OutputMode outputMode;
    };
}
