#pragma once

#include <CaraTest/API.h>
#include <chrono>
#include <QString>
#include <tuple>

namespace CaraTest
{
    template<typename T>
    struct is_string_like : std::false_type {};

    template<>
    struct is_string_like<char*> : std::true_type {};

    template<>
    struct is_string_like<const char*> : std::true_type {};

    template<>
    struct is_string_like<QString> : std::true_type {};

    template<>
    struct is_string_like<QStringView> : std::true_type {};

    template<>
    struct is_string_like<std::string> : std::true_type {};

    template<typename T>
    constexpr bool is_string_like_v = is_string_like<std::decay_t<T>>::value;

    CARATEST_API QString Stringify(bool value);
    CARATEST_API QString Stringify(int value);
    CARATEST_API QString Stringify(long long value);
    CARATEST_API QString Stringify(const char* str);
    CARATEST_API QString Stringify(const QString& string);
    CARATEST_API QString Stringify(const QStringView stringView);
    CARATEST_API QString Stringify(const std::string& string);
    CARATEST_API QString Stringify(const std::chrono::nanoseconds& ns);

    template<class T>
    QString Stringify(const T& t)
    {
        return QString("?");
    }

    template<class ...Ts>
    QString Stringify(const std::tuple<Ts...>& tuple)
    {
        auto converter = [](const auto& ...args) { return QStringList{ Stringify(args)... }; };
        auto parts = std::apply(converter, tuple);
        return QString("(%1)").arg(parts.join(", "));
    }

    CARATEST_API QString StringifyAndQuoted(const char* str);
    CARATEST_API QString StringifyAndQuoted(const QString& string);
    CARATEST_API QString StringifyAndQuoted(const QStringView stringView);
    CARATEST_API QString StringifyAndQuoted(const std::string& string);

    template<class T>
    QString StringifyAndMaybeQuote(const T& t)
    {
        if constexpr (is_string_like_v<T>)
        {
            return StringifyAndQuoted(t);
        }
        else
        {
            return Stringify(t);
        }
    }

    template<class ...Ts>
    QString StringifyAndMaybeQuote(const std::tuple<Ts...>& tuple)
    {
        auto converter = [](const auto& ...args) { return QStringList{ StringifyAndMaybeQuote(args)... }; };
        auto parts = std::apply(converter, tuple);
        return QString("(%1)").arg(parts.join(", "));
    }

    CARATEST_API QString Sanitize(QString input);
}
