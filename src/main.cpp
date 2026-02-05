// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

extern "C" {
    #include <libavutil/log.h>
}

#include <QFileSystemWatcher>
#include <QLoggingCategory>
#include <QPainter>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSvgRenderer>
#include <QThreadPool>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "cli/OptionExecutor.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"

int main(int argc, char** argv) {
    av_log_set_level(AV_LOG_ERROR);
    QLoggingCategory::setFilterRules(
        "qt.text.font.db=false\n"
        "qt.gui.imageio=false"
    );

    Config::load();

    util::createDirIfNotExists(util::getLocalDataPath());

    if(!QFile::exists(util::getDefaultWallpaperPath())) {
        // TODO Swap to ffmpeg api. Render for highest monitor resolution
        QSvgRenderer renderer(QString(":/qt/qml/BlaadPapers/resource/default-wallpaper.svg"));
        QImage defaultWallpaperImage(renderer.defaultSize(), QImage::Format_ARGB32);
        defaultWallpaperImage.fill(Qt::transparent);

        QPainter painter(&defaultWallpaperImage);
        renderer.render(&painter);

        defaultWallpaperImage.save(util::getDefaultWallpaperPath());
    }

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

        //TODO Redo watchers
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

        #if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
        if(qgetenv("QT_QUICK_CONTROLS_STYLE").isNull()) {
            QQuickStyle::setStyle("BStyle");
        }
        #endif

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("Wallpapers", &WallpapersModel::inst());
        engine.rootContext()->setContextProperty("Config", &ConfigModel::inst());
        engine.rootContext()->setContextProperty("Status", &StatusModel::inst());
        engine.loadFromModule(PROJECT_NAME, "MainWindow");

        return QGuiApplication::exec();
    }
}
