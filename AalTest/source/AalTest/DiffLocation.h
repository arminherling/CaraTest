#pragma once

#include <AalTest/API.h>

namespace AalTest
{
    enum class AALTEST_API DiffChange
    {
        Unknown,
        Addition,
        Deletion
    };

    struct AALTEST_API DiffLocation
    {
        int startIndex = 0;
        int endIndex = 0;
        DiffChange change = DiffChange::Unknown;
    };
}
