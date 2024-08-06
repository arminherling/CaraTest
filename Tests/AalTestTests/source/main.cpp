#include <AalTest.h>
#include "AalTestTests.h"

int main(int argc, char* argv[])
{
    AalTest::TestRunner runner{argc, argv};
    auto testSuites = AalTestTestsSuite();

    runner.run(testSuites);

    return 0;
}
