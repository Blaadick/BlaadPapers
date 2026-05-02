// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QThreadPool>
#include <vips/vips8>
#include "Config.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
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

    vips_init(argv[0]);
    vips_cache_set_max(0);

    Config::load();
    PostSetScript::createIfNotExists();
    WallpapersModel::inst().load();

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

    return QGuiApplication::exec();
}
