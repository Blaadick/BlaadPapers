// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QThreadPool>
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

        QThreadPool::globalInstance()->setMaxThreadCount(std::max(1, QThread::idealThreadCount() - 2));
        WallpapersModel::inst().load();

        // QFileSystemWatcher wallpapersWatcher;
        // wallpapersWatcher.addPath(Config::getWallpapersDirPath());
        // QObject::connect(&wallpapersWatcher, &QFileSystemWatcher::directoryChanged, [] {
        //     WallpapersModel::inst().load();
        // });
        //
        // QFileSystemWatcher configWatcher;
        // configWatcher.addPath(Config::getConfigFilePath());
        // QObject::connect(&configWatcher, &QFileSystemWatcher::fileChanged, [] {
        //     Config::load();
        //     WallpapersModel::inst().load();
        //
        //     util::logInfo("Config reloaded");
        //     util::sendStatus("Config reloaded");
        // });

        #ifdef __linux__
        if(qgetenv("QT_QUICK_CONTROLS_STYLE").isNull()) {
            QQuickStyle::setStyle("BStyle");
        }
        #endif

        qmlRegisterSingletonInstance<WallpapersModel>(PROJECT_NAME, 1, 0, "WallpapersModel", &WallpapersModel::inst());
        qmlRegisterSingletonInstance<ConfigModel>(PROJECT_NAME, 1, 0, "ConfigModel", &ConfigModel::inst());
        qmlRegisterSingletonInstance<StatusModel>(PROJECT_NAME, 1, 0, "StatusModel", &StatusModel::inst());
        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
