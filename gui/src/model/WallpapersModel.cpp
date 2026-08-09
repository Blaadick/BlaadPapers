// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <iostream>
#include <QFileDialog>
#include <QtConcurrentMap>
#include <QThreadPool>
#include "WallpaperRepository.hpp"
#include "config/Config.hpp"
#include "util/FormatUtils.hpp"
#include "util/StringUtils.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

namespace fs = std::filesystem;

WallpapersModel::WallpapersModel(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<Config> config,
    sptr<PreviewManager> previewManager,
    sptr<util::Logger> logger
) : wallpaperLoader(std::move(wallpaperLoader)), wallpaperRepository(std::move(wallpaperRepository)), config(std::move(config)), previewManager(std::move(previewManager)), logger(std::move(logger)) {}

void WallpapersModel::loadWallpapers() {
    beginResetModel();
    wallpaperLoader->loadWallpapers();
    wallpaperRepository->sortByName();
    endResetModel();

    QtConcurrent::map(
        *wallpaperRepository,
        [this](const uptr<Wallpaper>& wallpaper) {
            previewManager->createAndSavePreviews(*wallpaper);
        }
    );
}

void WallpapersModel::addWallpapers(const QStringList& paths, const QString& destinationDirPath) {
    std::vector<fs::path> stdPaths;
    for(const auto& qStringPath : paths) {
        stdPaths.emplace_back(qStringPath.toStdString());
    }

    wallpaperLoader->addWallpapers(stdPaths, destinationDirPath.toStdString());

    loadWallpapers();
}

void WallpapersModel::addWallpapers() {
    std::unordered_set<std::string_view> allSupportedFormats;
    for(const auto& loader : wallpaperLoader->getWallpaperLoaders() | std::views::values) {
        const auto loaderFormats = loader->getSupportedFormats();
        allSupportedFormats.insert(loaderFormats.begin(), loaderFormats.end());
    }

    QFileDialog fileSelector;
    fileSelector.setFileMode(QFileDialog::ExistingFiles);
    fileSelector.setNameFilters(
        {
            QString("Supported Files (%1)").arg(util::getFormatString(allSupportedFormats)),
            QString("Any Files (*)")
        }
    );

    if(!fileSelector.exec()) {
        return;
    }

    const auto selectedFiles = fileSelector.selectedFiles();
    QThreadPool::globalInstance()->start(
        [this, selectedFiles] {
            addWallpapers(selectedFiles, QString::fromStdString(config->getWallpapersDirPath().string()));
        }
    );
}

void WallpapersModel::addWallpapers(const QStringList& paths) {
    QThreadPool::globalInstance()->start(
        [this, paths] {
            addWallpapers(paths, QString::fromStdString(config->getWallpapersDirPath().string()));
        }
    );
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [this, wallpaperId] {
            if(wallpaperRepository->apply(wallpaperId.toStdString())) {
                logger->logInfo("Wallpaper \"" + wallpaperId.toStdString() + "\" applied");
            } else {
                logger->logWarning("Failed to apply wallpaper \"" + wallpaperId.toStdString() + "\"");
            }
        }
    );
}

void WallpapersModel::deleteWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [this, wallpaperId] {
            if(wallpaperRepository->remove(wallpaperId.toStdString())) {
                logger->logInfo("Wallpaper \"" + wallpaperId.toStdString() + "\" deleted");
            } else {
                logger->logWarning("Failed to delete wallpaper \"" + wallpaperId.toStdString() + "\"");
            }
        }
    );
}

void WallpapersModel::refreshWallpapers() {
    loadWallpapers();
}

int WallpapersModel::rowCount(const QModelIndex& parent) const {
    return wallpaperRepository->count();
}

QVariant WallpapersModel::data(const QModelIndex& index, const int role) const {
    const Wallpaper* wallpaper = wallpaperRepository->get(index.row());

    QStringList qStringTags;
    qStringTags.reserve(wallpaper->getTags().size());
    for(const auto& tag : wallpaper->getTags()) {
        qStringTags.append(QString::fromStdString(tag));
    }

    switch(role) {
        case IdRole: return QString::fromStdString(wallpaper->getId());
        case NameRole: return QString::fromStdString(wallpaper->getName());
        case RootDirRole: return QString::fromStdString(wallpaper->getFilePath().parent_path().string());
        case ResolutionRole: return QString::fromStdString(util::toString(wallpaper->getResolution()));
        case SourceRole: return QString::fromStdString(wallpaper->getSource());
        case TagsRole: return qStringTags;
        case IsBadRole: return config->isWallpaperBad(*wallpaper);
        default: return {};
    }
}

QHash<int, QByteArray> WallpapersModel::roleNames() const {
    return {
        {IdRole, "wallpaperId"},
        {NameRole, "wallpaperName"},
        {RootDirRole, "wallpaperRootDir"},
        {ResolutionRole, "wallpaperResolution"},
        {SourceRole, "wallpaperSource"},
        {TagsRole, "wallpaperTags"},
        {IsBadRole, "isWallpaperBad"},
    };
}
