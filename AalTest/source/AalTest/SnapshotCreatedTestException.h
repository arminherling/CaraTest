#pragma once

#include <AalTest/API.h>
#include <exception>
#include <QString>
#include <source_location>

namespace AalTest
{
    class AALTEST_API SnapshotCreatedTestException : public std::exception
    {
    public:
        SnapshotCreatedTestException(
            const QString& expected,
            const QString& actual,
            const QString& filePath,
            const std::source_location& sourceLocation);

        QString expectedValue;
        QString actualValue;
        QString filePath;
        std::source_location location;
    };
}
