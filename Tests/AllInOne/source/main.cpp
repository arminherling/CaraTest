#include <CaraTest.h>
#include "../../CaraTestTests/source/CaraTestTests.h"
#include "../../DiffTests/source/DiffTests.h"

using namespace CaraTest;

int main(int argc, char* argv[])
{
    TestRunner runner{ argc, argv };
    QList<TestSuite> testSuites{};
    testSuites << CaraTestTestsSuite();
    testSuites << DiffTestsSuite();

    runner.run(testSuites);

    return 0;
}
