#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "Config.hpp"
#include "OptionExecutor.hpp"
#include "Wallpapers.hpp"
#include "model/WallpaperList.hpp"

int main(int argc, char** argv) {
    Config::load();
    Wallpapers::load();

    if(argc > 1) {
        OptionExecutor::execute(argc, argv);
    } else {
        QGuiApplication app(argc, argv);
        QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        QFileSystemWatcher configWatcher;
        configWatcher.addPath(Config::getConfigPath());
        QObject::connect(&configWatcher, &QFileSystemWatcher::fileChanged, [] {
            //TODO Fix double config update emit
            Config::load();

            Wallpapers::load();
            WallpaperList::loadPreviews();
        });

        QFileSystemWatcher wallpapersWatcher;
        wallpapersWatcher.addPath(Config::getWorkingPath());
        QObject::connect(&wallpapersWatcher, &QFileSystemWatcher::directoryChanged, [] {
            Wallpapers::load();
            WallpaperList::loadPreviews();
        });

        WallpaperList::loadPreviews();

        qmlRegisterSingletonInstance<WallpaperList>(PROJECT_NAME, 1, 0, "WallpaperList", new WallpaperList(&app));

        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
