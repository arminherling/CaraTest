#include "TestRunnerWindowsConsoleOutput.h"
#include <AalTest/Stringify.h>
#include <AalTest/Diff.h>

#include <QRegularExpression>

#include <algorithm>
#include <iostream>

#define NOMINMAX
#include <Windows.h>

namespace
{
    using namespace AalTest;

    const auto resetAttributes = "\033[0m";
    const auto greenColorSequence = "\033[38;2;138;226;138m";
    const auto yellowColorSequence = "\033[38;2;255;228;160m";
    const auto redColorSequence = "\033[38;2;244;75;86m";

    const auto coloredPass = QString("%1PASS%2").arg(greenColorSequence, resetAttributes);
    const auto coloredSkip = QString("%1SKIP%2").arg(yellowColorSequence, resetAttributes);
    const auto coloredFail = QString("%1FAIL%2").arg(redColorSequence, resetAttributes);

    const auto underlinedSequence = "\033[4m";
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

    QString ColorDifferences(const QString& input, const QList<DiffLocation>& differences)
    {
        QStringList parts{};
        int lastIndex = input.size();

        std::for_each(differences.rbegin(), differences.rend(), 
            [&](const DiffLocation& diff) {

                parts.prepend(input.mid(diff.endIndex, lastIndex - diff.endIndex));

                auto diffString = input.mid(diff.startIndex, diff.endIndex - diff.startIndex);
                // we need to reset the console attributes and reapply after the linebreak to prevent a visual bug
                diffString.replace(lineBreakRegex, diffLineBreak);

                parts.prepend(resetAttributes);
                parts.prepend(diffString);
                parts.prepend(redColorSequence);
                parts.prepend(underlinedSequence);

                lastIndex = diff.startIndex;
            });

        if(lastIndex != 0)
            parts.prepend(input.mid(0, lastIndex));

        return parts.join(QString());
    }

    COORD CurrentCursorPosition(void* consoleHandle)
    {
        CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
        GetConsoleScreenBufferInfo(consoleHandle, &screenBuffer);
        return screenBuffer.dwCursorPosition;
    }
}

namespace AalTest
{
    TestRunnerWindowsConsoleOutput::TestRunnerWindowsConsoleOutput()
        : m_consoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
        , m_oldConsoleOutputCodePage{ GetConsoleOutputCP() }
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleMode(m_consoleHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo{};
        GetConsoleScreenBufferInfo(m_consoleHandle, &screenBufferInfo);
        const auto consoleWidth = screenBufferInfo.dwSize.X;
        m_headerSize = consoleWidth - 12;
    }

    void TestRunnerWindowsConsoleOutput::writeSuiteName(const QString& name)
    {
        if (name.isEmpty())
            return;

        std::cout << "         --== " << name.toStdString() << " ==--\n";
    }

    QPoint TestRunnerWindowsConsoleOutput::writeTestHeader(int currentTest, int totalTests, const QString& testName, bool hasSubTests)
    {
        std::cout << " " << TestNumber(currentTest, totalTests, !hasSubTests);
        if (hasSubTests)
        {
            std::cout << testName.toStdString() << '\n';
            return {};
        }

        std::cout << " " << testName.leftJustified(m_headerSize, '.').toStdString() << " " << std::flush;

        const auto cursorPosition = CurrentCursorPosition(m_consoleHandle);

        std::cout << "....\n";

        return { cursorPosition.X, cursorPosition.Y };
    }

    QPoint TestRunnerWindowsConsoleOutput::writeSubTestHeader(int indentation, int currentTest, int totalTests, const QString& parameters)
    {
        const auto indent = QString(" ").repeated(indentation + 2);

        const auto testNumber = TestNumber(currentTest, totalTests, true);
        const auto testNumberSize = testNumber.size();

        const auto spaceAndDashSize = 3;
        const auto testAndSubTestSizeDifference = testNumberSize - indent.size();
        const auto headerSize = m_headerSize - indent.size() - testAndSubTestSizeDifference - spaceAndDashSize;
        const auto truncatedParameters = parameters.left(headerSize - 3);

        std::cout << indent.toStdString() << testNumber << " - " << truncatedParameters.leftJustified(headerSize, '.').toStdString() << " " << std::flush;

        const auto cursorPosition = CurrentCursorPosition(m_consoleHandle);

        std::cout << "....\n";

        return { cursorPosition.X, cursorPosition.Y };
    }

    void TestRunnerWindowsConsoleOutput::updateTestResult(const QPoint& position, TestResultKind result)
    {
        std::cout << std::flush;

        if (position.isNull())
            return;

        const auto oldPosition = CurrentCursorPosition(m_consoleHandle);

        const auto cursorPosition = COORD{ (short)position.x(), (short)position.y()};
        SetConsoleCursorPosition(m_consoleHandle, cursorPosition);

        std::cout << StringifyTestResult(result).toStdString() << std::flush;

        SetConsoleCursorPosition(m_consoleHandle, oldPosition);
    }

    void TestRunnerWindowsConsoleOutput::writeTestPassedMessage()
    {
    }

    void TestRunnerWindowsConsoleOutput::writeTestSkippedMessage(SkipTestException& e)
    {
    }

    void TestRunnerWindowsConsoleOutput::writeTestFailedMessage(FailedTestException& e)
    {
    }

    void TestRunnerWindowsConsoleOutput::writeTestValueMismatchMessage(ValueMismatchTestException& e)
    {
        std::cout << " " << e.location.file_name() << " Line:" << e.location.line() << '\n';

        const auto differences = Diff(e.expectedValue, e.actualValue);

        const auto coloredOutput = ColorDifferences(e.actualValue, differences);

        const auto length = std::max(e.expectedValue.size(), e.actualValue.size());
        const auto insertLinebreak = (length > 20);

        std::cout << "   Expected: ";
        if (insertLinebreak)
            std::cout << '\n';
        std::cout << e.expectedValue.toStdString() << '\n';

        std::cout << "   But got:  ";
        if (insertLinebreak)
            std::cout << '\n';
        std::cout << coloredOutput.toStdString() << '\n';
    }

    void TestRunnerWindowsConsoleOutput::writeTestRunnerResult(const TestSuiteResult& result)
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

    void TestRunnerWindowsConsoleOutput::writeTestRunnerTotalResult(const QList<TestSuiteResult>& results)
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

    void TestRunnerWindowsConsoleOutput::writeEmptyLine()
    {
        std::cout << '\n';
    }
}
