#pragma once 

#include <CaraTest/API.h>

#include <QtCore/QString>
#include <QtCore/QFileInfo>

namespace CaraTest
{
    namespace File
    {
        CARATEST_API QString ReadContent(const QString& filePath);
        CARATEST_API QString ReadContent(const QFileInfo& fileInfo);
        CARATEST_API bool WriteContent(const QString& filePath, const QString& content);
    }
}
