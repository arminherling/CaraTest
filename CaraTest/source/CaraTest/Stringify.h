#pragma once

#include <CaraTest/API.h>
#include <chrono>
#include <string>
#include <tuple>
#include <sstream>

namespace CaraTest
{
    template<typename T>
    struct is_string_like : std::false_type {};

    template<>
    struct is_string_like<char*> : std::true_type {};

    template<>
    struct is_string_like<const char*> : std::true_type {};

    template<>
    struct is_string_like<std::string> : std::true_type {};

    template<>
    struct is_string_like<std::string_view> : std::true_type {};

    template<typename T>
    constexpr bool is_string_like_v = is_string_like<std::decay_t<T>>::value;

    CARATEST_API std::string stringify(bool value);
    CARATEST_API std::string stringify(int value);
    CARATEST_API std::string stringify(long long value);
    CARATEST_API std::string stringify(const char* str);
    CARATEST_API std::string stringify(const std::string& string);
    CARATEST_API std::string stringify(std::string_view stringView);
    CARATEST_API std::string stringify(const std::chrono::nanoseconds& ns);

    template<class T>
    std::string stringify(const T& t)
    {
        return std::string("?");
    }

    template<class ...Ts>
    std::string stringify(const std::tuple<Ts...>& tuple)
    {
        auto converter = [](const auto& ...args) { return std::vector<std::string>{ stringify(args)... }; };
        auto parts = std::apply(converter, tuple);
        return std::string("(") + join(parts, ", ") + ")";
    }

    CARATEST_API std::string stringifyAndQuoted(const char* str);
    CARATEST_API std::string stringifyAndQuoted(const std::string& string);
    CARATEST_API std::string stringifyAndQuoted(std::string_view stringView);

    template<class T>
    std::string stringifyAndMaybeQuote(const T& t)
    {
        if constexpr (is_string_like_v<T>)
        {
            return stringifyAndQuoted(t);
        }
        else
        {
            return stringify(t);
        }
    }

    template<class ...Ts>
    std::string stringifyAndMaybeQuote(const std::tuple<Ts...>& tuple)
    {
        auto converter = [](const auto& ...args) { return std::vector<std::string>{ stringifyAndMaybeQuote(args)... }; };
        auto parts = std::apply(converter, tuple);

        std::stringstream joinedPartsStream;
        for (const auto& part : parts) {
            if (!joinedPartsStream.str().empty()) {
                joinedPartsStream << ", ";
            }
            joinedPartsStream << part;
        }
        return "(" + joinedPartsStream.str() + ")";
    }

    CARATEST_API std::string sanitize(const std::string& input);
}
