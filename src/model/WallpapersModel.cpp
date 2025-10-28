// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/WallpapersModel.hpp"

#include <QImage>
#include <QtConcurrent>
#include "Wallpapers.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"

namespace {
    void createAndSavePreview(const Wallpaper& wallpaper) {
        for(const auto screen : QGuiApplication::screens()) {
            const auto previewPath = util::getScreenPreviewsPath(screen) + wallpaper.getId() + ".webp";
            const auto screenAspectRatio = screen->size() / std::gcd(screen->size().width(), screen->size().height());
            const auto previewSize = screenAspectRatio * 20 * screen->devicePixelRatio();

            if(QFile previewFile(previewPath); previewFile.exists()) continue;

            switch(wallpaper.getType()) {
                case WallpaperType::Picture: {
                    const auto preview = QImage(wallpaper.getFilePath()).scaled(
                        previewSize,
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation
                    );

                    if(!preview.save(previewPath, "WEBP", 100)) {
                        util::logError("Unable to save preview file \"{}\"", previewPath.toStdString());
                        util::sendStatus("Unable to save preview file \"{}\"", previewPath.toStdString());
                    } else {
                        util::logInfo(
                            "Preview of {} for {} saved",
                            wallpaper.getId().toStdString(),
                            toString(screen->size() * screen->devicePixelRatio()).toStdString()
                        );
                    }

                    break;
                }

                case WallpaperType::Video: {
                    //TODO Use libswscale here
                    QProcess ffmpeg;

                    ffmpeg.start("ffmpeg", {
                        "-y",
                        "-i", wallpaper.getFilePath(),
                        "-t", "5",
                        "-vf", "fps=24,scale=" + QString::number(previewSize.width()) + ':' + QString::number(previewSize.height()) + ":force_original_aspect_ratio=decrease",
                        "-loop", "0",
                        "-lossless", "1",
                        "-compression_level", "6",
                        previewPath
                    });

                    ffmpeg.waitForFinished(-1);

                    if(ffmpeg.exitCode() != 0) {
                        util::logError("Unable to save preview file \"{}\"", previewPath.toStdString());
                        util::sendStatus("Unable to save preview file \"{}\"", previewPath.toStdString());
                    } else {
                        util::logInfo(
                            "Preview of {} for {} saved",
                            wallpaper.getId().toStdString(),
                            toString(screen->size() * screen->devicePixelRatio()).toStdString()
                        );
                    }

                    break;
                }

                case WallpaperType::Scene: {
                    break;
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
        util::createDirIfNotExists(util::getScreenPreviewsPath(screen));
    }

    QtConcurrent::map(Wallpapers::getWallpapers(), createAndSavePreview);
}

void WallpapersModel::applyWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start([=] {
        Wallpapers::applyWallpaper(wallpaperId);
    });
}

void WallpapersModel::deleteWallpaper(const QString& wallpaperId) const {
    QThreadPool::globalInstance()->start([=] {
        //TODO Fix model updating and fast deleting crash
        Wallpapers::deleteWallpaper(wallpaperId);
    });
}

int WallpapersModel::rowCount(const QModelIndex& parent) const {
    return Wallpapers::count();
}

QVariant WallpapersModel::data(const QModelIndex& index, const int role) const {
    const Wallpaper& wallpaper = Wallpapers::getWallpapers().at(index.row());

    switch(role) {
        case IdRole: return wallpaper.getId();
        case NameRole: return wallpaper.getName();
        case ResolutionRole: return toString(wallpaper.getResolution());
        case SourceRole: return wallpaper.getSource();
        case TagsRole: return wallpaper.getTags();
        case TypeRole: return toString(wallpaper.getType());
        case IsBadRole: return wallpaper.isBad();
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
        {TypeRole, "wallpaperType"},
        {IsBadRole, "isWallpaperBad"},
    };
}
