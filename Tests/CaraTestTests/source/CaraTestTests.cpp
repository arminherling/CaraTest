#include "CaraTestTests.h"
#include <CaraTest.h>

#include <QDir>
#include <QRandomGenerator>

using namespace CaraTest;

namespace Simple
{
    static void TestCanExecute()
    {
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};
        int i = 0;
        suite.add(QString(), [&i]() {i++; });

        const auto result = runner.run(suite);

        CaraTest::AreEqual(i, 1);
    }

    static void TestCanPass()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                // do nothing
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestCanFail()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                CaraTest::Fail();
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestCanBeSkipped()
    {
        int expectedPasses = 0;
        int expectedFails = 0;
        int expectedSkips = 1;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                CaraTest::Skip();
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestSuitesReturnsAddedTests()
    {
        auto expectedTestCount = 2;
        TestSuite suite{};
        suite.add(QString(), []()
            {
                CaraTest::Skip();
            });
        suite.add(QString(), []()
            {
                CaraTest::Fail();
            });

        CaraTest::AreEqual(expectedTestCount, (int)suite.tests().size());
    }

    static void TestContainsCallingTestName()
    {
        QString expectedName = QString("TestContainsCallingTestName");
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};
        suite.add(QString("TestContainsCallingTestName"), []()
            {
                CaraTest::Skip();
            });

        auto& test = suite.tests().at(0);

        CaraTest::AreEqual(expectedName, test->testName());
    }

    static void TestIsTrueWhenTrue()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = true;
                CaraTest::IsTrue(value);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestIsTrueWhenFalse()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = false;
                CaraTest::IsTrue(value);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestIsFalseWhenFalse()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = false;
                CaraTest::IsFalse(value);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestIsFalseWhenTrue()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto value = true;
                CaraTest::IsFalse(value);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenTrueAndTrue()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = true;
                auto actualValue = true;
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenFalseAndFalse()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = false;
                auto actualValue = false;
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenTrueAndFalse()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = true;
                auto actualValue = false;
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenZeroAndZero()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = 0;
                auto actualValue = 0;
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenOneAndOne()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = 1;
                auto actualValue = 1;
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenOneAndZero()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = 1;
                auto actualValue = 0;
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenEmptyStringAndEmptyString()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = QString();
                auto actualValue = QString();
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenStringAndSameString()
    {
        int expectedPasses = 1;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = QString("string");
                auto actualValue = QString("string");
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestAreEqualWhenStringAndDifferentString()
    {
        int expectedPasses = 0;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), []()
            {
                auto expectedValue = QString("string");
                auto actualValue = QString("other");
                CaraTest::AreEqual(expectedValue, actualValue);
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestRunnerExecutesTests()
    {
        int expectedPasses = 1;
        int expectedFails = 1;
        int expectedSkips = 1;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};
        suite.add(QString(), []()
            {
            });
        suite.add(QString(), []()
            {
                CaraTest::Skip();
            });
        suite.add(QString(), []()
            {
                CaraTest::Fail();
            });

        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }
}

namespace Parameterized
{
    static void TestCanExecuteWithParameters(bool data)
    {
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};
        int i = 0;
        suite.add(QString(), [&i]() {i++; });

        const auto result = runner.run(suite);

        CaraTest::AreEqual(i, 1);
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
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                // do nothing
            }, 
            []() 
            { 
                return QList{ std::make_tuple(true), std::make_tuple(false) }; 
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithSkipOnTrueInThreeSubTestsHasTwoPassesAndOneSkip()
    {
        int expectedPasses = 2;
        int expectedFails = 0;
        int expectedSkips = 1;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                if (value)
                    CaraTest::Skip();
            },
            []()
            {
                return QList{ std::make_tuple(true), std::make_tuple(false), std::make_tuple(false) };
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithFailInTwoSubTestsHasTwoFails()
    {
        int expectedPasses = 0;
        int expectedFails = 2;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                CaraTest::Fail();
            },
            []()
            {
                return QList{ std::make_tuple(false), std::make_tuple(false) };
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithIsTrueCheckInTwoSubTestsHasOnePassAndOneFails()
    {
        int expectedPasses = 1;
        int expectedFails = 1;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                CaraTest::IsTrue(value);
            },
            []()
            {
                return QList{ std::make_tuple(true), std::make_tuple(false) };
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithSkipOnTrueAndFailHasOneSkipAndTwoFails()
    {
        int expectedPasses = 0;
        int expectedFails = 2;
        int expectedSkips = 1;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                if (value)
                    CaraTest::Skip();

                CaraTest::Fail();
            },
            []()
            {
                return QList{ std::make_tuple(false), std::make_tuple(true), std::make_tuple(false) };
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithSkipInTwoSubTestsHasTwoSkips()
    {
        int expectedPasses = 0;
        int expectedFails = 0;
        int expectedSkips = 2;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                CaraTest::Skip();
            },
            []()
            {
                return QList{ std::make_tuple(true), std::make_tuple(false) };
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
    }

    static void TestWithoutDataDontExecute()
    {
        int expectedPasses = 0;
        int expectedFails = 0;
        int expectedSkips = 0;
        TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
        TestSuite suite{};

        suite.add(QString(), [](bool value)
            {
                // do nothing
            },
            []()
            {
                return QList<std::tuple<bool>>{};
            });
        const auto result = runner.run(suite);

        CaraTest::AreEqual(expectedPasses, suite.passedTests());
        CaraTest::AreEqual(expectedFails, suite.failedTests());
        CaraTest::AreEqual(expectedSkips, suite.skippedTests());
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
    TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
    TestSuite suite{};

    const auto testFilePath = GenerateRandomFilePath();

    suite.add(QString(), [=]()
        {
            CaraTest::EqualsFile(QFileInfo(testFilePath), QString("TestValue"));
        });
    const auto result = runner.run(suite);

    const auto snapshotFilePath = QDir::cleanPath(testFilePath + QString(".snapshot"));
    auto snapshotFile = QFile(snapshotFilePath);
    
    const auto fileExists = snapshotFile.exists();
    const auto wasRemoved = snapshotFile.remove();

    CaraTest::IsTrue(fileExists);
    CaraTest::IsTrue(wasRemoved);
    CaraTest::AreEqual(expectedPasses, suite.passedTests());
    CaraTest::AreEqual(expectedFails, suite.failedTests());
    CaraTest::AreEqual(expectedSkips, suite.skippedTests());
}

static void EqualsFileDoesntCreateSnapshotWhenValuesAreEqual()
{
    int expectedPasses = 1;
    int expectedFails = 0;
    int expectedSkips = 0;
    QString expectedValue("TestValue");

    TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
    TestSuite suite{};

    const auto testFilePath = GenerateRandomFilePath();
    CaraTest::File::WriteContent(testFilePath, expectedValue);

    suite.add(QString(), [=]()
        {
            CaraTest::EqualsFile(QFileInfo(testFilePath), expectedValue);
        });
    const auto result = runner.run(suite);

    const auto snapshotFilePath = QDir::cleanPath(testFilePath + QString(".snapshot"));
    auto snapshotFile = QFile(snapshotFilePath);

    const auto fileExists = snapshotFile.exists();
    const auto wasRemoved = snapshotFile.remove();

    CaraTest::IsFalse(fileExists);
    CaraTest::IsFalse(wasRemoved);
    CaraTest::AreEqual(expectedPasses, suite.passedTests());
    CaraTest::AreEqual(expectedFails, suite.failedTests());
    CaraTest::AreEqual(expectedSkips, suite.skippedTests());
}

static void EqualsFileCreateSnapshotWhenValuesAreNotEqual()
{
    int expectedPasses = 0;
    int expectedFails = 1;
    int expectedSkips = 0;
    QString expectedValue("TestValue");

    TestRunner runner{ 0, nullptr, TestRunner::OutputMode::None };
    TestSuite suite{};

    const auto testFilePath = GenerateRandomFilePath();
    CaraTest::File::WriteContent(testFilePath, expectedValue);

    suite.add(QString(), [=]()
        {
            CaraTest::EqualsFile(QFileInfo(testFilePath), QString("Some other Value"));
        });
    const auto result = runner.run(suite);

    const auto snapshotFilePath = QDir::cleanPath(testFilePath + QString(".snapshot"));
    auto snapshotFile = QFile(snapshotFilePath);

    const auto fileExists = snapshotFile.exists();
    const auto wasRemoved = snapshotFile.remove();

    CaraTest::IsTrue(fileExists);
    CaraTest::IsTrue(wasRemoved);
    CaraTest::AreEqual(expectedPasses, suite.passedTests());
    CaraTest::AreEqual(expectedFails, suite.failedTests());
    CaraTest::AreEqual(expectedSkips, suite.skippedTests());
}

TestSuite CaraTestTestsSuiteSimple()
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

TestSuite CaraTestTestsSuiteParameterized()
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

TestSuite CaraTestSnapshotTestsSuite()
{
    TestSuite suite{};
    suite.add(QString("EqualsFileCreatesSnapshotWhenFileDoesNotExist"), EqualsFileCreatesSnapshotWhenFileDoesNotExist);
    suite.add(QString("EqualsFileDoesntCreateSnapshotWhenValuesAreEqual"), EqualsFileDoesntCreateSnapshotWhenValuesAreEqual);
    suite.add(QString("EqualsFileCreateSnapshotWhenValuesAreNotEqual"), EqualsFileCreateSnapshotWhenValuesAreNotEqual);

    return suite;
}

QList<TestSuite> CaraTestTestsSuite()
{
    return QList<TestSuite>() << CaraTestTestsSuiteSimple() << CaraTestTestsSuiteParameterized() << CaraTestSnapshotTestsSuite();
}
