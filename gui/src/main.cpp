// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <filesystem>
#include <QFile>
#include <QFileSystemWatcher>
#include <qguiapplication.h>
#include <QPainter>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSvgRenderer>
#include <QThreadPool>
#include <util/PathUtilsExtra.hpp>
#include "Config.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"
#include "util/Loggers.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("blaadpapers");
    QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
    QGuiApplication::setApplicationVersion(PROJECT_VERSION);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));

    Config::load();
    PostSetScript::createIfNotExists();

    if(!util::createDirIfNotExists(util::localDataDirPath())) {
        util::logWarn("Can't create directory \"{}\"", util::localDataDirPath().c_str());
    }

    if(!util::createDirIfNotExists(util::cacheDirPath())) {
        util::logWarn("Failed to create directory \"{}\"", util::cacheDirPath().c_str());
    }

    if(!QFile::exists(util::defaultWallpaperPath())) {
        // TODO Swap to ffmpeg api. Render for highest monitor resolution
        QSvgRenderer renderer(QString(":/qt/qml/BlaadPapers/resource/default-wallpaper.svg"));
        QImage defaultWallpaperImage(renderer.defaultSize(), QImage::Format_ARGB32);
        defaultWallpaperImage.fill(Qt::transparent);

        QPainter painter(&defaultWallpaperImage);
        renderer.render(&painter);

        defaultWallpaperImage.save(util::defaultWallpaperPath().c_str());
    }

    WallpapersModel::inst().load();

    util::logInfo("{}", WallpapersModel::inst().rowCount());

    #ifdef __linux__
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
