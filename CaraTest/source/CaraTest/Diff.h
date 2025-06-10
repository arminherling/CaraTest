#pragma once

#include <CaraTest/API.h>
#include <CaraTest/DiffLocation.h>
#include <QString>
#include <QList>

namespace CaraTest 
{
    CARATEST_API QList<DiffLocation> Diff(const QString& first, const QString& second);
}
