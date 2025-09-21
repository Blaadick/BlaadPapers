#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "Config.hpp"
#include "OptionExecutor.hpp"
#include "Wallpapers.hpp"
#include "model/WallpapersModel.hpp"
#include "util/Loggers.hpp"

int main(int argc, char** argv) {
    Config::load();
    Wallpapers::load();

    if(argc > 1) {
        OptionExecutor::execute(argc, argv);
    } else {
        QGuiApplication app(argc, argv);
        QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        const auto wallpaperModel = new WallpapersModel(&app);
        WallpapersModel::loadPreviews();

        QFileSystemWatcher wallpapersWatcher;
        wallpapersWatcher.addPath(Config::getWorkingPath());
        QObject::connect(&wallpapersWatcher, &QFileSystemWatcher::directoryChanged, [wallpaperModel] {
            Wallpapers::load();
            WallpapersModel::loadPreviews();
            wallpaperModel->refresh();

            logInfo("Wallpapers reloaded");
        });

        QFileSystemWatcher configWatcher;
        configWatcher.addPath(Config::getConfigPath());
        QObject::connect(&configWatcher, &QFileSystemWatcher::fileChanged, [wallpaperModel] {
            Config::load();
            Wallpapers::load();
            WallpapersModel::loadPreviews();
            wallpaperModel->refresh();

            logInfo("Wallpapers reloaded");
        });

        qmlRegisterSingletonInstance<WallpapersModel>(PROJECT_NAME, 1, 0, "WallpapersModel", wallpaperModel);
        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
