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

        const auto suffixMismatchResult = std::mismatch(first.rbegin(), first.rend(), second.rbegin(), second.rend());
        const auto suffixEndIndex = std::distance(first.rbegin(), suffixMismatchResult.first) - 1;
        const auto endIndex = second.size() - secondIsStringOffset - suffixEndIndex;

        if (startIndex < endIndex)
            differences.append(DiffLocation{ .startIndex = (int)startIndex, .endIndex = (int)endIndex });

        return differences;
    }
}
