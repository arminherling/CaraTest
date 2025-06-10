#include "File.h"

#include <QtCore/QFile>
#include <QtCore/QDir>

namespace CaraTest
{
    namespace File
    {
        QString ReadContent(const QString& filePath)
        {
            return ReadContent(QFileInfo(filePath));
        }

        QString ReadContent(const QFileInfo& fileInfo)
        {
            const auto filePath = fileInfo.absoluteFilePath();
            auto file = QFile(filePath);
            file.open(QIODevice::ReadOnly);
            return QString(file.readAll());
        }

        bool WriteContent(const QString& filePath, const QString& content)
        {
            const auto cleanedFilePath = QDir::cleanPath(filePath);
            auto file = QFile(cleanedFilePath);
            const auto isOpen = file.open(QIODevice::WriteOnly);
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
}