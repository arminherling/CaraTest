#include "FileHelper.h"
#include <QFile>

QString ReadStyleContent(const QString& filePath)
{
    auto file = QFile(filePath);
    file.open(QIODevice::ReadOnly);
    return QString(file.readAll());
}
