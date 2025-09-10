#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "Config.hpp"
#include "OptionExecutor.hpp"
#include "Wallpapers.hpp"
#include "model/WallpaperList.hpp"

int main(int argc, char** argv) {
    Config::read();
    Wallpapers::load();

    if(argc > 1) {
        OptionExecutor::execute(argc, argv);
    } else {
        QGuiApplication app(argc, argv);
        QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        QFileSystemWatcher configWatcher;
        configWatcher.addPath(Config::getConfigPath());
        QObject::connect(&configWatcher, &QFileSystemWatcher::fileChanged, Config::update);

        WallpaperList::loadPreviews();

        qmlRegisterSingletonInstance<WallpaperList>(PROJECT_NAME, 1, 0, "WallpaperList", new WallpaperList(&app));

        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
