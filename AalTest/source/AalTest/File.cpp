#include "File.h"

#include <QtCore/QFile>
#include <QtCore/QDir>

namespace AalTest
{
    QString ReadFileContent(const QString& filePath)
    {
        return ReadFileContent(QFileInfo(filePath));
    }

    QString ReadFileContent(const QFileInfo& fileInfo)
    {
        const auto filePath = fileInfo.absoluteFilePath();
        auto file = QFile(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        return QString::fromUtf8(file.readAll());
    }

    bool WriteFileContent(const QString& filePath, const QString& content)
    {
        const auto cleanedFilePath = QDir::cleanPath(filePath);
        auto file = QFile(cleanedFilePath);
        const auto isOpen = file.open(QIODevice::WriteOnly | QIODevice::Text);
        if (!isOpen)
        {
            return false;
        }

        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);
        stream.setGenerateByteOrderMark(true);
        stream << content;
        file.close();

        return true;
    }
}