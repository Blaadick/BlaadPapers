// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model/WallpapersModel.hpp"

#include <QSize>
#include <QtConcurrentMap>
#include <QThreadPool>
#include "wallpaper/PictureWallpaper.hpp"
#include "wallpaper/VideoWallpaper.hpp"
#include "util/FormatExt.hpp"
#include "util/FormatUtils.hpp"

namespace fs = std::filesystem;

WallpapersModel::WallpapersModel(
    sptr<WallpaperLoaderManager> wallpaperLoaderManager,
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<FetchManager> fetchManager,
    sptr<Config> config,
    sptr<PreviewManager> previewManager,
    sptr<util::Logger> logger
) : wallpaperLoaderManager(std::move(wallpaperLoaderManager)), wallpaperRepository(std::move(wallpaperRepository)), fetchManager(std::move(fetchManager)), config(std::move(config)), previewManager(std::move(previewManager)), logger(std::move(logger)) {}

void WallpapersModel::loadWallpapers() {
    beginResetModel();
    wallpaperLoaderManager->loadWallpapers();
    wallpaperRepository->sortByName();
    endResetModel();

    QtConcurrent::map(
        *wallpaperRepository,
        [this](const sptr<Wallpaper>& wallpaper) {
            previewManager->createAndSavePreviews(*wallpaper);
        }
    );
}

auto WallpapersModel::getFileDialogNameFilters() const -> QStringList {
    std::unordered_set<const file::FileType*> allSupportedFormats;
    for(const auto& loader : wallpaperLoaderManager->getWallpaperLoaders() | std::views::values) {
        const auto loaderFormats = loader->getSupportedFileTypes();
        allSupportedFormats.insert(loaderFormats.begin(), loaderFormats.end());
    }

    return {
        QString("Supported Files (%1)").arg(util::getFormatString(allSupportedFormats)),
        QString("Any Files (*)")
    };
}

void WallpapersModel::downloadAndInstallWallpaperAsync(const QUrl& url) {
    QThreadPool::globalInstance()->start(
        [this, url] {
            downloadAndInstallWallpaper(url);
            refreshWallpapers();
        }
    );
}

void WallpapersModel::downloadAndInstallWallpapersAsync(const QList<QUrl>& urlList) {
    QThreadPool::globalInstance()->start(
        [this, urlList] {
            for(auto& url : urlList) {
                downloadAndInstallWallpaper(url);
            }

            refreshWallpapers();
        }
    );
}

void WallpapersModel::deleteWallpaperAsync(const QString& wallpaperId) {
    QThreadPool::globalInstance()->start(
        [this, wallpaperId] {
            deleteWallpaper(wallpaperId);
            refreshWallpapers();
        }
    );
}

void WallpapersModel::deleteWallpapersAsync(const QStringList& wallpaperIds) {
    QThreadPool::globalInstance()->start(
        [this, wallpaperIds] {
            for(auto& wallpaperId : wallpaperIds) {
                deleteWallpaper(wallpaperId);
            }

            refreshWallpapers();
        }
    );
}

void WallpapersModel::applyWallpaperAsync(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [this, wallpaperId] {
            auto result = wallpaperRepository->apply(wallpaperId.toStdString());
            if(result.has_value()) {
                logger->logInfo(std::format("Wallpaper \"{}\" applied", wallpaperId));
            } else {
                logger->logWarning(std::format("Failed to apply wallpaper \"{}\": {}", wallpaperId, result.error()));
            }
        }
    );
}

void WallpapersModel::refreshWallpapers() {
    loadWallpapers();
}

auto WallpapersModel::rowCount(const QModelIndex&) const -> int {
    return wallpaperRepository->count();
}

auto WallpapersModel::data(const QModelIndex& index, const int role) const -> QVariant {
    auto wallpaper = wallpaperRepository->get(index.row());
    auto pictureWallpaper = std::dynamic_pointer_cast<PictureWallpaper>(wallpaper);
    auto videoWallpaper = std::dynamic_pointer_cast<VideoWallpaper>(wallpaper);

    QStringList qStringTags;
    qStringTags.reserve(wallpaper->getTags().size());
    for(const auto& tag : wallpaper->getTags()) {
        qStringTags.append(QString::fromStdString(tag));
    }

    switch(role) {
        case IdRole: return QString::fromStdString(wallpaper->getId());
        case NameRole: return QString::fromStdString(wallpaper->getName());
        case RootDirRole: return QUrl::fromLocalFile(QString::fromStdString(wallpaper->getRootDirPath().string()));
        case ResolutionRole: {
            if(pictureWallpaper) {
                return QSize(pictureWallpaper->getResolution().width, pictureWallpaper->getResolution().height);
            }

            if(videoWallpaper) {
                return QSize(videoWallpaper->getResolution().width, videoWallpaper->getResolution().height);
            }

            return QVariant();
        }
        case SourceRole: return QString::fromStdString(wallpaper->getSource());
        case TagsRole: return qStringTags;
        case FrameRateRole: return videoWallpaper ? videoWallpaper->getFrameRate() : QVariant();
        case TypeRole: {
            if(pictureWallpaper) {
                return QString("Picture");
            }

            if(videoWallpaper) {
                return QString("Video");
            }

            return QVariant();
        }
        case IsBadRole: return config->isWallpaperBad(*wallpaper);
        default: return QVariant();
    }
}

auto WallpapersModel::roleNames() const -> QHash<int, QByteArray> {
    return {
        {IdRole, "wallpaperId"},
        {NameRole, "wallpaperName"},
        {RootDirRole, "wallpaperRootDir"},
        {ResolutionRole, "wallpaperResolution"},
        {SourceRole, "wallpaperSource"},
        {TagsRole, "wallpaperTags"},
        {FrameRateRole, "wallpaperFrameRate"},
        {TypeRole, "wallpaperType"},
        {IsBadRole, "isWallpaperBad"},
    };
}

void WallpapersModel::downloadAndInstallWallpaper(const QUrl& url) const {
    auto uri = Uri::parse(url.toString(QUrl::FullyEncoded).toStdString());
    if(!uri.has_value()) {
        logger->logWarning(std::format("Failed to parse \"{}\" as URI", url.toString(QUrl::FullyEncoded)));
        return;
    }

    auto downloadedFilePath = fetchManager->fetchFile(*uri, util::localDownloadsDir());
    if(!downloadedFilePath.has_value()) {
        logger->logWarning(std::format("Failed to download file from \"{}\": {}", *uri, downloadedFilePath.error()));
        return;
    }

    auto wallpaper = wallpaperLoaderManager->installWallpaper(*downloadedFilePath);
    if(wallpaper.has_value()) {
        wallpaperRepository->add(*wallpaper);
        logger->logInfo(std::format("Wallpaper \"{}\" installed", wallpaper.value()->getId()));
    } else {
        logger->logWarning(std::format("Failed to install \"{}\": {}", downloadedFilePath->filename(), wallpaper.error()));
    }

    fs::remove(*downloadedFilePath);
}

void WallpapersModel::deleteWallpaper(const QString& wallpaperId) const {
    const auto result = wallpaperRepository->remove(wallpaperId.toStdString());
    if(result.has_value()) {
        logger->logInfo(std::format("Wallpaper \"{}\" deleted", wallpaperId));
    } else {
        logger->logWarning(std::format("Failed to delete wallpaper \"{}\": {}", wallpaperId, result.error()));
    }
}
