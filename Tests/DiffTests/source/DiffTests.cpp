#include "DiffTests.h"
#include <CaraTest.h>
#include <CaraTest/Diff.h>
#include <tuple>

using namespace CaraTest;

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
        std::make_tuple(QString("x"), QString("y"), QList<DiffLocation>{
            DiffLocation{.startIndex = 0, .endIndex = 0, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 0, .endIndex = 0, .change = DiffChange::Addition}
        }),
        std::make_tuple(QString("abc123"), QString("abc789"), QList<DiffLocation>{
            DiffLocation{.startIndex = 3, .endIndex = 3, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 4, .endIndex = 4, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 5, .endIndex = 5, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 3, .endIndex = 3, .change = DiffChange::Addition},
            DiffLocation{.startIndex = 4, .endIndex = 4, .change = DiffChange::Addition},
            DiffLocation{.startIndex = 5, .endIndex = 5, .change = DiffChange::Addition}
    }),
        std::make_tuple(
            QString("enum Values \n{\n    First\n    Second\n    Third\n}\n"),
            QString("enum Values \n{\n    First\n    Second = 3\n    Third\n}\n"),
            QList<DiffLocation>{
                DiffLocation{.startIndex = 35, .endIndex = 35, .change = DiffChange::Addition},
                DiffLocation{.startIndex = 36, .endIndex = 36, .change = DiffChange::Addition},
                DiffLocation{.startIndex = 37, .endIndex = 37, .change = DiffChange::Addition},
                DiffLocation{.startIndex = 38, .endIndex = 38, .change = DiffChange::Addition}
            }),
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
)"),
    QList<DiffLocation>{
        DiffLocation{.startIndex = 35, .endIndex = 35, .change = DiffChange::Addition},
        DiffLocation{.startIndex = 36, .endIndex = 36, .change = DiffChange::Addition},
        DiffLocation{.startIndex = 37, .endIndex = 37, .change = DiffChange::Addition},
        DiffLocation{.startIndex = 38, .endIndex = 38, .change = DiffChange::Addition}
    }),
    };
}

TestSuite DiffTestsSuite()
{
    TestSuite suite{};
    suite.add(QString("Diff"), DiffTest, DiffTest_Data);

    return suite;
}
