// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QFileDialog>
#include <QtConcurrentMap>
#include <QThreadPool>
#include "Config.hpp"
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "util/FormatUtils.hpp"
#include "util/WallpaperUtils.hpp"

namespace fs = std::filesystem;

WallpapersModel::WallpapersModel(
    sptr<WallpaperLoader> wallpaperLoader,
    sptr<Wallpapers> wallpapers,
    sptr<Config> config,
    sptr<PreviewManager> previewManager,
    sptr<util::Logger> logger
) : wallpaperLoader(std::move(wallpaperLoader)), wallpapers(std::move(wallpapers)), config(std::move(config)), previewManager(std::move(previewManager)), logger(std::move(logger)) {}

void WallpapersModel::loadWallpapers() {
    beginResetModel();
    wallpaperLoader->loadWallpapers();
    wallpapers->sortByName();
    endResetModel();

    logger->logInfo("Loaded " + std::to_string(wallpapers->count()) + " wallpapers");

    QtConcurrent::map(
        *wallpapers,
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

    beginResetModel();
    wallpaperLoader->addWallpapers(stdPaths, destinationDirPath.toStdString());
    wallpapers->sortByName();
    endResetModel();

    QtConcurrent::map(
        *wallpapers,
        [this](const uptr<Wallpaper>& wallpaper) {
            previewManager->createAndSavePreviews(*wallpaper);
        }
    );
}

void WallpapersModel::addWallpapers() {
    const auto supportedPictureFormatsString = util::getFormatString(util::supportedPictureFormats);
    const auto supportedVideoFormatsString = util::getFormatString(util::supportedVideoFormats);

    QFileDialog fileSelector;
    fileSelector.setFileMode(QFileDialog::ExistingFiles);
    fileSelector.setNameFilters(
        {
            "Any Supported (" + supportedPictureFormatsString + ' ' + supportedVideoFormatsString + ')',
            "Picture (" + supportedPictureFormatsString + ')',
            "Video (" + supportedVideoFormatsString + ')'
        }
    );

    if(!fileSelector.exec()) {
        return;
    }

    const auto selectedFiles = fileSelector.selectedFiles();
    QThreadPool::globalInstance()->start(
        [this, selectedFiles] {
            addWallpapers(selectedFiles, QString::fromStdString(config->getWallpaperDirPaths()[0]));
        }
    );
}

void WallpapersModel::addWallpapers(const QStringList& paths) {
    QThreadPool::globalInstance()->start(
        [this, paths] {
            addWallpapers(paths, QString::fromStdString(config->getWallpaperDirPaths()[0]));
        }
    );
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [this, wallpaperId] {
            if(wallpapers->apply(wallpaperId.toStdString())) {
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
            if(wallpapers->remove(wallpaperId.toStdString())) {
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
    return wallpapers->count();
}

QVariant WallpapersModel::data(const QModelIndex& index, const int role) const {
    const Wallpaper* wallpaper = wallpapers->get(index.row());

    QStringList qStringTags;
    qStringTags.reserve(wallpaper->getTags().size());
    for(const auto& tag : wallpaper->getTags()) {
        qStringTags.append(QString::fromStdString(tag));
    }

    switch(role) {
        case IdRole: return QString::fromStdString(wallpaper->getId());
        case NameRole: return QString::fromStdString(wallpaper->getName());
        case RootDirRole: return QString::fromStdString(wallpaper->getFilePath().parent_path());
        case ResolutionRole: return QString::fromStdString(wallpaper->getResolution().toString());
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
