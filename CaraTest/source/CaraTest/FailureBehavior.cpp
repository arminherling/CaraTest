#include <CaraTest/FailureBehavior.h>

namespace CaraTest
{
    FailureBehavior g_caraTestFailureBehavior = FailureBehavior::Default;

    FailureBehavior GetFailureBehavior()
    {
        return g_caraTestFailureBehavior;
    }

    void SetFailureBehavior(FailureBehavior value)
    {
        g_caraTestFailureBehavior = value;
    }
}
