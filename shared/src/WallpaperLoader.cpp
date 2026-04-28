// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "WallpaperLoader.hpp"

#include <iostream>
#include <QDirIterator>
#include <QJsonArray>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "util/Ffmpeg.hpp"
#include "util/FormatUtils.hpp"
#include "util/PathUtils.hpp"
#include "util/Vips.hpp"

void WallpaperLoader::loadWallpapers() {
    Wallpapers::inst().clear();
    jpegUnifier();

    vips_init("");

    for(const QString& wallpaperDirPath : Config::getWallpaperDirPaths()) {
        if(!util::createDirIfNotExists(wallpaperDirPath + "/.index")) {
            continue;
        }

        QDirIterator pictureIterator(
            wallpaperDirPath,
            util::getFileMask(util::supportedPictureFormats),
            QDir::Files,
            QDirIterator::Subdirectories
        );
        while(pictureIterator.hasNext()) {
            pictureIterator.next();

            const auto wallpaperId = pictureIterator.fileInfo().completeBaseName();

            Wallpapers::inst().add(
                std::move(
                    loadPictureWallpaper(
                        pictureIterator.fileInfo(),
                        readWallpaperData(wallpaperDirPath + "/.index/" + wallpaperId + ".json", wallpaperId)
                    )
                )
            );
        }

        QDirIterator videoIterator(
            wallpaperDirPath,
            util::getFileMask(util::supportedVideoFormats),
            QDir::Files,
            QDirIterator::Subdirectories
        );
        while(videoIterator.hasNext()) {
            videoIterator.next();

            const auto wallpaperId = videoIterator.fileInfo().completeBaseName();

            Wallpapers::inst().add(
                std::move(
                    loadVideoWallpaper(
                        videoIterator.fileInfo(),
                        readWallpaperData(wallpaperDirPath + "/.index/" + wallpaperId + ".json", wallpaperId)
                    )
                )
            );
        }

        Wallpapers::inst().sortByName();
    }

    vips_shutdown();
}

QJsonObject WallpaperLoader::readWallpaperData(const QString& filePath, const QString& wallpaperId) {
    const QJsonObject defaultWallpaperData{{"name", wallpaperId}, {"source", ""}, {"tags", QJsonArray{"General"}}};
    QJsonObject wallpaperData;

    if(QFile dataFile(filePath); dataFile.exists()) {
        util::open(dataFile, QIODeviceBase::ReadOnly);
        wallpaperData = QJsonDocument::fromJson(dataFile.readAll()).object();
        dataFile.close();

        auto isFull = true;

        if(wallpaperData["name"].isNull()) {
            wallpaperData["name"] = defaultWallpaperData["name"];
            isFull = false;
        }

        if(wallpaperData["source"].isNull()) {
            wallpaperData["source"] = defaultWallpaperData["source"];
            isFull = false;
        }

        if(wallpaperData["tags"].isNull()) {
            wallpaperData["tags"] = defaultWallpaperData["tags"];
            isFull = false;
        }

        if(!isFull) {
            util::open(dataFile, QIODeviceBase::WriteOnly);
            dataFile.write(QJsonDocument(wallpaperData).toJson());
            dataFile.close();
        }
    } else {
        util::open(dataFile, QIODeviceBase::WriteOnly);
        dataFile.write(QJsonDocument(defaultWallpaperData).toJson());
        dataFile.close();

        wallpaperData = defaultWallpaperData;
    }

    return wallpaperData;
}

// TODO Move to std fs
uptr<PictureWallpaper> WallpaperLoader::loadPictureWallpaper(const QFileInfo& fileInfo, const QJsonObject& data) {
    auto [width, height] = getPictureResolutionData(std::filesystem::path(fileInfo.filePath().toStdString()));
    QVector<QString> tags;

    for(auto tag : data["tags"].toArray()) {
        tags.append(tag.toString());
    }

    return std::make_unique<PictureWallpaper>(
        fileInfo.completeBaseName(),
        fileInfo.absoluteFilePath(),
        data.value("name").toString(),
        QSize(width, height),
        data.value("source").toString(),
        tags
    );
}

// TODO Move to std fs
uptr<VideoWallpaper> WallpaperLoader::loadVideoWallpaper(const QFileInfo& fileInfo, const QJsonObject& data) {
    auto [resolution, frameRate] = getVideoData(std::filesystem::path(fileInfo.filePath().toStdString()));

    QVector<QString> tags;
    for(auto tag : data["tags"].toArray()) {
        tags.append(tag.toString());
    }

    return std::make_unique<VideoWallpaper>(
        fileInfo.completeBaseName(),
        fileInfo.absoluteFilePath(),
        data.value("name").toString(),
        QSize(resolution.width, resolution.height),
        frameRate,
        data.value("source").toString(),
        tags
    );
}

void WallpaperLoader::jpegUnifier() {
    for(const auto& wallpaperDirPath : Config::getWallpaperDirPaths()) {
        QDirIterator dirIterator(wallpaperDirPath, {"*.jpg", "*.jpe", "*.JPG"}, QDir::Files);

        while(dirIterator.hasNext()) {
            auto filePath = dirIterator.next();
            auto newFilePath = filePath;

            newFilePath.chop(3);
            newFilePath.append("jpeg");

            QFile::rename(filePath, newFilePath);
        }
    }
}
