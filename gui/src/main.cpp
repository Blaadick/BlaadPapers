// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

extern "C" {
    #include <libavutil/log.h>
}

#include <QFileSystemWatcher>
#include <QPainter>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSvgRenderer>
#include <QThreadPool>
#include <util/ExtraPathUtils.hpp>
#include "Config.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("blaadpapers");
    QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
    QGuiApplication::setApplicationVersion(PROJECT_VERSION);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));
    av_log_set_level(AV_LOG_ERROR);

    Config::load();
    util::createDirIfNotExists(util::getDataPath());

    if(!QFile::exists(util::getDefaultWallpaperPath())) {
        // TODO Swap to ffmpeg api. Render for highest monitor resolution
        QSvgRenderer renderer(QString(":/qt/qml/BlaadPapers/resource/default-wallpaper.svg"));
        QImage defaultWallpaperImage(renderer.defaultSize(), QImage::Format_ARGB32);
        defaultWallpaperImage.fill(Qt::transparent);

        QPainter painter(&defaultWallpaperImage);
        renderer.render(&painter);

        defaultWallpaperImage.save(util::getDefaultWallpaperPath());
    }

    WallpapersModel::inst().load();

    //TODO Redo watchers
    QFileSystemWatcher wallpapersWatcher;
    wallpapersWatcher.addPaths(Config::getWallpaperDirPaths());
    QObject::connect(
        &wallpapersWatcher,
        &QFileSystemWatcher::directoryChanged,
        [] {
            WallpapersModel::inst().load();
        }
    );

    QFileSystemWatcher configWatcher;
    configWatcher.addPath(Config::getConfigFilePath());
    QObject::connect(
        &configWatcher,
        &QFileSystemWatcher::fileChanged,
        [] {
            Config::load();
            WallpapersModel::inst().load();

            util::logInfo("Config reloaded");
            util::sendStatus("Config reloaded");
        }
    );

    #if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    if(!qEnvironmentVariableIsSet("QT_QUICK_CONTROLS_STYLE")) {
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
