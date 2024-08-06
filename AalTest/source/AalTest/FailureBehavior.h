#pragma once

namespace AalTest
{
    enum class FailureBehavior
    {
        Default,
        DebugBreak
    };

    inline FailureBehavior g_aalTestFailureBehavior = FailureBehavior::Default;
}

#define HANDLE_AALTEST_FAILURE()                                        \
    do {                                                                \
        if(g_aalTestFailureBehavior == FailureBehavior::DebugBreak)     \
        {                                                               \
            __debugbreak();                                             \
        }                                                               \
    } while(false)
