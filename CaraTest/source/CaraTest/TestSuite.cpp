#include "TestSuite.h"

namespace CaraTest
{
    TestSuite::TestSuite(const std::string& name)
        : m_name{ name }
        , m_tests{}
    {
    }

    TestSuite::TestSuite(const std::source_location& location)
        : m_name{ toSuiteName(location) }
        , m_tests{}
    {
    }

    std::string toSuiteName(const std::source_location& location)
    {
        if (!std::string(location.function_name()).empty())
        {
            return location.function_name();
        }
        else
        {
            std::filesystem::path fullPath = location.file_name();
            const auto filename = fullPath.stem();
            return filename.string();
        }
    }
}
