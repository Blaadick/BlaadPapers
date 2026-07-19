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
#include "logger/GuiLogger.hpp"
#include "model/ClipboardModel.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"
#include "preview/generator/PicturePreviewGenerator.hpp"
#include "preview/generator/VideoPreviewGenerator.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("blaadpapers");
    QApplication::setApplicationDisplayName(PROJECT_NAME);
    QApplication::setApplicationVersion(PROJECT_VERSION);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));

    vips_init(argv[0]);
    vips_cache_set_max(0);

    DefaultWallpaper::createIfNotExists(false);
    PostSetScript::createIfNotExists();

    auto clipboardModel = std::make_shared<ClipboardModel>();
    auto statusModel = std::make_shared<StatusModel>();
    auto logger = std::make_shared<util::GuiLogger>(statusModel);
    auto config = std::make_shared<Config>(logger);
    config->load();

    auto wallpapers = std::make_shared<Wallpapers>();
    auto wallpaperLoader = std::make_shared<WallpaperLoader>(wallpapers, config, logger);

    auto previewManager = std::make_shared<PreviewManager>(logger);
    previewManager->addGenerator(typeid(PictureWallpaper), std::make_unique<PicturePreviewGenerator>());
    previewManager->addGenerator(typeid(VideoWallpaper), std::make_unique<VideoPreviewGenerator>());

    auto configModel = std::make_shared<ConfigModel>(config);
    auto wallpapersModel = std::make_shared<WallpapersModel>(wallpaperLoader, wallpapers, config, previewManager, logger);
    wallpapersModel->loadWallpapers();

    #ifdef __linux__
    if(!getenv("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle("BStyle");
    }
    #endif

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Wallpapers", &*wallpapersModel);
    engine.rootContext()->setContextProperty("Config", &*configModel);
    engine.rootContext()->setContextProperty("Status", &*statusModel);
    engine.rootContext()->setContextProperty("Clipboard", &*clipboardModel);
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
