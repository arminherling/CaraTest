#include <CaraTest.h>
#include "CaraTestTests.h"

int main(int argc, char* argv[])
{
    CaraTest::TestRunner runner{argc, argv};
    auto testSuites = CaraTestTestsSuite();

    return runner.run(testSuites);
}
