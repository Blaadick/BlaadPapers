// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QThreadPool>
#include <vips/vips8>
#include "Config.hpp"
#include "DefaultWallpaper.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("blaadpapers");
    QApplication::setApplicationDisplayName(PROJECT_NAME);
    QApplication::setApplicationVersion(PROJECT_VERSION);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));

    vips_init(argv[0]);
    vips_cache_set_max(0);

    Config::load();
    PostSetScript::createIfNotExists();
    DefaultWallpaper::createIfNotExists();
    WallpapersModel::inst().loadWallpapers();

    #ifdef __linux__
    if(!getenv("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle("BStyle");
    }
    #endif

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Wallpapers", &WallpapersModel::inst());
    engine.rootContext()->setContextProperty("Config", &ConfigModel::inst());
    engine.rootContext()->setContextProperty("Status", &StatusModel::inst());
    engine.loadFromModule(PROJECT_NAME, "MainWindow");

    QObject::connect(
        &app,
        &QCoreApplication::aboutToQuit,
        [] {
            vips_shutdown();
        }
    );

    return QApplication::exec();
}
