// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpapers.hpp"

#include <QDirIterator>
#include <QFile>
#include <QImageReader>
#include <QJsonDocument>
#include <util/PathUtils.hpp>
#include "Config.hpp"
#include "Tags.hpp"
#include "util/FormatUtils.hpp"
#include "util/Loggers.hpp"
#include "util/ffmpeg.hpp"

namespace {
    QJsonObject readWallpaperData(const QString& wallpaperId) {
        const QString wallpaperDataPath = Config::getWallpapersDirPath() + ".index/" + wallpaperId + ".json";
        const QJsonObject defaultWallpaperData{
            {"name", wallpaperId},
            {"source", ""},
            {"tags", QJsonArray{"General"}}
        };
        QJsonObject wallpaperData;

        if(QFile dataFile(wallpaperDataPath); dataFile.exists()) {
            dataFile.open(QIODeviceBase::ReadOnly);
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
                dataFile.open(QIODeviceBase::WriteOnly);
                dataFile.write(QJsonDocument(wallpaperData).toJson());
                dataFile.close();
            }
        } else {
            dataFile.open(QIODeviceBase::WriteOnly);
            dataFile.write(QJsonDocument(defaultWallpaperData).toJson());
            dataFile.close();

            wallpaperData = defaultWallpaperData;
        }

        return wallpaperData;
    }
}

void Wallpapers::load() {
    wallpapers.clear();
    util::createDirIfNotExists(Config::getWallpapersDirPath() + ".index/");
    util::jpegUnifier();

    QDirIterator dirPictureIterator(
        Config::getWallpapersDirPath(),
        util::getFileMask(util::supportedPictureFormats),
        QDir::NoFilter
    );

    QDirIterator dirVideoIterator(
        Config::getWallpapersDirPath(),
        util::getFileMask(util::supportedVideoFormats),
        QDir::Files
    );

    QDirIterator dirSceneIterator(
        Config::getWallpapersDirPath(),
        util::getFileMask(util::supportedSceneFormats),
        QDir::Files
    );

    while(dirPictureIterator.hasNext()) {
        dirPictureIterator.next();
        auto wallpaperId = dirPictureIterator.fileInfo().baseName();

        QImageReader imageReader(dirPictureIterator.filePath());

        wallpapers.append(Wallpaper(
            wallpaperId,
            dirPictureIterator.filePath(),
            imageReader.size(),
            WallpaperType::Picture,
            readWallpaperData(wallpaperId)
        ));
    }

    while(dirVideoIterator.hasNext()) {
        dirVideoIterator.next();
        auto wallpaperId = dirVideoIterator.fileInfo().baseName();

        wallpapers.append(Wallpaper(
            wallpaperId,
            dirVideoIterator.filePath(),
            getVideoResolution(dirVideoIterator.filePath()),
            WallpaperType::Video,
            readWallpaperData(wallpaperId)
        ));
    }

    while(dirSceneIterator.hasNext()) {
        dirSceneIterator.next();
        auto wallpaperId = dirSceneIterator.fileInfo().baseName();

        wallpapers.append(Wallpaper(
            wallpaperId,
            dirSceneIterator.filePath(),
            QSize(),
            WallpaperType::Scene,
            readWallpaperData(wallpaperId)
        ));
    }

    std::ranges::sort(wallpapers, [](const Wallpaper& w1, const Wallpaper& w2) {
        return w1.getName() < w2.getName();
    });

    for(const auto& wallpaper : wallpapers) {
        for(const auto& tag : wallpaper.getTags()) {
            Tags::addTag(tag);
        }
    }
}

const QVector<Wallpaper>& Wallpapers::getWallpapers() {
    return wallpapers;
}

std::optional<std::reference_wrapper<const Wallpaper>> Wallpapers::getWallpaper(const QString& wallpaperId) {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper.getId() == wallpaperId) {
            return wallpaper;
        }
    }

    return std::nullopt;
}

int Wallpapers::count() {
    return wallpapers.count();
}

void Wallpapers::deleteWallpaper(const Wallpaper& wallpaper) {
    QFile(wallpaper.getFilePath()).remove();
    QFile(Config::getWallpapersDirPath() + ".index/" + wallpaper.getId() + ".json").remove();

    QDirIterator dirIterator(
        util::getPreviewsPath(),
        {wallpaper.getId() + ".webp"},
        QDir::Files,
        QDirIterator::Subdirectories
    );

    while(dirIterator.hasNext()) {
        dirIterator.next();
        QFile(dirIterator.filePath()).remove();
    }

    for(const auto& tag : wallpaper.getTags()) {
        Tags::removeTag(tag);
    }

    wallpapers.removeOne(wallpaper);

    util::logInfo("Wallpaper {} deleted", wallpaper.getId().toStdString());
    util::sendStatus("Wallpaper {} deleted", wallpaper.getId().toStdString());
}

void Wallpapers::deleteWallpaper(const QString& wallpaperId) {
    const auto wallpaper = getWallpaper(wallpaperId);

    if(!wallpaper) {
        util::logError("Wallpaper {} not found", wallpaperId.toStdString());
        util::sendStatus("Wallpaper {} not found", wallpaperId.toStdString());
        return;
    }

    deleteWallpaper(*wallpaper);
}

void Wallpapers::applyWallpaper(const Wallpaper& wallpaper) {
    //TODO Remove when implement own renderer

    const auto de = qgetenv("XDG_SESSION_DESKTOP").toStdString();
    if(de == "Hyprland") {
        QFile hyprpaperConfig(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/hypr/hyprpaper.conf");

        system("hyprctl -q hyprpaper unload all");
        system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + '\"').toStdString().c_str());
        system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + '\"').toStdString().c_str());

        hyprpaperConfig.open(QIODevice::WriteOnly);
        hyprpaperConfig.write(("preload = " + wallpaper.getFilePath() + '\n').toStdString().c_str());
        hyprpaperConfig.write(("wallpaper = , " + wallpaper.getFilePath()).toStdString().c_str());
        hyprpaperConfig.close();
    } else if(de == "KDE") {
        system(("plasma-apply-wallpaperimage \"" + wallpaper.getFilePath() + '\"').toStdString().c_str());
    } else if(de == "gnome" || de == "ubuntu") {
        system(("gsettings set org.gnome.desktop.background picture-uri \"" + wallpaper.getFilePath() + '\"').toStdString().c_str());
        system(("gsettings set org.gnome.desktop.background picture-uri-dark \"" + wallpaper.getFilePath() + '\"').toStdString().c_str());
    } else {
        util::logError("Desktop environment \"{}\" unsupported", de);
        util::sendStatus("Desktop environment \"{}\" unsupported", de);
        return;
    }

    util::logInfo("Wallpaper {} set", wallpaper.getId().toStdString());
    util::sendStatus("Wallpaper {} set", wallpaper.getId().toStdString());

    system(("bash " + Config::getPostSetScriptFilePath() + " \"" + wallpaper.getName() + '\"' + " \"" + wallpaper.getFilePath() + '\"').toStdString().c_str());
}

void Wallpapers::applyWallpaper(const QString& wallpaperId) {
    const auto wallpaper = getWallpaper(wallpaperId);

    if(!wallpaper) {
        util::logError("Wallpaper {} not found", wallpaperId.toStdString());
        util::sendStatus("Wallpaper {} not found", wallpaperId.toStdString());
        return;
    }

    applyWallpaper(*wallpaper);
}

QJsonArray Wallpapers::toJson() {
    QJsonArray wallpapersData;

    for(const auto& wallpaper : wallpapers) {
        wallpapersData.append(wallpaper.toJson());
    }

    return wallpapersData;
}

QVector<Wallpaper> Wallpapers::wallpapers;
