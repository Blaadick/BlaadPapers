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
        QGuiApplication::setApplicationDisplayName("BlaadPapers");
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        WallpaperList::loadPreviews();

        qmlRegisterSingletonInstance<WallpaperList>("BlaadPapers", 1, 0, "WallpaperList", new WallpaperList);

        QQmlApplicationEngine engine;
        engine.loadFromModule("BlaadPapers", "MainWindow");

        return QGuiApplication::exec();
    }
}
