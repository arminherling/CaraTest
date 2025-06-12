#pragma once

#include <CaraTest/API.h>
#include <CaraTest/FailureBehavior.h>
#include <CaraTest/Stringify.h>
#include <CaraTest/ValueMismatchTestException.h>
#include <CaraTest/SnapshotCreatedTestException.h>
#include <CaraTest/File.h>

#include <QString>
#include <QFileInfo>
#include <QDir>

#include <source_location>

namespace CaraTest
{
    CARATEST_API void Fail(const std::source_location& location = std::source_location::current());
    CARATEST_API void Skip(const std::source_location& location = std::source_location::current());
    CARATEST_API void IsTrue(bool value, const std::source_location& location = std::source_location::current());
    CARATEST_API void IsFalse(bool value, const std::source_location& location = std::source_location::current());

    template<class T1, class T2>
    void AreEqual(
        T1&& expectedValue, 
        T2&& actualValue, 
        const std::source_location& location = std::source_location::current())
    {
        if (expectedValue != actualValue)
        {
            HANDLE_CARATEST_FAILURE();
            throw ValueMismatchTestException(
                StringifyAndMaybeQuote(expectedValue),
                StringifyAndMaybeQuote(actualValue),
                location,
                ValueMismatchTestException::OutputMode::Diff
            );
        }
    }

    template<class T1>
    void EqualsFile(
        const QFileInfo& expectedContentFilePath, 
        T1&& actualValue, 
        const std::source_location& location = std::source_location::current())
    {
        const auto stringifiedActualValue = Stringify(actualValue);
        if (!expectedContentFilePath.exists())
        {
            const auto snapshotFilePath = QDir::cleanPath(expectedContentFilePath.absoluteFilePath() + QString(".snapshot"));
            File::WriteContent(snapshotFilePath, stringifiedActualValue);

            HANDLE_CARATEST_FAILURE();
            throw SnapshotCreatedTestException(QString(), StringifyAndMaybeQuote(actualValue), snapshotFilePath, location);
        }

        const auto fileContent = File::ReadContent(expectedContentFilePath);
        if (actualValue != fileContent)
        {
            const auto snapshotFilePath = QDir::cleanPath(expectedContentFilePath.absoluteFilePath() + QString(".snapshot"));
            File::WriteContent(snapshotFilePath, stringifiedActualValue);

            HANDLE_CARATEST_FAILURE();
            throw SnapshotCreatedTestException(StringifyAndMaybeQuote(fileContent), StringifyAndMaybeQuote(actualValue), snapshotFilePath, location);
        }
    }
}

#define CARATEST_TRIGGER_BREAK_ON_MATCH(firstValue, secondValue)                                                    \
    do {                                                                                                            \
        if ((firstValue) == (secondValue)) {                                                                        \
            TRIGGER_DEBUG_BREAK();                                                                                  \
        } else  {                                                                                                   \
            const auto _firstStringified = StringifyAndMaybeQuote(firstValue);                                      \
            const auto _secondStringified = StringifyAndMaybeQuote(secondValue);                                    \
            if (_firstStringified.contains(_secondStringified) || _secondStringified.contains(_firstStringified)) { \
                TRIGGER_DEBUG_BREAK();                                                                              \
            }                                                                                                       \
        }                                                                                                           \
    } while(0)
