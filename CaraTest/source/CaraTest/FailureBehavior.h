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

}
extern "C" CARATEST_API CaraTest::FailureBehavior g_caraTestFailureBehavior;

#define TRIGGER_DEBUG_BREAK()                                                   \
    __debugbreak();

#define HANDLE_CARATEST_FAILURE()                                                \
    do {                                                                        \
        if(g_caraTestFailureBehavior == FailureBehavior::DebugBreak)             \
        {                                                                       \
           TRIGGER_DEBUG_BREAK();                                               \
        }                                                                       \
    } while(false)
