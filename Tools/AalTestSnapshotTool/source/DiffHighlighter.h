#pragma once

#include <AalTest/Diff.h>

#include <QSyntaxHighlighter>
#include <QList>

class DiffHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    DiffHighlighter(AalTest::DiffChange diffChange, const QColor& color, QTextDocument* parent = nullptr);

    void highlightDiffs( const QList<AalTest::DiffLocation>& differences);
    void clear();

protected:
    void highlightBlock(const QString& text) override;

private:
    QList<AalTest::DiffLocation> m_differences;
    AalTest::DiffChange m_diffChange;
    QTextCharFormat m_format;
};
