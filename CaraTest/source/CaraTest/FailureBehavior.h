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

#if defined(_MSC_VER)
    #define TRIGGER_DEBUG_BREAK() __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
    #define TRIGGER_DEBUG_BREAK() __builtin_trap();
#else
    #define TRIGGER_DEBUG_BREAK() static_assert(false, "Debug break not supported for this compiler");
#endif

#define HANDLE_CARATEST_FAILURE()                                                   \
    do {                                                                            \
        if(CaraTest::GetFailureBehavior() == CaraTest::FailureBehavior::DebugBreak) \
        {                                                                           \
           TRIGGER_DEBUG_BREAK();                                                   \
        }                                                                           \
    } while(false)
