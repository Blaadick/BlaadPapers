// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QThreadPool>
#include <vips/vips8>
#include "DefaultWallpaper.hpp"
#include "PostSetScript.hpp"
#include "config/Config.hpp"
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "logger/GuiLogger.hpp"
#include "model/ClipboardModel.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"
#include "network/file/FileDownloader.hpp"
#include "network/http/HttpDownloader.hpp"
#include "preview/generator/PicturePreviewGenerator.hpp"
#include "preview/generator/VideoPreviewGenerator.hpp"
#include "wallpaper_loader/PictureWallpaperLoader.hpp"
#include "wallpaper_loader/VideoWallpaperLoader.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

auto main(int argc, char* argv[]) -> int {
    // TODO Interim colors distortion fix
    QSurfaceFormat fmt;
    fmt.setRedBufferSize(8);
    fmt.setGreenBufferSize(8);
    fmt.setBlueBufferSize(8);
    fmt.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(PROJECT_NAME);
    QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
    QGuiApplication::setApplicationVersion(PROJECT_VERSION);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));

    QCommandLineParser parser;
    parser.setApplicationDescription(PROJECT_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    vips_init(argv[0]);
    vips_cache_set_max(0);

    if(!std::filesystem::exists(DefaultWallpaper::defaultWallpaperFilePath())) {
        DefaultWallpaper::create();
    }

    PostSetScript::createIfNotExists();

    auto clipboardModel = std::make_shared<ClipboardModel>();

    auto statusModel = std::make_shared<StatusModel>();
    auto logger = std::make_shared<util::GuiLogger>(statusModel);
    auto config = std::make_shared<Config>(logger);
    config->load();

    auto wallpapers = std::make_shared<WallpaperRepository>();
    auto wallpaperLoader = std::make_shared<WallpaperLoaderManager>(wallpapers, config, logger);
    wallpaperLoader->addWallpaperLoader<PictureWallpaper>(std::make_unique<PictureWallpaperLoader>(config));
    wallpaperLoader->addWallpaperLoader<VideoWallpaper>(std::make_unique<VideoWallpaperLoader>(config));

    auto previewManager = std::make_shared<PreviewManager>(logger);
    previewManager->addGenerator<PictureWallpaper>(std::make_unique<PicturePreviewGenerator>());
    previewManager->addGenerator<VideoWallpaper>(std::make_unique<VideoPreviewGenerator>());

    auto httpClient = std::make_shared<HttpClient>();
    auto downloadManager = std::make_shared<DownloadManager>();
    downloadManager->addDownloader("http", std::make_unique<HttpDownloader>(httpClient));
    downloadManager->addDownloader("https", std::make_unique<HttpDownloader>(httpClient));
    downloadManager->addDownloader("file", std::make_unique<FileDownloader>());

    auto configModel = std::make_shared<ConfigModel>(config);
    auto wallpapersModel = std::make_shared<WallpapersModel>(wallpaperLoader, wallpapers, downloadManager, config, previewManager, logger);
    wallpapersModel->loadWallpapers();
    logger->logInfo(std::format("Loaded {} wallpapers", wallpapers->count()));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Wallpapers", &*wallpapersModel);
    engine.rootContext()->setContextProperty("Config", &*configModel);
    engine.rootContext()->setContextProperty("Status", &*statusModel);
    engine.rootContext()->setContextProperty("Clipboard", &*clipboardModel);
    engine.loadFromModule(PROJECT_NAME, "MainWindow");

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &vips_shutdown);
    return QGuiApplication::exec();
}
