#pragma once

#include <CaraTest/API.h>
#include <exception>
#include <QString>
#include <source_location>

namespace CaraTest
{
    class CARATEST_API ValueMismatchTestException : public std::exception
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
