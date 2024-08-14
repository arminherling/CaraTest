#include "DiffTests.h"
#include <AalTest.h>
#include <AalTest/Diff.h>
#include <tuple>

using namespace AalTest;

static void DiffTest(const QString& first, const QString& second, const QList<DiffLocation>& expectedDifferences)
{
    const auto differences = Diff(first, second);

    AreEqual(expectedDifferences.length(), differences.length());
    for (auto i = 0; i < differences.length(); i++)
    {
        AreEqual(expectedDifferences.at(i).startIndex, differences.at(i).startIndex);
        AreEqual(expectedDifferences.at(i).endIndex, differences.at(i).endIndex);
    }
}

static QList<std::tuple<QString, QString, QList<DiffLocation>>> DiffTest_Data()
{
    return {
        std::make_tuple(QString(), QString(), QList<DiffLocation>()),
        std::make_tuple(QString(""), QString(""), QList<DiffLocation>()),
        std::make_tuple(QString("x"), QString("x"), QList<DiffLocation>()),
        std::make_tuple(QString("x"), QString("y"), QList<DiffLocation>{ DiffLocation{.startIndex = 0, .endIndex = 1} }),
        std::make_tuple(QString("abc123"), QString("abc789"), QList<DiffLocation>{ DiffLocation{.startIndex = 3, .endIndex = 6} }),
        std::make_tuple(
            QString("enum Values \n{\n    First\n    Second\n    Third\n}\n"),
            QString("enum Values \n{\n    First\n    Second = 3\n    Third\n}\n"),
            QList<DiffLocation>{ DiffLocation{.startIndex = 35, .endIndex = 52} }),
        std::make_tuple(
QString(R"(enum Values 
{
    First
    Second
    Third
}
)"),
QString(R"(enum Values 
{
    First
    Second = 3
    Third
}
)"), QList<DiffLocation>{ DiffLocation{.startIndex = 35, .endIndex = 52} }),
    };
}

TestSuite DiffTestsSuite()
{
    TestSuite suite{};
    suite.add(QString("Diff"), DiffTest, DiffTest_Data);

    return suite;
}
