// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QImage>
#include <QtConcurrent>
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "model/StatusModel.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"

namespace {
    void createAndSavePreview(const uptr<Wallpaper>& wallpaper) {
        for(const auto screen : QGuiApplication::screens()) {
            const auto previewPath = util::getPreviewsPath(screen) + '/' + wallpaper->getId() + ".webp";
            const auto screenAspectRatio = screen->size() / std::gcd(screen->size().width(), screen->size().height());
            const auto previewSize = screenAspectRatio * 20 * screen->devicePixelRatio();

            if(QFile previewFile(previewPath); previewFile.exists()) continue;

            if(dynamic_cast<PictureWallpaper*>(wallpaper.get())) {
                const auto preview = QImage(wallpaper->getFilePath()).scaled(
                    previewSize,
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation
                );

                if(!preview.save(previewPath, "WEBP", 100)) {
                    util::logWarn("Unable to save preview file \"{}\"", previewPath.toStdString());
                    util::sendStatus("Unable to save preview file \"{}\"", previewPath.toStdString());
                } else {
                    util::logInfo(
                        "Preview of \"{}\" saved for {}",
                        wallpaper->getId().toStdString(),
                        util::toString(screen->size() * screen->devicePixelRatio()).toStdString()
                    );
                }
            } else if(dynamic_cast<VideoWallpaper*>(wallpaper.get())) {
                //TODO Use libswscale here
                QProcess ffmpeg;

                ffmpeg.start(
                    "ffmpeg",
                    {
                        "-y",
                        "-i", wallpaper->getFilePath(),
                        "-t", "5",
                        "-vf", "fps=24,scale=" + QString::number(previewSize.width()) + ':' + QString::number(previewSize.height()) + ":force_original_aspect_ratio=decrease",
                        "-loop", "0",
                        "-lossless", "1",
                        "-compression_level", "6",
                        previewPath
                    }
                );

                ffmpeg.waitForFinished(-1);

                if(ffmpeg.exitCode() != 0) {
                    util::logWarn("Unable to save preview file \"{}\"", previewPath.toStdString());
                    util::sendStatus("Unable to save preview file \"{}\"", previewPath.toStdString());
                } else {
                    util::logInfo(
                        "Preview of \"{}\" saved for {}",
                        wallpaper->getId().toStdString(),
                        util::toString(screen->size() * screen->devicePixelRatio()).toStdString()
                    );
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
    WallpaperLoader::loadWallpapers();
    endResetModel();

    for(const auto screen : QGuiApplication::screens()) {
        util::createDirIfNotExists(util::getPreviewsPath(screen));
    }

    QtConcurrent::map(Wallpapers::inst(), createAndSavePreview);
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start(
        [=] {
            if(Wallpapers::inst().apply(wallpaperId)) {
                util::logInfo("Wallpaper \"{}\" applied", wallpaperId.toStdString());
                util::sendStatus("Wallpaper \"{}\" applied", wallpaperId.toStdString());
            } else {
                util::logInfo("Failed to apply wallpaper \"{}\"", wallpaperId.toStdString());
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
        case ResolutionRole: return util::toString(wallpaper->getResolution());
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
