#include <CaraTest.h>
#include "DiffTests.h"

int main(int argc, char* argv[])
{
    CaraTest::TestRunner runner{argc, argv};
    auto testSuites = DiffTestsSuite();

    return runner.run(testSuites);
}
