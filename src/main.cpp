#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "Config.hpp"
#include "OptionExecutor.hpp"
#include "Wallpapers.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
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

        WallpapersModel::inst().load();

        QFileSystemWatcher wallpapersWatcher;
        wallpapersWatcher.addPath(Config::getWorkingPath());
        QObject::connect(&wallpapersWatcher, &QFileSystemWatcher::directoryChanged, [] {
            WallpapersModel::inst().load();
        });

        QFileSystemWatcher configWatcher;
        configWatcher.addPath(Config::getConfigPath());
        QObject::connect(&configWatcher, &QFileSystemWatcher::fileChanged, [] {
            Config::load();
            WallpapersModel::inst().load();

            logInfo("Config reloaded");
            sendStatus("Config reloaded");
        });

        qmlRegisterSingletonInstance<WallpapersModel>(PROJECT_NAME, 1, 0, "WallpapersModel", &WallpapersModel::inst());
        qmlRegisterSingletonInstance<ConfigModel>(PROJECT_NAME, 1, 0, "ConfigModel", &ConfigModel::inst());
        qmlRegisterSingletonInstance<StatusModel>(PROJECT_NAME, 1, 0, "StatusModel", &StatusModel::inst());
        QQmlApplicationEngine engine;
        engine.addImportPath("");
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        logInfo("Loaded {} wallpapers", Wallpapers::getWallpapers().count());
        sendStatus("Loaded {} wallpapers", Wallpapers::getWallpapers().count());

        return QGuiApplication::exec();
    }
}
