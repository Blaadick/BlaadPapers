// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QtConcurrentMap>
#include <QThreadPool>
#include <QFileDialog>
#include "Config.hpp"
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "model/StatusModel.hpp"
#include "preview/PreviewManager.hpp"
#include "util/Loggers.hpp"
#include "util/FormatUtils.hpp"

namespace fs = std::filesystem;

WallpapersModel& WallpapersModel::inst() {
    static WallpapersModel instance;
    return instance;
}

void WallpapersModel::loadWallpapers() {
    beginResetModel();
    WallpaperLoader::loadWallpapers();
    Wallpapers::inst().sortByName();
    endResetModel();

    QtConcurrent::map(Wallpapers::inst(), PreviewManager::createAndSavePreviews);
}

void WallpapersModel::addWallpapers(const QStringList& paths, const QString& destinationDirPath) {
    std::vector<fs::path> stdPaths;
    for(const auto& qStringPath : paths) {
        stdPaths.emplace_back(qStringPath.toStdString());
    }

    beginResetModel();
    WallpaperLoader::addWallpapers(stdPaths, destinationDirPath.toStdString());
    Wallpapers::inst().sortByName();
    endResetModel();

    QtConcurrent::map(Wallpapers::inst(), PreviewManager::createAndSavePreviews);
}

void WallpapersModel::addWallpapers() {
    const auto supportedPictureFormatsString = util::getFormatString(PictureWallpaper::supportedFormats);
    const auto supportedVideoFormatsString = util::getFormatString(VideoWallpaper::supportedFormats);

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
        util::logWarn("Failed to take files from file dialog");
        return;
    }

    const auto selectedFiles = fileSelector.selectedFiles();
    QThreadPool::globalInstance()->start(
        [this, selectedFiles] {
            addWallpapers(selectedFiles, QString::fromStdString(Config::getWallpaperDirPaths()[0]));
        }
    );
}

void WallpapersModel::addWallpapers(const QStringList& paths) {
    QThreadPool::globalInstance()->start(
        [this, paths] {
            addWallpapers(paths, QString::fromStdString(Config::getWallpaperDirPaths()[0]));
        }
    );
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [wallpaperId] {
            if(Wallpapers::inst().apply(wallpaperId.toStdString())) {
                util::logInfo("Wallpaper \"{}\" applied", wallpaperId.toStdString());
                util::sendStatus("Wallpaper \"{}\" applied", wallpaperId.toStdString());
            } else {
                util::logWarn("Failed to apply wallpaper \"{}\"", wallpaperId.toStdString());
                util::sendStatus("Failed to apply wallpaper \"{}\"", wallpaperId.toStdString());
            }
        }
    );
}

void WallpapersModel::deleteWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [wallpaperId] {
            if(Wallpapers::inst().remove(wallpaperId.toStdString())) {
                util::logInfo("Wallpaper \"{}\" deleted", wallpaperId.toStdString());
                util::sendStatus("Wallpaper \"{}\" deleted", wallpaperId.toStdString());
            } else {
                util::logWarn("Failed to delete wallpaper \"{}\"", wallpaperId.toStdString());
                util::sendStatus("Failed to delete wallpaper \"{}\"", wallpaperId.toStdString());
            }
        }
    );
}

void WallpapersModel::refreshWallpapers() {
    loadWallpapers();
}

int WallpapersModel::rowCount(const QModelIndex& parent) const {
    return Wallpapers::inst().count();
}

QVariant WallpapersModel::data(const QModelIndex& index, const int role) const {
    const Wallpaper* wallpaper = Wallpapers::inst().get(index.row());

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
        case IsBadRole: return wallpaper->isBad();
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
