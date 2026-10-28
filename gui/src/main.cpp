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
#include "SecretStorage.hpp"
#include "ShellExecutor.hpp"
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
    av_log_set_level(AV_LOG_ERROR);
    QLoggingCategory::setFilterRules("qt.multimedia.*.info=false");

    vips_init(argv[0]);
    vips_cache_set_max(0);

    if(!std::filesystem::exists(DefaultWallpaper::defaultWallpaperFilePath())) {
        DefaultWallpaper::create();
    }

    auto statusModel = std::make_shared<StatusModel>();
    auto logger = std::make_shared<util::GuiLogger>(statusModel);
    auto shellExecutor = std::make_shared<ShellExecutor>();
    auto httpClient = std::make_shared<HttpClient>();
    auto fetchManager = std::make_shared<FetchManager>();
    fetchManager->addFetcher("http", std::make_unique<HttpFetcher>(httpClient, logger));
    fetchManager->addFetcher("https", std::make_unique<HttpFetcher>(httpClient, logger));
    fetchManager->addFetcher("file", std::make_unique<FileFetcher>());

    auto secretStorage = std::make_shared<SecretStorage>();
    auto config = std::make_shared<Config>(logger, secretStorage);
    config->load();

    auto wallpaperRepository = std::make_shared<WallpaperRepository>(config, shellExecutor);
    auto wallpaperLoaderManager = std::make_shared<WallpaperLoaderManager>(wallpaperRepository, config, shellExecutor, logger);
    wallpaperLoaderManager->addWallpaperLoader<PictureWallpaper>(std::make_unique<PictureWallpaperLoader>(config));
    wallpaperLoaderManager->addWallpaperLoader<VideoWallpaper>(std::make_unique<VideoWallpaperLoader>(config));

    auto fmt = QSurfaceFormat::defaultFormat();
    fmt.setRedBufferSize(8);
    fmt.setGreenBufferSize(8);
    fmt.setBlueBufferSize(8);
    fmt.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

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

    auto previewManager = std::make_shared<PreviewManager>(logger);
    previewManager->addGenerator<PictureWallpaper>(std::make_unique<PicturePreviewGenerator>());
    previewManager->addGenerator<VideoWallpaper>(std::make_unique<VideoPreviewGenerator>(shellExecutor));

    auto clipboardModel = std::make_shared<ClipboardModel>();
    auto configModel = std::make_shared<ConfigModel>(config);
    auto wallpapersModel = std::make_shared<WallpapersModel>(wallpaperLoaderManager, wallpaperRepository, fetchManager, config, previewManager, logger);
    wallpapersModel->loadWallpapers();
    logger->logInfo(std::format("Loaded {} wallpapers", wallpaperRepository->count()));

    QQmlApplicationEngine engine;
    engine.setUiLanguage(configModel->getUiLanguage());
    engine.rootContext()->setContextProperty("Wallpapers", wallpapersModel.get());
    engine.rootContext()->setContextProperty("Config", configModel.get());
    engine.rootContext()->setContextProperty("Status", statusModel.get());
    engine.rootContext()->setContextProperty("Clipboard", clipboardModel.get());
    engine.loadFromModule(PROJECT_NAME, "MainWindow");

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &vips_shutdown);
    return QGuiApplication::exec();
}
