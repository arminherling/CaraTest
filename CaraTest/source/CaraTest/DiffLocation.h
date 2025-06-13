#pragma once

#include <CaraTest/API.h>

namespace CaraTest
{
    enum class DiffChange
    {
        Unknown,
        Addition,
        Deletion
    };

    struct CARATEST_API DiffLocation
    {
        int startIndex = 0;
        int endIndex = 0;
        DiffChange change = DiffChange::Unknown;
    };
}
