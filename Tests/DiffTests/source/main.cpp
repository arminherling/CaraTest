#include <AalTest.h>
#include "DiffTests.h"

int main(int argc, char* argv[])
{
    AalTest::TestRunner runner{argc, argv};
    auto testSuites = DiffTestsSuite();

    runner.run(testSuites);

    return 0;
}
