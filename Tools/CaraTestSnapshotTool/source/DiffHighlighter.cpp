#include "DiffHighlighter.h"

DiffHighlighter::DiffHighlighter(CaraTest::DiffChange diffChange, const QColor& color, QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , m_diffChange{ diffChange }
{
    m_format.setForeground(color);
    m_format.setFontUnderline(true);
}

void DiffHighlighter::highlightDiffs(const std::vector<CaraTest::DiffLocation>& differences)
{
    m_differences = differences;
    rehighlight();
}

void DiffHighlighter::clear()
{
    m_differences.clear();
    rehighlight();
}

void DiffHighlighter::highlightBlock(const QString& text)
{
    const auto firstPosition = currentBlock().position();
    for (const auto& diff : m_differences)
    {
        if (diff.change == m_diffChange)
        {
            int start = diff.startIndex - firstPosition;
            int end = diff.endIndex - firstPosition + 1;
            setFormat(start, end - start, m_format);
        }
    }
}
