#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "OptionExecutor.hpp"
#include "Wallpapers.hpp"
#include "model/WallpaperList.hpp"

int main(int argc, char** argv) {
    Wallpapers::load();

    if(argc > 1) {
        OptionExecutor::execute(argc, argv);
    } else {
        QGuiApplication app(argc, argv);
        QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        WallpaperList::loadPreviews();

        qmlRegisterSingletonInstance<WallpaperList>(PROJECT_NAME, 1, 0, "WallpaperList", new WallpaperList);

        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
