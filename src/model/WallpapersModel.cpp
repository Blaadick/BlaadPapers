// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QImage>
#include <QtConcurrent>
#include "Wallpapers.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"
#include "util/WallpaperUtils.hpp"

namespace {
    QString getScreenPreviewsPath(const QScreen* screen) {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
            + "/preview/"
            + QString::number(screen->geometry().width() * screen->devicePixelRatio())
            + "x"
            + QString::number(screen->geometry().height() * screen->devicePixelRatio())
            + '/';
    }

    void createAndSavePreview(const Wallpaper& wallpaper) {
        for(const auto screen : QGuiApplication::screens()) {
            const QString previewPath = getScreenPreviewsPath(screen) + wallpaper.getId() + ".webp";
            const QSize screenAspectRatio = screen->geometry().size() / std::gcd(screen->geometry().width(), screen->geometry().height());

            if(QFile previewFile(previewPath); !previewFile.exists()) {
                auto preview = QImage(wallpaper.getFilePath()).scaled(
                    screenAspectRatio * 20 * screen->devicePixelRatio(),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation
                );

                if(!preview.save(previewPath, "WEBP", 100)) {
                    util::logError("Unable to save preview file \"{}\"", previewPath.toStdString());
                    util::sendStatus("Unable to save preview file \"{}\"", previewPath.toStdString());
                } else {
                    util::logInfo("Preview of {} for {} saved", wallpaper.getId().toStdString(), screen->devicePixelRatio());
                }
            }
        }
    }
}

WallpapersModel& WallpapersModel::inst() {
    static WallpapersModel instance;
    return instance;
}

void WallpapersModel::load() {
    beginResetModel();
    Wallpapers::load();
    endResetModel();

    for(const auto screen : QGuiApplication::screens()) {
        util::createDirIfNotExists(getScreenPreviewsPath(screen));
    }

    QtConcurrent::map(Wallpapers::getWallpapers(), createAndSavePreview);

    util::logInfo("Loaded {} wallpapers", Wallpapers::getWallpapers().count());
    util::sendStatus("Loaded {} wallpapers", Wallpapers::getWallpapers().count());
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start([=] {
        util::applyWallpaper(wallpaperId);
    });
}

void WallpapersModel::deleteWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start([=] {
        //TODO Fix model updating and fast deleting crash
        util::deleteWallpaper(wallpaperId);
    });
}

int WallpapersModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(Wallpapers::getWallpapers().count());
}

QVariant WallpapersModel::data(const QModelIndex& index, const int role) const {
    const Wallpaper& wallpaper = Wallpapers::getWallpapers().at(index.row());

    switch(role) {
        case IdRole: return wallpaper.getId();
        case NameRole: return wallpaper.getName();
        case TagsRole: return wallpaper.getTags();
        case IsAnimatedRole: return wallpaper.isAnimated();
        case IsBadRole: return wallpaper.isBad();
        default: return {};
    }
}

QHash<int, QByteArray> WallpapersModel::roleNames() const {
    return {
        {IdRole, "wallpaperId"},
        {NameRole, "wallpaperName"},
        {TagsRole, "wallpaperTags"},
        {IsAnimatedRole, "isWallpaperAnimated"},
        {IsBadRole, "isWallpaperBad"},
    };
}
