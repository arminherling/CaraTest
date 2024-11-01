#include "Diff.h"

#include <vector>
#include <algorithm>
#include <QSet>

namespace AalTest
{
    QList<DiffLocation> Diff(const QString& first, const QString& second)
    {
        QList<DiffLocation> differences;

        if (first.isNull() || second.isNull())
            return differences;

        if (first == second && first.isEmpty())
            return differences;

        const int firstLength = first.size();
        const int secondLength = second.size();

        std::vector<std::vector<int>> lengthTable(firstLength + 1, std::vector<int>(secondLength + 1, 0));

        for (int i = 1; i <= firstLength; ++i)
        {
            for (int j = 1; j <= secondLength; ++j)
            {
                if (first[i - 1] == second[j - 1])
                {
                    lengthTable[i][j] = lengthTable[i - 1][j - 1] + 1;
                }
                else
                {
                    lengthTable[i][j] = std::max(lengthTable[i - 1][j], lengthTable[i][j - 1]);
                }
            }
        }

        // we need to backtrack to find the differences
        for (int i = firstLength, j = secondLength; i > 0 || j > 0;)
        {
            if (i > 0 && j > 0 && first[i - 1] == second[j - 1])
            {
                --i; --j;
            }
            else if (j > 0 && (i == 0 || lengthTable[i][j - 1] >= lengthTable[i - 1][j]))
            {
                differences.append(DiffLocation{ .startIndex = j - 1, .endIndex = j - 1, .change = DiffChange::Addition });
                --j;
            }
            else
            {
                differences.append(DiffLocation{ .startIndex = i - 1, .endIndex = i - 1, .change = DiffChange::Deletion });
                --i;
            }
        }

        // reverse the order so that the indices are in ascending order
        std::reverse(differences.begin(), differences.end());

        return differences;
    }
}
