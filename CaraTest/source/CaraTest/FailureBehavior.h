#pragma once

#include <CaraTest/API.h>

namespace CaraTest
{
    enum class FailureBehavior
    {
        Default,
        DebugBreak,
        DebugBreakAndRerun
    };

    CARATEST_API FailureBehavior GetFailureBehavior();
    CARATEST_API void SetFailureBehavior(FailureBehavior value);
}

#define TRIGGER_DEBUG_BREAK()                                                       \
    __debugbreak();

#define HANDLE_CARATEST_FAILURE()                                                   \
    do {                                                                            \
        if(CaraTest::GetFailureBehavior() == CaraTest::FailureBehavior::DebugBreak) \
        {                                                                           \
           TRIGGER_DEBUG_BREAK();                                                   \
        }                                                                           \
    } while(false)
