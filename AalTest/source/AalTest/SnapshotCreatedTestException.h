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
        SnapshotCreatedTestException(const QString& filePath, const std::source_location& sourceLocation);

        QString filePath;
        std::source_location location;
    };
}
