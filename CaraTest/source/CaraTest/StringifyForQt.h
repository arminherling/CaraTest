#ifdef QT_CORE_LIB

#pragma once

#include <QString>
#include <string>

namespace CaraTest
{
    template<>
    struct is_string_like<QString> : std::true_type {};

    template<>
    struct is_string_like<QStringView> : std::true_type {};

    inline std::string stringify(const QString& string)
    {
        return string.toStdString();
    }

    inline std::string stringify(const QStringView stringView)
    {
        return stringView.toString().toStdString();
    }

    inline std::string stringifyAndQuoted(const QString& string)
    {
        return "\"" + string.toStdString() + "\"";
    }

    inline std::string stringifyAndQuoted(const QStringView stringView)
    {
        return stringifyAndQuoted(stringView.toString());
    }
}
#endif
