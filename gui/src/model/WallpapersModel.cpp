// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QImage>
#include <QtConcurrent>
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "model/StatusModel.hpp"
#include "preview/PreviewManager.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtilsExtra.hpp"

WallpapersModel& WallpapersModel::inst() {
    static WallpapersModel instance;
    return instance;
}

void WallpapersModel::load() {
    beginResetModel();
    WallpaperLoader::loadWallpapers();
    endResetModel();

    for(const auto screen : QGuiApplication::screens()) {
        util::createDirIfNotExists(util::getPreviewsPath(screen));
    }

    QtConcurrent::map(Wallpapers::inst(), PreviewManager::createAndSavePreview);
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [=] {
            if(Wallpapers::inst().apply(wallpaperId)) {
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
            if(Wallpapers::inst().remove(wallpaperId)) {
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

    switch(role) {
        case IdRole: return wallpaper->getId();
        case NameRole: return wallpaper->getName();
        case ResolutionRole: return QString::fromStdString(util::toString(wallpaper->getResolution()));
        case SourceRole: return wallpaper->getSource();
        case TagsRole: return wallpaper->getTags();
        case IsBadRole: return wallpaper->isBad();
        default: return {};
    }
}

QHash<int, QByteArray> WallpapersModel::roleNames() const {
    return {
        {IdRole, "wallpaperId"},
        {NameRole, "wallpaperName"},
        {ResolutionRole, "wallpaperResolution"},
        {SourceRole, "wallpaperSource"},
        {TagsRole, "wallpaperTags"},
        {IsBadRole, "isWallpaperBad"},
    };
}
