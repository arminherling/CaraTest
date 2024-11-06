#pragma once

#include <AalTest/API.h>
#include <AalTest/FailureBehavior.h>
#include <AalTest/Stringify.h>
#include <AalTest/ValueMismatchTestException.h>
#include <AalTest/SnapshotCreatedTestException.h>
#include <AalTest/File.h>

#include <QString>
#include <QFileInfo>

#include <source_location>

namespace AalTest 
{
    AALTEST_API void Fail(const std::source_location& location = std::source_location::current());
    AALTEST_API void Skip(const std::source_location& location = std::source_location::current());
    AALTEST_API void IsTrue(bool value, const std::source_location& location = std::source_location::current());
    AALTEST_API void IsFalse(bool value, const std::source_location& location = std::source_location::current());

    template<class T1, class T2>
    void AreEqual(T1&& expectedValue, T2&& actualValue, const std::source_location& location = std::source_location::current())
    {
        if (expectedValue != actualValue)
        {
            HANDLE_AALTEST_FAILURE();
            throw ValueMismatchTestException(Stringify(expectedValue, true), Stringify(actualValue, true), location, ValueMismatchTestException::OutputMode::Diff);
        }
    }

    template<class T1>
    void EqualsFile(T1&& expectedValue, const QFileInfo& filePath, const std::source_location& location = std::source_location::current())
    {
        const auto stringifiedExpectedValue = Stringify(expectedValue, false);

        if (!filePath.exists())
        {
            const auto snapshotFilePath = QDir::cleanPath(filePath.absoluteFilePath() + QString(".snapshot"));
            WriteFileContent(snapshotFilePath, stringifiedExpectedValue);

            HANDLE_AALTEST_FAILURE();
            throw SnapshotCreatedTestException(Stringify(expectedValue, true), QString(), snapshotFilePath, location);
        }

        const auto fileContent = ReadFileContent(filePath);
        if (expectedValue != fileContent)
        {
            const auto snapshotFilePath = QDir::cleanPath(filePath.absoluteFilePath() + QString(".snapshot"));
            WriteFileContent(snapshotFilePath, stringifiedExpectedValue);

            HANDLE_AALTEST_FAILURE();
            throw SnapshotCreatedTestException(Stringify(expectedValue, true), Stringify(fileContent, true), snapshotFilePath, location);
        }
    }
}
