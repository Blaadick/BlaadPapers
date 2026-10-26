// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCommandLineParser>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QStyleHints>
#include <vips/vips8>
#include "DefaultWallpaper.hpp"
#include "PostSetScript.hpp"
#include "config/Config.hpp"
#include "logger/GuiLogger.hpp"
#include "model/ClipboardModel.hpp"
#include "model/ConfigModel.hpp"
#include "model/StatusModel.hpp"
#include "model/WallpapersModel.hpp"
#include "network/file/FileFetcher.hpp"
#include "network/http/HttpFetcher.hpp"
#include "preview/generator/PicturePreviewGenerator.hpp"
#include "preview/generator/VideoPreviewGenerator.hpp"
#include "wallpaper/PictureWallpaper.hpp"
#include "wallpaper/VideoWallpaper.hpp"
#include "wallpaper/loader/PictureWallpaperLoader.hpp"
#include "wallpaper/loader/VideoWallpaperLoader.hpp"
#include "wallpaper/loader/WallpaperLoaderManager.hpp"

extern "C" {
    #include <libavutil/log.h>
}

auto main(int argc, char* argv[]) -> int {
    vips_init(argv[0]);
    vips_cache_set_max(0);

    av_log_set_level(AV_LOG_ERROR);
    QLoggingCategory::setFilterRules("qt.multimedia.*.info=false");

    auto statusModel = std::make_shared<StatusModel>();
    auto logger = std::make_shared<util::GuiLogger>(statusModel);
    auto config = std::make_shared<Config>(logger);
    config->load();

    QGuiApplication::setDesktopFileName("blaadpapers");
    QGuiApplication::setApplicationName(PROJECT_NAME);
    QGuiApplication::setApplicationDisplayName(PROJECT_NAME);
    QGuiApplication::setApplicationVersion(PROJECT_VERSION);
    QGuiApplication app(argc, argv);

    QThreadPool::globalInstance()->setMaxThreadCount(std::ceil(QThread::idealThreadCount() / 2));
    QIcon::setThemeName("blaadpapers");

    if(config->getColorScheme() == "dark") {
        QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Dark);
    } else if(config->getColorScheme() == "light") {
        QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
    } else {
        QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Unknown);
    }

    if(config->getNativeTextRendering()) {
        QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    }

    QCommandLineParser parser;
    parser.setApplicationDescription(PROJECT_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    if(!std::filesystem::exists(DefaultWallpaper::defaultWallpaperFilePath())) {
        DefaultWallpaper::create();
    }

    PostSetScript::createIfNotExists();

    auto wallpaperRepository = std::make_shared<WallpaperRepository>();
    auto wallpaperLoaderManager = std::make_shared<WallpaperLoaderManager>(wallpaperRepository, config, logger);
    wallpaperLoaderManager->addWallpaperLoader<PictureWallpaper>(std::make_unique<PictureWallpaperLoader>(config));
    wallpaperLoaderManager->addWallpaperLoader<VideoWallpaper>(std::make_unique<VideoWallpaperLoader>(config));

    auto previewManager = std::make_shared<PreviewManager>(logger);
    previewManager->addGenerator<PictureWallpaper>(std::make_unique<PicturePreviewGenerator>());
    previewManager->addGenerator<VideoWallpaper>(std::make_unique<VideoPreviewGenerator>());

    auto httpClient = std::make_shared<HttpClient>();
    auto fetchManager = std::make_shared<FetchManager>();
    fetchManager->addFetcher("http", std::make_unique<HttpFetcher>(httpClient, logger));
    fetchManager->addFetcher("https", std::make_unique<HttpFetcher>(httpClient, logger));
    fetchManager->addFetcher("file", std::make_unique<FileFetcher>());

    auto clipboardModel = std::make_shared<ClipboardModel>();
    auto configModel = std::make_shared<ConfigModel>(config);
    auto wallpapersModel = std::make_shared<WallpapersModel>(wallpaperLoaderManager, wallpaperRepository, fetchManager, config, previewManager, logger);
    wallpapersModel->loadWallpapers();
    logger->logInfo(std::format("Loaded {} wallpapers", wallpaperRepository->count()));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Wallpapers", wallpapersModel.get());
    engine.rootContext()->setContextProperty("Config", configModel.get());
    engine.rootContext()->setContextProperty("Status", statusModel.get());
    engine.rootContext()->setContextProperty("Clipboard", clipboardModel.get());
    engine.loadFromModule(PROJECT_NAME, "MainWindow");

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &vips_shutdown);
    return QGuiApplication::exec();
}
