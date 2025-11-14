#include <CaraTest/TestRunnerConsoleOutput.h>
#include <CaraTest/Stringify.h>
#include <CaraTest/Diff.h>
#include <CaraTest/TestSuite.h>
#include <CaraTest/HelperExceptions.h>
#include <algorithm>
#include <iostream>
#include <source_location>
#include <numeric>

namespace
{
    using namespace CaraTest;

    const auto defaultConsoleWidth = 80;

    const auto resetAttributes = std::string("\033[0m");
    const auto greenColorSequence = std::string("\033[38;2;138;226;138m");
    const auto yellowColorSequence = std::string("\033[38;2;255;228;160m");
    const auto orangeColorSequence = std::string("\033[38;2;255;166;77m");
    const auto redColorSequence = std::string("\033[38;2;244;75;86m");
    const auto blueColorSequence = std::string("\033[38;2;42;129;211m");

    const auto moveCursorToPositionSequence = std::string("\033[%1;%2H");

    const auto coloredPass = std::format("{}PASS{}", greenColorSequence, resetAttributes);
    const auto coloredSkip = std::format("{}SKIP{}", yellowColorSequence, resetAttributes);
    const auto coloredFail = std::format("{}FAIL{}", redColorSequence, resetAttributes);

    const auto underlinedSequence = "\033[4m";
    const auto diffLineBreak = std::format("{}\n{}{}", resetAttributes, underlinedSequence, redColorSequence);

    static std::string TestNumber(int currentNumber, int totalCount, bool printNumber)
    {
        const auto totalCountString = std::to_string(totalCount);
        const auto countCharCount = totalCountString.length();
        const auto numberString = std::to_string(currentNumber);

        if (printNumber)
        {
            // Right-justify the number string
            return std::string(countCharCount > numberString.length() ? countCharCount - numberString.length() : 0, ' ') + numberString;
        }
        else
        {
            // Return a string of spaces with the same length as countCharCount
            return std::string(countCharCount, ' ');
        }
    }

    static std::string ResultNumber(int currentNumber, int totalCount)
    {
        const auto totalCountString = std::to_string(totalCount);
        const auto numberString = std::to_string(currentNumber);

        return std::format("{}/{}", numberString, totalCountString);
    }

    static std::string StringifyTestResult(TestResultKind result)
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
                return std::string("....");
        }
    }

    static std::string ColorDifferences(const std::string& input, const std::vector<DiffLocation>& differences, const std::string& colorSequence, DiffChange change)
    {
        std::vector<std::string> parts{};
        std::size_t lastIndex = 0;

        std::for_each(differences.begin(), differences.end(),
            [&](const DiffLocation& diff) 
            {
                if (diff.change != change)
                    return;

                const auto startIndex = diff.startIndex;
                parts.push_back(input.substr(lastIndex, startIndex - lastIndex));

                auto diffString = input.substr(startIndex, diff.endIndex - startIndex + 1);
                if (diffString == "\r")
                {
                    lastIndex = diff.endIndex + 1;
                    return;
                }
                else if (diffString == "\n")
                {
                    diffString = diffLineBreak;
                }

                parts.push_back(colorSequence);
                parts.push_back(underlinedSequence);
                parts.push_back(diffString);
                parts.push_back(resetAttributes);

                lastIndex = diff.endIndex + 1;
            });

        if (lastIndex < input.size())
            parts.push_back(input.substr(lastIndex, input.size() - lastIndex));

        return std::accumulate(parts.begin(), parts.end(), std::string());
    }

    static std::vector<DiffLocation> DiffAll(const std::string& first, const std::string& second)
    {
        std::vector<DiffLocation> differences;

        if (first.empty() || second.empty())
            return differences;

        if (first == second && first.empty())
            return differences;

        const int firstLength = first.size();
        const int secondLength = second.size();

        differences.push_back(DiffLocation{ .startIndex = 0, .endIndex = firstLength, .change = DiffChange::Addition });
        differences.push_back(DiffLocation{ .startIndex = 0, .endIndex = secondLength, .change = DiffChange::Deletion });

        return differences;
    }

    static void PrintDiff(
        const std::string& expectedValue,
        const std::string& actualValue,
        const std::source_location& location,
        ValueMismatchTestException::OutputMode outputMode)
    {
        std::cout << " " << location.file_name() << " Line:" << location.line() << '\n';

        std::vector<DiffLocation> differences;
        if (outputMode == ValueMismatchTestException::OutputMode::Diff)
        {
            differences = diff(expectedValue, actualValue);
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
        std::cout << expectedColoredOutput << '\n';

        std::cout << "   But got:  ";
        if (insertLinebreak)
            std::cout << '\n';
        std::cout << actualColoredOutput << '\n';
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

    static int SetupConsole()
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

    static Position CurrentCursorPosition()
    {
        CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
        GetConsoleScreenBufferInfo(g_windowConsoleHandle, &screenBuffer);
        return Position(screenBuffer.dwCursorPosition.X, screenBuffer.dwCursorPosition.Y);
    }

    static void MoveCursorToPosition(const Position& position)
    {
        COORD coord{ static_cast<SHORT>(position.x), static_cast<SHORT>(position.y) };
        SetConsoleCursorPosition(g_windowConsoleHandle, coord);
    }
}

#else

namespace
{
    static int SetupConsole()
    {
        return defaultConsoleWidth;
    }

    static Position CurrentCursorPosition()
    {
        return Position{};
    }

    static void MoveCursorToPosition(const Position& position)
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

    void TestRunnerConsoleOutput::writeSuiteName(const std::string& name)
    {
        if (name.empty())
            return;

        std::cout << "         --== " << name << " ==--\n";
    }

    Position TestRunnerConsoleOutput::writeTestHeader(int currentTest, int totalTests, const std::string& testName, bool hasSubTests)
    {
        std::cout << " " << TestNumber(currentTest, totalTests, !hasSubTests);
        if (hasSubTests)
        {
            std::cout << testName << '\n';
            return {};
        }
        const auto totalCountString = std::to_string(totalTests);
        const auto extraSpaces = totalCountString.length() - 1;

        std::cout << " " << testName << std::string(m_headerSize - (testName.size() + extraSpaces), '.') << " " << std::flush;

        const auto cursorPosition = CurrentCursorPosition();

        std::cout << "....\n";

        return cursorPosition;
    }

    Position TestRunnerConsoleOutput::writeSubTestHeader(int indentation, int currentTest, int totalTests, const std::string& parameters)
    {
        const auto indent = std::string(indentation + 2, ' ');

        const auto testNumber = TestNumber(currentTest, totalTests, true);
        const auto testNumberSize = testNumber.size();

        const auto spaceAndDashSize = 3;
        const auto testAndSubTestSizeDifference = testNumberSize - indent.size();
        const auto headerSize = m_headerSize - indent.size() - testAndSubTestSizeDifference - spaceAndDashSize;
        const auto truncatedParameters = parameters.substr(0, headerSize - 3);

        std::cout << indent << testNumber << " - " << std::string(headerSize - truncatedParameters.size(), '.') << truncatedParameters << " " << std::flush;

        const auto cursorPosition = CurrentCursorPosition();

        std::cout << "....\n";

        return cursorPosition;
    }

    void TestRunnerConsoleOutput::updateTestResult(const Position& position, TestResultKind result)
    {
        std::cout << std::flush;

        if (position.x == 0 && position.y == 0)
            return;

        const auto oldPosition = CurrentCursorPosition();

        MoveCursorToPosition(position);

        std::cout << StringifyTestResult(result) << std::flush;

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

        std::cout << '\n' << orangeColorSequence << "   Snapshot created: " << resetAttributes << e.filePath << '\n';
    }

    void TestRunnerConsoleOutput::writeTestRunnerResult(const TestSuiteResult& result)
    {
        std::cout
            << " " << coloredPass
            << " " << ResultNumber(result.passedTestCount, result.totalTestCount)
            << " " << coloredSkip
            << " " << ResultNumber(result.skippedTestCount, result.totalTestCount)
            << " " << coloredFail
            << " " << ResultNumber(result.failedTestCount, result.totalTestCount)
            << " TIME " << stringify(result.duration)
            << '\n';
    }

    void TestRunnerConsoleOutput::writeTestRunnerTotalResult(const std::vector<TestSuiteResult>& results)
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
