#include "Stringify.h"
#include <sstream>
#include <iomanip>

namespace CaraTest
{
    std::string stringify(bool value)
    {
        return value ? "true" : "false";
    }

    std::string stringify(int value)
    {
        return std::to_string(value);
    }

    std::string stringify(long long value)
    {
        return std::to_string(value);
    }

    std::string stringify(const char* str)
    {
        return std::string(str);
    }

    std::string stringify(const std::string& string)
    {
        return string;
    }

    std::string stringify(const std::string_view& stringView)
    {
        return std::string(stringView);
    }

    std::string stringify(const std::chrono::nanoseconds& input)
    {
        auto ns = input;

        using namespace std::chrono;

        auto s = duration_cast<seconds>(ns);
        ns -= s;
        auto ms = duration_cast<milliseconds>(ns);
        ns -= ms;
        auto us = duration_cast<microseconds>(ns);
        ns -= us;

        std::ostringstream stream;
        stream << s.count() << "s ";
        stream << std::setw(3) << std::setfill('0') << ms.count() << "ms ";
        stream << std::setw(3) << std::setfill('0') << us.count() << "μs ";
        stream << std::setw(3) << std::setfill('0') << ns.count() << "ns";

        return stream.str();
    }

    std::string stringifyAndQuoted(const char* str)
    {
        return "\"" + std::string(str) + "\"";
    }

    std::string stringifyAndQuoted(const std::string& string)
    {
        return "\"" + string + "\"";
    }

    std::string sanitize(const std::string& input)
    {
        const std::vector<std::pair<char, std::string>> replacements = {
            {'\t', "\\t"},
            {'\r', "\\r"},
            {'\n', "\\n"}
        };

        std::string result = input;
        for (const auto& [target, replacement] : replacements)
        {
            size_t pos = 0;
            while ((pos = result.find(target, pos)) != std::string::npos)
            {
                result.replace(pos, 1, replacement);
                // move past the replacement
                pos += replacement.length();
            }
        }

        return result;
    }
}
