#include "AalTestTests.h"
#include <AalTest.h>

#include <QDir>
#include <QRandomGenerator>

using namespace AalTest;

namespace Simple
{
    static void TestCanExecute()
    {
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};
        int i = 0;
        suite.add(QString(), [&i]() {i++; });

        runner.run(suite);

        AalTest::AreEqual(i, 1);
    }

    static void TestCanPass()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                // do nothing
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestCanFail()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                AalTest::Fail();
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestCanBeSkipped()
    {
        int expectedPasses = 0;
        int expectedFails = 0;
        int expectedSkips = 1;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                AalTest::Skip();
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestSuitesReturnsAddedTests()
    {
        auto expectedTestCount = 2;
        TestSuite suite{};
        suite.add(QString(), []()
            {
                AalTest::Skip();
            });
        suite.add(QString(), []()
            {
                AalTest::Fail();
            });

        AalTest::AreEqual(expectedTestCount, (int)suite.tests().size());
    }

    static void TestContainsCallingTestName()
    {
        QString expectedName = QString("TestContainsCallingTestName");
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};
        suite.add(QString("TestContainsCallingTestName"), []()
            {
                AalTest::Skip();
            });

        auto& test = suite.tests().at(0);

        AalTest::AreEqual(expectedName, test->testName());
    }

    static void TestIsTrueWhenTrue()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = true;
                AalTest::IsTrue(value);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestIsTrueWhenFalse()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = false;
                AalTest::IsTrue(value);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestIsFalseWhenFalse()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = false;
                AalTest::IsFalse(value);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestIsFalseWhenTrue()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = true;
                AalTest::IsFalse(value);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenTrueAndTrue()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = true;
                auto actualValue = true;
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenFalseAndFalse()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = false;
                auto actualValue = false;
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenTrueAndFalse()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = true;
                auto actualValue = false;
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenZeroAndZero()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = 0;
                auto actualValue = 0;
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenOneAndOne()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = 1;
                auto actualValue = 1;
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenOneAndZero()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = 1;
                auto actualValue = 0;
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    void TestAreEqualWhenEmptyStringAndEmptyString()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = QString();
                auto actualValue = QString();
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    void TestAreEqualWhenStringAndSameString()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = QString("string");
                auto actualValue = QString("string");
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenStringAndDifferentString()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = QString("string");
                auto actualValue = QString("other");
                AalTest::AreEqual(expectedValue, actualValue);
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestRunnerExecutesTests()
    {
        int expectedPasses = 1;
        int expectedFails = 1;
        int expectedSkips = 1;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};
        suite.add(QString(), []()
            {
            });
        suite.add(QString(), []()
            {
                AalTest::Skip();
            });
        suite.add(QString(), []()
            {
                AalTest::Fail();
            });

        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }
}

namespace Parameterized
{
    static void TestCanExecuteWithParameters(bool data)
    {
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};
        int i = 0;
        suite.add(QString(), [&i]() {i++; });

        runner.run(suite);

        AalTest::AreEqual(i, 1);
    }

    static QList<std::tuple<bool>> TestCanExecuteWithParameters_Data()
    {
        return { std::make_tuple(true), std::make_tuple(false) };
    }

    static void TestWithTwoEmptySubTestsHasTwoPasses()
    {
        int expectedPasses = 2;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                // do nothing
            }, 
            []() 
            { 
                return QList{ std::make_tuple(true), std::make_tuple(false) }; 
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithSkipOnTrueInThreeSubTestsHasTwoPassesAndOneSkip()
    {
        int expectedPasses = 2;
        int expectedFails = 0;
        int expectedSkips = 1;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                if (value)
                    AalTest::Skip();
            },
            []()
            {
                return QList{ std::make_tuple(true), std::make_tuple(false), std::make_tuple(false) };
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithFailInTwoSubTestsHasTwoFails()
    {
        int expectedPasses = 0;
        int expectedFails = 2;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                AalTest::Fail();
            },
            []()
            {
                return QList{ std::make_tuple(false), std::make_tuple(false) };
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithIsTrueCheckInTwoSubTestsHasOnePassAndOneFails()
    {
        int expectedPasses = 1;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                AalTest::IsTrue(value);
            },
            []()
            {
                return QList{ std::make_tuple(true), std::make_tuple(false) };
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithSkipOnTrueAndFailHasOneSkipAndTwoFails()
    {
        int expectedPasses = 0;
        int expectedFails = 2;
        int expectedSkips = 1;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                if (value)
                    AalTest::Skip();

                AalTest::Fail();
            },
            []()
            {
                return QList{ std::make_tuple(false), std::make_tuple(true), std::make_tuple(false) };
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithSkipInTwoSubTestsHasTwoSkips()
    {
        int expectedPasses = 0;
        int expectedFails = 0;
        int expectedSkips = 2;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                AalTest::Skip();
            },
            []()
            {
                return QList{ std::make_tuple(true), std::make_tuple(false) };
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithoutDataDontExecute()
    {
        int expectedPasses = 0;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                // do nothing
            },
            []()
            {
                return QList<std::tuple<bool>>{};
            });
        runner.run(suite);

        AalTest::AreEqual(expectedPasses, suite.passedTests());
        AalTest::AreEqual(expectedFails, suite.failedTests());
        AalTest::AreEqual(expectedSkips, suite.skippedTests());
    }
}

static QString GenerateRandomFilePath()
{
    const auto value = QRandomGenerator::global()->generate();
    const auto testFilePath = QDir::cleanPath(QDir::tempPath() + QString("/test_file_%1.test").arg(value));
    return testFilePath;
}

static void EqualsFileCreatesSnapshotWhenFileDoesNotExist()
{
    int expectedPasses = 0;
    int expectedFails = 1;
    int expectedSkips = 0;
    TestRunner runner{ TestRunner::OutputMode::None };
    TestSuite suite{};

    const auto testFilePath = GenerateRandomFilePath();

    suite.add(QString(), [=]()
        {
            AalTest::EqualsFile(QString("TestValue"), QFileInfo(testFilePath));
        });
    runner.run(suite);

    const auto snapshotFilePath = QDir::cleanPath(testFilePath + QString(".snapshot"));
    auto snapshotFile = QFile(snapshotFilePath);
    
    const auto fileExists = snapshotFile.exists();
    const auto wasRemoved = snapshotFile.remove();

    AalTest::IsTrue(fileExists);
    AalTest::IsTrue(wasRemoved);
    AalTest::AreEqual(expectedPasses, suite.passedTests());
    AalTest::AreEqual(expectedFails, suite.failedTests());
    AalTest::AreEqual(expectedSkips, suite.skippedTests());
}

static void EqualsFileDoesntCreateSnapshotWhenValuesAreEqual()
{
    int expectedPasses = 1;
    int expectedFails = 0;
    int expectedSkips = 0;
    QString expectedValue("TestValue");

    TestRunner runner{ TestRunner::OutputMode::None };
    TestSuite suite{};

    const auto testFilePath = GenerateRandomFilePath();
    AalTest::WriteFileContent(testFilePath, expectedValue);

    suite.add(QString(), [=]()
        {
            AalTest::EqualsFile(expectedValue, QFileInfo(testFilePath));
        });
    runner.run(suite);

    const auto snapshotFilePath = QDir::cleanPath(testFilePath + QString(".snapshot"));
    auto snapshotFile = QFile(snapshotFilePath);

    const auto fileExists = snapshotFile.exists();
    const auto wasRemoved = snapshotFile.remove();

    AalTest::IsFalse(fileExists);
    AalTest::IsFalse(wasRemoved);
    AalTest::AreEqual(expectedPasses, suite.passedTests());
    AalTest::AreEqual(expectedFails, suite.failedTests());
    AalTest::AreEqual(expectedSkips, suite.skippedTests());
}

static void EqualsFileCreateSnapshotWhenValuesAreNotEqual()
{
    int expectedPasses = 0;
    int expectedFails = 1;
    int expectedSkips = 0;
    QString expectedValue("TestValue");

    TestRunner runner{ TestRunner::OutputMode::None };
    TestSuite suite{};

    const auto testFilePath = GenerateRandomFilePath();
    AalTest::WriteFileContent(testFilePath, expectedValue);

    suite.add(QString(), [=]()
        {
            AalTest::EqualsFile(QString("Some other Value"), QFileInfo(testFilePath));
        });
    runner.run(suite);

    const auto snapshotFilePath = QDir::cleanPath(testFilePath + QString(".snapshot"));
    auto snapshotFile = QFile(snapshotFilePath);

    const auto fileExists = snapshotFile.exists();
    const auto wasRemoved = snapshotFile.remove();

    AalTest::IsTrue(fileExists);
    AalTest::IsTrue(wasRemoved);
    AalTest::AreEqual(expectedPasses, suite.passedTests());
    AalTest::AreEqual(expectedFails, suite.failedTests());
    AalTest::AreEqual(expectedSkips, suite.skippedTests());
}

TestSuite AalTestTestsSuiteSimple()
{
    using namespace Simple;

    TestSuite suite{};
    suite.add(QString("TestCanExecute"), TestCanExecute);
    suite.add(QString("TestCanPass"), TestCanPass);
    suite.add(QString("TestCanFail"), TestCanFail);
    suite.add(QString("TestCanBeSkipped"), TestCanBeSkipped);
    suite.add(QString("TestSuitesReturnsAddedTests"), TestSuitesReturnsAddedTests);
    suite.add(QString("TestContainsCallingTestName"), TestContainsCallingTestName);
    suite.add(QString("TestIsTrueWhenTrue"), TestIsTrueWhenTrue);
    suite.add(QString("TestIsTrueWhenFalse"), TestIsTrueWhenFalse);
    suite.add(QString("TestIsFalseWhenFalse"), TestIsFalseWhenFalse);
    suite.add(QString("TestIsFalseWhenTrue"), TestIsFalseWhenTrue);
    suite.add(QString("TestAreEqualWhenTrueAndTrue"), TestAreEqualWhenTrueAndTrue);
    suite.add(QString("TestAreEqualWhenFalseAndFalse"), TestAreEqualWhenFalseAndFalse);
    suite.add(QString("TestAreEqualWhenTrueAndFalse"), TestAreEqualWhenTrueAndFalse);
    suite.add(QString("TestAreEqualWhenZeroAndZero"), TestAreEqualWhenZeroAndZero);
    suite.add(QString("TestAreEqualWhenOneAndOne"), TestAreEqualWhenOneAndOne);
    suite.add(QString("TestAreEqualWhenOneAndZero"), TestAreEqualWhenOneAndZero);
    suite.add(QString("TestAreEqualWhenEmptyStringAndEmptyString"), TestAreEqualWhenEmptyStringAndEmptyString);
    suite.add(QString("TestAreEqualWhenStringAndSameString"), TestAreEqualWhenStringAndSameString);
    suite.add(QString("TestAreEqualWhenStringAndDifferentString"), TestAreEqualWhenStringAndDifferentString);
    suite.add(QString("TestRunnerExecutesTests"), TestRunnerExecutesTests);

    return suite;
}

TestSuite AalTestTestsSuiteParameterized()
{
    using namespace Parameterized;

    TestSuite suite{};
    suite.add(QString("TestCanExecuteWithParameters"), TestCanExecuteWithParameters, TestCanExecuteWithParameters_Data);
    suite.add(QString("TestWithTwoEmptySubTestsHasTwoPasses"), TestWithTwoEmptySubTestsHasTwoPasses);
    suite.add(QString("TestWithSkipOnTrueInThreeSubTestsHasTwoPassesAndOneSkip"), TestWithSkipOnTrueInThreeSubTestsHasTwoPassesAndOneSkip);
    suite.add(QString("TestWithFailInTwoSubTestsHasTwoFails"), TestWithFailInTwoSubTestsHasTwoFails);
    suite.add(QString("TestWithIsTrueCheckInTwoSubTestsHasOnePassAndOneFails"), TestWithIsTrueCheckInTwoSubTestsHasOnePassAndOneFails);
    suite.add(QString("TestWithSkipOnTrueAndFailHasOneSkipAndTwoFails"), TestWithSkipOnTrueAndFailHasOneSkipAndTwoFails);
    suite.add(QString("TestWithSkipInTwoSubTestsHasTwoSkips"), TestWithSkipInTwoSubTestsHasTwoSkips);
    suite.add(QString("TestWithoutDataDontExecute"), TestWithoutDataDontExecute);

    return suite;
}

TestSuite AalTestSnapshotTestsSuite()
{
    TestSuite suite{};
    suite.add(QString("EqualsFileCreatesSnapshotWhenFileDoesNotExist"), EqualsFileCreatesSnapshotWhenFileDoesNotExist);
    suite.add(QString("EqualsFileDoesntCreateSnapshotWhenValuesAreEqual"), EqualsFileDoesntCreateSnapshotWhenValuesAreEqual);
    suite.add(QString("EqualsFileCreateSnapshotWhenValuesAreNotEqual"), EqualsFileCreateSnapshotWhenValuesAreNotEqual);

    return suite;
}

QList<TestSuite> AalTestTestsSuite()
{
    return QList<TestSuite>() << AalTestTestsSuiteSimple() << AalTestTestsSuiteParameterized() << AalTestSnapshotTestsSuite();
}
