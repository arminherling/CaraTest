#include "SnapshotToolWindow.h"
#include "DiffHighlighter.h"

#include <CaraTest/Diff.h>
#include <CaraTest/File.h>

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QStandardPaths>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>

#include <QtGui/QFileSystemModel>
#include <QtGui/QShowEvent>

namespace
{
    const auto ToolName = QString("CaraTest Snapshot Tool");
    const auto EmptyFileTitle = QString("...");
    const auto SnapshotFileExtensionMask = QString("*.snapshot");
    const auto ConsolasFont = QFont{ "Consolas", 12, QFont::Normal };
    const auto AcceptColor = QColor("#8AE28A");
    const auto RejectColor = QColor("#F44B56");

    static void ShowWarningDialog(const QString& message)
    {
        QMessageBox messageBox;
        messageBox.setText(message);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }

    static QFileInfo GetOriginFileInfoForSnapshot(const QFileInfo& fileInfo)
    {
        const auto directory = fileInfo.dir();
        const auto completeBaseName = fileInfo.completeBaseName();
        const auto originPath = directory.filePath(completeBaseName);
        return QFileInfo(originPath);
    }

    static QString GetSettingsFilePath()
    {
        const auto appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(appDataDir); // Ensure the directory exists

        const auto filePath = QDir(appDataDir).filePath("snapshot_tool_settings.json");
        return filePath;
    }

    static QString LoadLastDirectory()
    {
        const auto fileContent = CaraTest::File::ReadContent(GetSettingsFilePath());
        if (fileContent.isEmpty())
            return {};

        const auto jsonDocument = QJsonDocument::fromJson(fileContent.toUtf8());
        if (!jsonDocument.isObject())
            return {};

        const auto jsonObject = jsonDocument.object();
        return jsonObject.value("lastDirectory").toString();
    }

    static void SaveLastDirectory(const QString& dirPath)
    {
        QJsonObject jsonObject{};
        jsonObject["lastDirectory"] = dirPath;
        QJsonDocument jsonDocument(jsonObject);

        const auto settingsFilePath = GetSettingsFilePath();
        const auto jsonContent = jsonDocument.toJson(QJsonDocument::Indented);
        CaraTest::File::WriteContent(settingsFilePath, jsonContent);
    }
}

SnapshotToolWindow::SnapshotToolWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_selectedSnapshotWatcher(new QFileSystemWatcher(this))
{
    setWindowTitle(ToolName);

    m_menuBar = new QMenuBar(this);
    setMenuBar(m_menuBar);
    m_menuBar->addAction(QString("Select Directory"), this, &SnapshotToolWindow::selectDirectoryToWatch);

    m_snapshotFileTree = new QTreeView(this);
    m_fileSystemModel = new QFileSystemModel(this);
    m_fileSystemModel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    m_fileSystemModel->setNameFilters(QStringList() << SnapshotFileExtensionMask);
    m_fileSystemModel->setNameFilterDisables(false);
    m_snapshotFileTree->setModel(m_fileSystemModel);

    m_snapshotFileTree->setMinimumWidth(220);
    m_snapshotFileTree->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_snapshotFileTree->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    m_snapshotFileTree->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    m_snapshotFileTree->setFocusPolicy(Qt::NoFocus);

    m_snapshotFileTree->header()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    m_snapshotFileTree->header()->setSectionHidden(1, true);
    m_snapshotFileTree->header()->setSectionHidden(2, true);
    m_snapshotFileTree->header()->setSectionHidden(3, true);
    m_snapshotFileTree->header()->setStretchLastSection(true);

    auto fileTreeWidget = new QWidget(this);
    auto fileTreeLayout = new QVBoxLayout(fileTreeWidget);
    auto fileTreeContentMargins = fileTreeLayout->contentsMargins();
    fileTreeContentMargins.setRight(4);
    fileTreeLayout->setContentsMargins(fileTreeContentMargins);
    fileTreeLayout->addWidget(m_snapshotFileTree);

    m_originFileName = new QLabel(EmptyFileTitle, this);
    m_originFileContent = new QTextEdit(this);
    m_originFileContent->setReadOnly(true);
    m_originFileContent->setFont(ConsolasFont);
    m_originFileContent->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
    m_originFileDiffHighlighter = new DiffHighlighter(CaraTest::DiffChange::Deletion, RejectColor, m_originFileContent->document());
    m_rejectSnapshotButton = new QPushButton("Reject", this);
    m_rejectSnapshotButton->setEnabled(false);
    auto rejectStyleSheet = CaraTest::File::ReadContent(":/styles/rejectbutton.qss");
    m_rejectSnapshotButton->setStyleSheet(rejectStyleSheet);

    auto originWidget = new QWidget(this);
    auto originLayout = new QVBoxLayout(originWidget);
    auto originContentMargins = originLayout->contentsMargins();
    originContentMargins.setLeft(4);
    originContentMargins.setRight(4);
    originLayout->setContentsMargins(originContentMargins);
    originLayout->addWidget(m_originFileName);
    originLayout->addWidget(m_originFileContent);
    originLayout->addWidget(m_rejectSnapshotButton);

    m_snapshotFileName = new QLabel(EmptyFileTitle, this);
    m_snapshotFileContent = new QTextEdit(this);
    m_snapshotFileContent->setReadOnly(true);
    m_snapshotFileContent->setFont(ConsolasFont);
    m_snapshotFileContent->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
    m_snapshotFileDiffHighlighter = new DiffHighlighter(CaraTest::DiffChange::Addition, AcceptColor, m_snapshotFileContent->document());
    m_acceptSnapshotButton = new QPushButton("Accept", this);
    m_acceptSnapshotButton->setEnabled(false);
    auto acceptStyleSheet = CaraTest::File::ReadContent(":/styles/acceptbutton.qss");
    m_acceptSnapshotButton->setStyleSheet(acceptStyleSheet);

    auto snapshotWidget = new QWidget(this);
    auto snapshotLayout = new QVBoxLayout(snapshotWidget);
    auto snapshotContentMargins = snapshotLayout->contentsMargins();
    snapshotContentMargins.setLeft(4);
    snapshotLayout->setContentsMargins(snapshotContentMargins);
    snapshotLayout->addWidget(m_snapshotFileName);
    snapshotLayout->addWidget(m_snapshotFileContent);
    snapshotLayout->addWidget(m_acceptSnapshotButton);

    auto splitter = new QSplitter(this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(fileTreeWidget);
    splitter->addWidget(originWidget);
    splitter->addWidget(snapshotWidget);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 1);

    setCentralWidget(splitter);
    resize(1000, 600);

    QObject::connect(m_fileSystemModel, &QFileSystemModel::directoryLoaded, m_snapshotFileTree, &QTreeView::expandAll);
    QObject::connect(m_snapshotFileTree->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SnapshotToolWindow::onSnapshotClicked);
    QObject::connect(m_snapshotFileTree->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SnapshotToolWindow::onUpdateButtonState);
    QObject::connect(m_rejectSnapshotButton, &QPushButton::clicked, this, &SnapshotToolWindow::onRejectClicked);
    QObject::connect(m_acceptSnapshotButton, &QPushButton::clicked, this, &SnapshotToolWindow::onAcceptClicked);
    QObject::connect(m_selectedSnapshotWatcher, &QFileSystemWatcher::fileChanged, this, &SnapshotToolWindow::onCurrentDisplayedFileChanged);
}

void SnapshotToolWindow::showEvent(QShowEvent* event)
{
    static auto firstShowEvent = true;
    if (!firstShowEvent)
        return;

    firstShowEvent = false;

    const auto lastDirectory = LoadLastDirectory();
    if (!lastDirectory.isEmpty() && QDir(lastDirectory).exists())
    {
        setDirectoryToWatch(lastDirectory);
    }
    else
    {
        selectDirectoryToWatch();
    }
}

void SnapshotToolWindow::selectDirectoryToWatch()
{
    const auto dirPath = QFileDialog::getExistingDirectory(this, "Select Directory");
    if (dirPath.isNull())
        return;

    setDirectoryToWatch(dirPath);
    SaveLastDirectory(dirPath);
}

void SnapshotToolWindow::setDirectoryToWatch(const QString& dirPath)
{
    clearPreviews();
    m_snapshotFileTree->clearSelection();

    m_fileSystemModel->setRootPath(dirPath);
    m_snapshotFileTree->setRootIndex(m_fileSystemModel->index(dirPath));

    const auto windowTitle = QString("%1 - %2").arg(ToolName, dirPath);
    setWindowTitle(windowTitle);
}

void SnapshotToolWindow::clearPreviews()
{
    m_originFileName->setText(EmptyFileTitle);
    m_originFileContent->clear();
    m_originFileDiffHighlighter->clear();
    m_snapshotFileName->setText(EmptyFileTitle);
    m_snapshotFileContent->clear();
    m_snapshotFileDiffHighlighter->clear();

    clearSelectedSnapshotWatcher();
}

void SnapshotToolWindow::clearSelectedSnapshotWatcher()
{
    const auto watchedFiles = m_selectedSnapshotWatcher->files();
    if(!watchedFiles.isEmpty())
        m_selectedSnapshotWatcher->removePaths(watchedFiles);
}

void SnapshotToolWindow::updateHighlighters()
{
    const auto originFileContent = m_originFileContent->toPlainText();
    const auto snapshotFileContent = m_snapshotFileContent->toPlainText();
    const auto diffs = CaraTest::Diff(originFileContent, snapshotFileContent);

    m_originFileDiffHighlighter->highlightDiffs(diffs);
    m_snapshotFileDiffHighlighter->highlightDiffs(diffs);
}

void SnapshotToolWindow::onSnapshotClicked()
{
    const auto selectedIndexes = m_snapshotFileTree->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return;
    
    clearPreviews();

    const auto& index = selectedIndexes.first();
    const auto snapshotFileInfo = m_fileSystemModel->fileInfo(index);
    const auto isFile = snapshotFileInfo.isFile();
    if (!isFile)
        return;

    const auto originFileInfo = GetOriginFileInfoForSnapshot(snapshotFileInfo);
    const auto originExists = originFileInfo.exists();
    if (originExists)
    {
        const auto originFileName = originFileInfo.fileName();
        m_originFileName->setText(originFileName);
        const auto originFileContent = CaraTest::File::ReadContent(originFileInfo);
        m_originFileContent->setText(originFileContent);
        const auto originFilePath = originFileInfo.absoluteFilePath();
        m_selectedSnapshotWatcher->addPath(originFilePath);
    }

    const auto snapshotFileName = snapshotFileInfo.fileName();
    m_snapshotFileName->setText(snapshotFileName);
    const auto snapshotFileContent = CaraTest::File::ReadContent(snapshotFileInfo);
    m_snapshotFileContent->setText(snapshotFileContent);
    const auto snapshotFilePath = snapshotFileInfo.absoluteFilePath();
    m_selectedSnapshotWatcher->addPath(snapshotFilePath);

    updateHighlighters();
}

void SnapshotToolWindow::onUpdateButtonState()
{
    const auto selectedIndexes = m_snapshotFileTree->selectionModel()->selectedIndexes();
    const auto isFileSelected = (!selectedIndexes.isEmpty() && m_fileSystemModel->fileInfo(selectedIndexes.first()).isFile());

    m_acceptSnapshotButton->setEnabled(isFileSelected);
    m_rejectSnapshotButton->setEnabled(isFileSelected);
}

void SnapshotToolWindow::onRejectClicked()
{
    const auto selectedIndexes = m_snapshotFileTree->selectionModel()->selectedIndexes();
    const auto snapshotFileInfo = m_fileSystemModel->fileInfo(selectedIndexes.first());
    auto snapshotFile = QFile(snapshotFileInfo.absoluteFilePath());
    if (!snapshotFile.exists())
        return;

    clearSelectedSnapshotWatcher();

    const auto wasRemoved = snapshotFile.remove();
    if (wasRemoved)
        return;

    ShowWarningDialog(QString("Failed to delete the file: '%1'").arg(snapshotFile.fileName()));
}

void SnapshotToolWindow::onAcceptClicked()
{
    const auto selectedIndexes = m_snapshotFileTree->selectionModel()->selectedIndexes();
    const auto snapshotFileInfo = m_fileSystemModel->fileInfo(selectedIndexes.first());
    const auto snapshotFileName = snapshotFileInfo.fileName();
    auto snapshotFile = QFile(snapshotFileInfo.absoluteFilePath());
    if (!snapshotFile.exists())
        return;

    clearSelectedSnapshotWatcher();

    const auto originFileInfo = GetOriginFileInfoForSnapshot(snapshotFileInfo);
    const auto originFileName = originFileInfo.fileName();
    const auto originFilePath = originFileInfo.absoluteFilePath();
    auto originFile = QFile(originFileInfo.absoluteFilePath());
    if (originFile.exists())
    {
        const auto wasRemoved = originFile.remove();
        if (!wasRemoved)
        {
            ShowWarningDialog(QString("Failed to delete the file: '%1'").arg(originFileName));
            return;
        }
    }

    const auto wasRenamed = snapshotFile.rename(originFilePath);
    if(!wasRenamed)
        ShowWarningDialog(QString("Failed to rename the file: '%1' to '%2'").arg(snapshotFileName, originFileName));
}

void SnapshotToolWindow::onCurrentDisplayedFileChanged(const QString& filePath)
{
    // we might need to add the file again because some programs delete files before saving
    // https://doc.qt.io/qt-6/qfilesystemwatcher.html#fileChanged

    const auto watchedFiles = m_selectedSnapshotWatcher->files();
    if (!watchedFiles.contains(filePath))
        m_selectedSnapshotWatcher->addPath(filePath);

    const auto changedFile = QFileInfo(filePath);
    const auto fileExtension = changedFile.suffix();
    const auto snapshotExtension = SnapshotFileExtensionMask.mid(2); // remove the leading "*."
    const auto isSnapshot = fileExtension.compare(snapshotExtension, Qt::CaseSensitivity::CaseInsensitive) == 0;
    
    if (isSnapshot)
    {
        const auto snapshotFileContent = CaraTest::File::ReadContent(changedFile);
        m_snapshotFileContent->setText(snapshotFileContent);
    }
    else
    {
        const auto originFileContent = CaraTest::File::ReadContent(changedFile);
        m_originFileContent->setText(originFileContent);
    }

    updateHighlighters();
}
