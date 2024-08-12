#pragma once

#include <AalTest/API.h>
#include <AalTest/DiffLocation.h>
#include <QString>
#include <QList>

namespace AalTest 
{
    AALTEST_API QList<DiffLocation> Diff(const QString& first, const QString& second);
}
