#pragma once

#include <AalTest/API.h>

namespace AalTest
{
    enum class FailureBehavior
    {
        Default,
        DebugBreak,
        DebugBreakAndRerun
    };

}
extern "C" AALTEST_API AalTest::FailureBehavior g_aalTestFailureBehavior;

#define TRIGGER_DEBUG_BREAK()                                                   \
    __debugbreak();

#define HANDLE_AALTEST_FAILURE()                                                \
    do {                                                                        \
        if(g_aalTestFailureBehavior == FailureBehavior::DebugBreak)             \
        {                                                                       \
           TRIGGER_DEBUG_BREAK();                                               \
        }                                                                       \
    } while(false)
