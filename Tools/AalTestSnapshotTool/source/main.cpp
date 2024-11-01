#include <QtWidgets/QApplication>

#include "SnapshotToolWindow.h"
#include <AalTest/File.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "dwmapi.h"

static void SetDarkModeTitleBar(const QMainWindow& window)
{
    const auto useDarkMode = TRUE;
    DwmSetWindowAttribute((HWND)window.winId(), DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
}
#else
static void SetDarkModeTitleBar(const QMainWindow& window) {}
#endif

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    const auto styleSheet = AalTest::ReadFileContent(":/styles/darkmode.qss");
    application.setStyleSheet(styleSheet);

    SnapshotToolWindow window;
    SetDarkModeTitleBar(window);
    window.show();
    
    return application.exec();
}
