#pragma once

#include <AalTest/API.h>
#include <chrono>
#include <QString>
#include <tuple>

namespace AalTest
{
    AALTEST_API QString Stringify(bool value, bool quoteStrings = false);
    AALTEST_API QString Stringify(int value, bool quoteStrings = false);
    AALTEST_API QString Stringify(long long value, bool quoteStrings = false);
    AALTEST_API QString Stringify(const char* str, bool quoteStrings = false);
    AALTEST_API QString Stringify(const QString& string, bool quoteStrings = false);
    AALTEST_API QString Stringify(const QStringView stringView, bool quoteStrings = false);
    AALTEST_API QString Stringify(const std::chrono::nanoseconds& ns, bool quoteStrings = false);

    template<class T>
    QString Stringify(T t, bool quoteStrings = false)
    {
        return QString("?");
    }

    template<class ...Ts>
    QString Stringify(const std::tuple<Ts...>& tuple, bool quoteStrings = false)
    {
        auto converter = [quoteStrings](const auto& ...args) { return QStringList{ Stringify(args, quoteStrings)... }; };
        auto parts = std::apply(converter, tuple);
        return QString("(%1)").arg(parts.join(", "));
    }

    AALTEST_API QString Sanitize(QString input);
}
