// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QtConcurrent>
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "model/StatusModel.hpp"
#include "preview/PreviewManager.hpp"
#include "util/Loggers.hpp"

namespace fs = std::filesystem;

WallpapersModel& WallpapersModel::inst() {
    static WallpapersModel instance;
    return instance;
}

void WallpapersModel::load() {
    beginResetModel();
    WallpaperLoader::loadWallpapers();
    endResetModel();

    QtConcurrent::map(Wallpapers::inst(), PreviewManager::createAndSavePreview);
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [=] {
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
        [=] {
            //TODO Fix model updating and fast deleting crash
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
