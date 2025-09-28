#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
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

    if(argc > 1) {
        Wallpapers::load();
        OptionExecutor::execute(argc, argv);
    } else {
        QGuiApplication app(argc, argv);
        QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        WallpapersModel::inst().load();

        QFileSystemWatcher wallpapersWatcher;
        wallpapersWatcher.addPath(Config::getWallpapersPath());
        QObject::connect(&wallpapersWatcher, &QFileSystemWatcher::directoryChanged, [] {
            WallpapersModel::inst().load();
        });

        QFileSystemWatcher configWatcher;
        configWatcher.addPath(Config::getConfigFilePath());
        QObject::connect(&configWatcher, &QFileSystemWatcher::fileChanged, [] {
            Config::load();
            WallpapersModel::inst().load();

            util::logInfo("Config reloaded");
            util::sendStatus("Config reloaded");
        });

        if(qgetenv("QT_QUICK_CONTROLS_STYLE").isEmpty()) {
            QQuickStyle::setStyle("BStyle");
        }

        qmlRegisterSingletonInstance<WallpapersModel>(PROJECT_NAME, 1, 0, "WallpapersModel", &WallpapersModel::inst());
        qmlRegisterSingletonInstance<ConfigModel>(PROJECT_NAME, 1, 0, "ConfigModel", &ConfigModel::inst());
        qmlRegisterSingletonInstance<StatusModel>(PROJECT_NAME, 1, 0, "StatusModel", &StatusModel::inst());
        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
