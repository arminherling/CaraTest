#include <AalTest.h>
#include "../../AalTestTests/source/AalTestTests.h"
#include "../../DiffTests/source/DiffTests.h"

using namespace AalTest;

int main()
{
    TestRunner runner{};
    QList<TestSuite> testSuites{};
    testSuites << AalTestTestsSuite();
    testSuites << DiffTestsSuite();

    runner.run(testSuites);

    return 0;
}
