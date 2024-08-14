#include "Diff.h"

#include <algorithm>

namespace AalTest
{
    QList<DiffLocation> Diff(const QString& first, const QString& second)
    {
        auto differences = QList<DiffLocation>();

        if (first.isNull() || second.isNull())
            return differences;

        if (first == second && first.isEmpty())
            return differences;

        const auto prefixMismatchResult = std::mismatch(first.begin(), first.end(), second.begin(), second.end());
        const auto startIndex = std::distance(first.begin(), prefixMismatchResult.first);
        const auto firstIsStringOffset = (first.at(first.size() - 1) == '"') ? 1 : 0;
        const auto secondIsStringOffset = (second.at(second.size() - 1) == '"') ? 1 : 0;
        const auto endIndex = std::max(first.size() - firstIsStringOffset, second.size() - secondIsStringOffset);

        if (startIndex < endIndex)
            differences.append(DiffLocation{ .startIndex = (int)startIndex, .endIndex = (int)endIndex });

        return differences;
    }
}
