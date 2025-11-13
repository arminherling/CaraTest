#include "Tests.h"

namespace CaraTest 
{
    FailureBehavior& getFailureBehavior()
    {
        static FailureBehavior failureBehavior = FailureBehavior::Default;
        return failureBehavior;
    }

    void setFailureBehavior(FailureBehavior value)
    {
        getFailureBehavior() = value;
    }
}
