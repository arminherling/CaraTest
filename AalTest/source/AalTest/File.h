#pragma once 

#include <AalTest/API.h>

#include <QtCore/QString>
#include <QtCore/QFileInfo>

namespace AalTest
{
    AALTEST_API QString ReadFileContent(const QString& path);
    AALTEST_API QString ReadFileContent(const QFileInfo& fileInfo);

    AALTEST_API bool WriteFileContent(const QString& filePath, const QString& content);
}
