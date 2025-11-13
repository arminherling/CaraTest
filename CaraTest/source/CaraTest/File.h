#pragma once 

#include <CaraTest/API.h>

#include <filesystem>
#include <string>
#include <optional>

namespace CaraTest
{
    namespace File
    {
        CARATEST_API std::optional<std::string> readContent(const std::filesystem::path& filePath);
        CARATEST_API bool writeContent(const std::filesystem::path& filePath, const std::string& content);
    }
}
