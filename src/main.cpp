// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

extern "C" {
    #include <libavutil/log.h>
}

#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QThreadPool>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "cli/OptionExecutor.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"
#include "util/Loggers.hpp"

int main(int argc, char** argv) {
    av_log_set_level(AV_LOG_ERROR);

    Config::load();

    if(argc > 1) {
        Wallpapers::load();
        OptionExecutor::execute(argc, argv);
    } else {
        QGuiApplication app(argc, argv);
        QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

        QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));
        WallpapersModel::inst().load();

        util::logInfo("Loaded {} wallpapers", Wallpapers::count());
        util::sendStatus("Loaded {} wallpapers", Wallpapers::count());

        QFileSystemWatcher wallpapersWatcher;
        wallpapersWatcher.addPath(Config::getWallpapersDirPath());
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

        #ifdef __linux__
        if(qgetenv("QT_QUICK_CONTROLS_STYLE").isNull()) {
            QQuickStyle::setStyle("BStyle");
        }
        #endif

        qmlRegisterSingletonInstance<WallpapersModel>(PROJECT_NAME, 1, 0, "Wallpapers", &WallpapersModel::inst());
        qmlRegisterSingletonInstance<ConfigModel>(PROJECT_NAME, 1, 0, "Config", &ConfigModel::inst());
        qmlRegisterSingletonInstance<StatusModel>(PROJECT_NAME, 1, 0, "Status", &StatusModel::inst());
        QQmlApplicationEngine engine;
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
