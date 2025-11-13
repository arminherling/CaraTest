#pragma once

#include <CaraTest/Diff.h>

#include <QSyntaxHighlighter>
#include <vector>

class DiffHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    DiffHighlighter(CaraTest::DiffChange diffChange, const QColor& color, QTextDocument* parent = nullptr);

    void highlightDiffs(const std::vector<CaraTest::DiffLocation>& differences);
    void clear();

protected:
    void highlightBlock(const QString& text) override;

private:
    std::vector<CaraTest::DiffLocation> m_differences;
    CaraTest::DiffChange m_diffChange;
    QTextCharFormat m_format;
};
