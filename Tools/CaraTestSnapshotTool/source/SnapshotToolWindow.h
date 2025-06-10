#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/QList>

class QMenuBar;
class QTreeView;
class QLabel;
class QTextEdit;
class QPushButton;
class QShowEvent;
class QFileSystemModel;
class DiffHighlighter;
class QFileSystemWatcher;

class SnapshotToolWindow : public QMainWindow
{
    Q_OBJECT

public:
    SnapshotToolWindow(QWidget *parent = nullptr);
    virtual ~SnapshotToolWindow() = default;

private:
    QMenuBar* m_menuBar;
    QTreeView* m_snapshotFileTree;
    QLabel* m_originFileName;
    QLabel* m_snapshotFileName;
    QTextEdit* m_originFileContent;
    QTextEdit* m_snapshotFileContent;
    QPushButton* m_rejectSnapshotButton;
    QPushButton* m_acceptSnapshotButton;
    QFileSystemModel* m_fileSystemModel;
    QFileSystemWatcher* m_selectedSnapshotWatcher;
    DiffHighlighter* m_originFileDiffHighlighter;
    DiffHighlighter* m_snapshotFileDiffHighlighter;

    void showEvent(QShowEvent* event) override;
    void onSnapshotClicked();
    void onUpdateButtonState();
    void onRejectClicked();
    void onAcceptClicked();
    void onCurrentDisplayedFileChanged(const QString& filePath);
    void selectDirectoryToWatch();
    void setDirectoryToWatch(const QString& dirPath);
    void clearPreviews();
    void clearSelectedSnapshotWatcher();
    void updateHighlighters();
};
