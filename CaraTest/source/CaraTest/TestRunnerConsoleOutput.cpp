#include "TestRunnerConsoleOutput.h"
#include <CaraTest/Stringify.h>
#include <CaraTest/Diff.h>

#include <QRegularExpression>

#include <algorithm>
#include <iostream>

namespace
{
    using namespace CaraTest;

    const auto defaultConsoleWidth = 80;

    const auto resetAttributes = QString("\033[0m");
    const auto greenColorSequence = QString("\033[38;2;138;226;138m");
    const auto yellowColorSequence = QString("\033[38;2;255;228;160m");
    const auto orangeColorSequence = QString("\033[38;2;255;166;77m");
    const auto redColorSequence = QString("\033[38;2;244;75;86m");
    const auto blueColorSequence = QString("\033[38;2;42;129;211m");

    const auto moveCursorToPositionSequence = QString("\033[%1;%2H");

    const auto coloredPass = QString("%1PASS%2").arg(greenColorSequence, resetAttributes);
    const auto coloredSkip = QString("%1SKIP%2").arg(yellowColorSequence, resetAttributes);
    const auto coloredFail = QString("%1FAIL%2").arg(redColorSequence, resetAttributes);

    const auto underlinedSequence = QString("\033[4m");
    const auto diffLineBreak = QString("%1\n%2%3").arg(resetAttributes, underlinedSequence, redColorSequence);
    const auto lineBreakRegex = QRegularExpression("[\r\n]");


    std::string TestNumber(int currentNumber, int totalCount, bool printNumber)
    {
        const auto totalCountString = QString::number(totalCount);
        const auto countCharCount = totalCountString.length();
        const auto numberString = QString::number(currentNumber);

        if (printNumber)
            return numberString.rightJustified(countCharCount, ' ').toStdString();
        else
            return QString().rightJustified(countCharCount, ' ').toStdString();
    }

    std::string ResultNumber(int currentNumber, int totalCount)
    {
        const auto totalCountString = QString::number(totalCount);
        const auto numberString = QString::number(currentNumber);

        return QString("%1/%2").arg(numberString, totalCountString).toStdString();
    }

    QString StringifyTestResult(TestResultKind result)
    {
        switch (result)
        {
            case TestResultKind::Skipped:
                return coloredSkip;
            case TestResultKind::Failed:
                return coloredFail;
            case TestResultKind::Passed:
                return coloredPass;
            case TestResultKind::Invalid:
            default:
                return QString("....");
        }
    }

    QString ColorDifferences(const QString& input, const QList<DiffLocation>& differences, const QString& colorSequence, DiffChange change)
    {
        QStringList parts{};
        int lastIndex = input.size();

        std::for_each(differences.rbegin(), differences.rend(),
            [&](const DiffLocation& diff) {
                if (diff.change != change)
                    return;

                const auto endIndex = diff.endIndex + 1;
                parts.prepend(input.mid(endIndex, lastIndex - endIndex));

                auto diffString = input.mid(diff.startIndex, endIndex - diff.startIndex);
                
                if (diffString == '\r')
                {
                    lastIndex = diff.startIndex;
                    return;
                }
                else if (diffString == '\n')
                {
                    diffString = diffLineBreak;
                }

                parts.prepend(resetAttributes);
                parts.prepend(diffString);
                parts.prepend(colorSequence);
                parts.prepend(underlinedSequence);

                lastIndex = diff.startIndex;
            });

        if (lastIndex != 0)
            parts.prepend(input.mid(0, lastIndex));

        return parts.join(QString());
    }

    QList<DiffLocation> DiffAll(const QString& first, const QString& second)
    {
        QList<DiffLocation> differences;

        if (first.isNull() || second.isNull())
            return differences;

        if (first == second && first.isEmpty())
            return differences;

        const int firstLength = first.size();
        const int secondLength = second.size();

        differences.append(DiffLocation{ .startIndex = 0, .endIndex = firstLength, .change = DiffChange::Addition });
        differences.append(DiffLocation{ .startIndex = 0, .endIndex = secondLength, .change = DiffChange::Deletion });

        return differences;
    }

    void PrintDiff(
        const QString& expectedValue,
        const QString& actualValue,
        const std::source_location& location,
        ValueMismatchTestException::OutputMode outputMode)
    {
        std::cout << " " << location.file_name() << " Line:" << location.line() << '\n';

        QList<DiffLocation> differences;
        if (outputMode == ValueMismatchTestException::OutputMode::Diff)
        {
            differences = Diff(expectedValue, actualValue);
        }
        else
        {
            differences = DiffAll(expectedValue, actualValue);
        }

        const auto expectedColoredOutput = ColorDifferences(expectedValue, differences, blueColorSequence, DiffChange::Deletion);
        const auto actualColoredOutput = ColorDifferences(actualValue, differences, redColorSequence, DiffChange::Addition);

        const auto length = std::max(expectedValue.size(), actualValue.size());
        const auto insertLinebreak = (length > 20);

        std::cout << "   Expected: ";
        if (insertLinebreak)
            std::cout << '\n';
        std::cout << expectedColoredOutput.toStdString() << '\n';

        std::cout << "   But got:  ";
        if (insertLinebreak)
            std::cout << '\n';
        std::cout << actualColoredOutput.toStdString() << '\n';
    }
}

#if defined(_WIN32) || defined(_WIN64)

namespace
{
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

    void* g_windowConsoleHandle = nullptr;

    int SetupConsole()
    {
        g_windowConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleOutputCP(CP_UTF8);
        SetConsoleMode(g_windowConsoleHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo{};
        GetConsoleScreenBufferInfo(g_windowConsoleHandle, &screenBufferInfo);
        const auto consoleWidth = screenBufferInfo.dwSize.X;
        if (consoleWidth == 0)
            return defaultConsoleWidth;

        return consoleWidth - 12;
    }

    QPoint CurrentCursorPosition()
    {
        CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
        GetConsoleScreenBufferInfo(g_windowConsoleHandle, &screenBuffer);
        return QPoint(screenBuffer.dwCursorPosition.X, screenBuffer.dwCursorPosition.Y);
    }

    void MoveCursorToPosition(const QPoint& position)
    {
        std::cout << moveCursorToPositionSequence.arg(position.y() + 1).arg(position.x() + 1).toStdString();
        //COORD coord{ static_cast<SHORT>(position.x()), static_cast<SHORT>(position.y()) };
        //SetConsoleCursorPosition(g_windowConsoleHandle, coord);
    }
}

#else

namespace
{
    int SetupConsole()
    {
        return defaultConsoleWidth;
    }

    QPoint CurrentCursorPosition()
    {
        return QPoint();
    }

    void MoveCursorToPosition(const QPoint& position)
    {
    }
}

#endif


namespace CaraTest
{
    TestRunnerConsoleOutput::TestRunnerConsoleOutput()
    {
        m_headerSize = SetupConsole();
    }

    void TestRunnerConsoleOutput::writeSuiteName(const QString& name)
    {
        if (name.isEmpty())
            return;

        std::cout << "         --== " << name.toStdString() << " ==--\n";
    }

    QPoint TestRunnerConsoleOutput::writeTestHeader(int currentTest, int totalTests, const QString& testName, bool hasSubTests)
    {
        std::cout << " " << TestNumber(currentTest, totalTests, !hasSubTests);
        if (hasSubTests)
        {
            std::cout << testName.toStdString() << '\n';
            return {};
        }

        std::cout << " " << testName.leftJustified(m_headerSize, '.').toStdString() << " " << std::flush;

        const auto cursorPosition = CurrentCursorPosition();

        std::cout << "....\n";

        return cursorPosition;
    }

    QPoint TestRunnerConsoleOutput::writeSubTestHeader(int indentation, int currentTest, int totalTests, const QString& parameters)
    {
        const auto indent = QString(" ").repeated(indentation + 2);

        const auto testNumber = TestNumber(currentTest, totalTests, true);
        const auto testNumberSize = testNumber.size();

        const auto spaceAndDashSize = 3;
        const auto testAndSubTestSizeDifference = testNumberSize - indent.size();
        const auto headerSize = m_headerSize - indent.size() - testAndSubTestSizeDifference - spaceAndDashSize;
        const auto truncatedParameters = parameters.left(headerSize - 3);

        std::cout << indent.toStdString() << testNumber << " - " << truncatedParameters.leftJustified(headerSize, '.').toStdString() << " " << std::flush;

        const auto cursorPosition = CurrentCursorPosition();

        std::cout << "....\n";

        return cursorPosition;
    }

    void TestRunnerConsoleOutput::updateTestResult(const QPoint& position, TestResultKind result)
    {
        std::cout << std::flush;

        if (position.isNull())
            return;

        const auto oldPosition = CurrentCursorPosition();

        //const auto cursorPosition = COORD{ (short)position.x(), (short)position.y() };
        //SetConsoleCursorPosition(m_consoleHandle, cursorPosition);
        MoveCursorToPosition(position);

        std::cout << StringifyTestResult(result).toStdString() << std::flush;

        //SetConsoleCursorPosition(m_consoleHandle, oldPosition);
        MoveCursorToPosition(oldPosition);
    }

    void TestRunnerConsoleOutput::writeTestPassedMessage()
    {
    }

    void TestRunnerConsoleOutput::writeTestSkippedMessage(SkipTestException& e)
    {
    }

    void TestRunnerConsoleOutput::writeTestFailedMessage(FailedTestException& e)
    {
    }

    void TestRunnerConsoleOutput::writeTestValueMismatchMessage(ValueMismatchTestException& e)
    {
        PrintDiff(e.expectedValue, e.actualValue, e.location, e.outputMode);
    }

    void TestRunnerConsoleOutput::writeSnapshotCreatedMessage(SnapshotCreatedTestException& e)
    {
        PrintDiff(e.expectedValue, e.actualValue, e.location, ValueMismatchTestException::OutputMode::Diff);

        std::cout << '\n' << orangeColorSequence.toStdString() << "   Snapshot created: " << resetAttributes.toStdString() << e.filePath.toStdString() << '\n';
    }

    void TestRunnerConsoleOutput::writeTestRunnerResult(const TestSuiteResult& result)
    {
        std::cout
            << " " << coloredPass.toStdString()
            << " " << ResultNumber(result.passedTestCount, result.totalTestCount)
            << " " << coloredSkip.toStdString()
            << " " << ResultNumber(result.skippedTestCount, result.totalTestCount)
            << " " << coloredFail.toStdString()
            << " " << ResultNumber(result.failedTestCount, result.totalTestCount)
            << " TIME " << Stringify(result.duration).toStdString()
            << '\n';
    }

    void TestRunnerConsoleOutput::writeTestRunnerTotalResult(const QList<TestSuiteResult>& results)
    {
        TestSuiteResult totalResult{};
        for (const auto& result : results)
        {
            totalResult.passedTestCount += result.passedTestCount;
            totalResult.skippedTestCount += result.skippedTestCount;
            totalResult.failedTestCount += result.failedTestCount;
            totalResult.totalTestCount += result.totalTestCount;
            totalResult.duration += result.duration;
        }
        std::cout << "                  ==== Total Result ====\n";
        writeTestRunnerResult(totalResult);
    }

    void TestRunnerConsoleOutput::writeEmptyLine()
    {
        std::cout << '\n';
    }
}
