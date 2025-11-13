#include "File.h"
#include <fstream>
#include <sstream>

namespace CaraTest::File
{
    std::optional<std::string> readContent(const std::filesystem::path& filePath)
    {
        if (!std::filesystem::exists(filePath)) {
            return std::nullopt;
        }

        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::nullopt;
        }

        std::ostringstream contentStream;
        contentStream << file.rdbuf();
        return contentStream.str();
    }

    bool writeContent(const std::filesystem::path& filePath, const std::string& content)
    {
        std::ofstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        file << content;
        file.close();
        return true;
    }
}