#include "DiffTests.h"
#include <AalTest.h>
#include <AalTest/Diff.h>
#include <tuple>

using namespace AalTest;

static void DiffTest(const QString& first, const QString& second, const QList<DiffLocation>& expectedDifferences)
{
    auto differences = Diff(first, second);

    AreEqual(expectedDifferences.length(), differences.length());
}

static QList<std::tuple<QString, QString, QList<DiffLocation>>> DiffTest_Data()
{
    return {
        std::make_tuple(QString("x"), QString("x"), QList<DiffLocation>()),
    };
}

TestSuite DiffTestsSuite()
{
    TestSuite suite{};
    suite.add(QString("Diff"), DiffTest, DiffTest_Data);

    return suite;
}
