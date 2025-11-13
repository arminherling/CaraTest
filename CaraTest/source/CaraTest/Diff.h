#pragma once

#include <CaraTest/API.h>
#include <string>
#include <vector>

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

    CARATEST_API std::vector<DiffLocation> diff(const std::string& first, const std::string& second);
}
