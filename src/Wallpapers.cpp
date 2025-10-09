// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpapers.hpp"

#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <util/PathUtils.hpp>
#include "Config.hpp"
#include "util/FormatUtils.hpp"
#include "util/Loggers.hpp"

namespace {
    QJsonObject readWallpaperData(const QString& wallpaperId) {
        const QString wallpaperDataPath = Config::getWallpapersDirPath() + ".index/" + wallpaperId + ".json";
        const QJsonObject defaultWallpaperData{
            {"name", wallpaperId},
            {"tags", QJsonArray{"General"}}
        };
        QJsonObject wallpaperData;

        if(QFile dataFile(wallpaperDataPath); dataFile.exists()) {
            dataFile.open(QIODeviceBase::ReadOnly);
            wallpaperData = QJsonDocument::fromJson(dataFile.readAll()).object();
            dataFile.close();

            auto notFull = false;

            if(wallpaperData["name"].isNull()) {
                wallpaperData["name"] = defaultWallpaperData["name"];
                notFull = true;
            }

            if(wallpaperData["tags"].isNull()) {
                wallpaperData["tags"] = defaultWallpaperData["tags"];
                notFull = true;
            }

            if(notFull) {
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

    /**
     * We are not in DOS time! I turn it off if it is too annoying.
     */
    void shaitanMachine() {
        QDirIterator dirIterator(Config::getWallpapersDirPath(), {"*.jpg", "*.jpe", "*.JPG"});

        while(dirIterator.hasNext()) {
            auto file = dirIterator.next();
            auto newFile = file;

            newFile.chop(4);
            newFile.append(".jpeg");

            QFile::rename(file, newFile);
        }
    }
}

void Wallpapers::load() {
    const QString wallpapersDataPath = Config::getWallpapersDirPath() + ".index/";
    util::createDirIfNotExists(wallpapersDataPath);

    wallpapers.clear();
    shaitanMachine();

    QDirIterator dirIterator(Config::getWallpapersDirPath(), util::getFileMask(util::supportedStaticFormats));
    while(dirIterator.hasNext()) {
        dirIterator.next();

        auto wallpaperId = dirIterator.fileInfo().baseName();
        auto wallpaperData = readWallpaperData(wallpaperId);

        QVector<QString> wallpaperTags;
        for(auto tag : wallpaperData["tags"].toArray()) {
            wallpaperTags.append(tag.toString());
        }

        wallpapers.append(
            Wallpaper(
                wallpaperId,
                dirIterator.fileInfo().suffix(),
                wallpaperData["name"].toString(),
                wallpaperTags
            )
        );

        for(const auto& tag : wallpaperTags) {
            if(!uniqueTags.contains(tag)) {
                uniqueTags.insert(tag, 1);
            } else {
                uniqueTags[tag] += 1;
            }
        }
    }

    std::ranges::sort(wallpapers, [](const Wallpaper& w1, const Wallpaper& w2) {
        return w1.getName() < w2.getName();
    });
}

const QVector<Wallpaper>& Wallpapers::getWallpapers() {
    return wallpapers;
}

const Wallpaper* Wallpapers::getWallpaper(const QString& id) {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper.getId() == id) {
            return &wallpaper;
        }
    }

    return nullptr;
}

void Wallpapers::deleteWallpaper(const Wallpaper& wallpaper) {
    auto wallpaperFile = QFile(wallpaper.getFilePath());
    auto wallpaperDataFile = QFile(Config::getWallpapersDirPath() + ".index/" + wallpaper.getId() + ".json");

    //TODO Remove cached previews
    wallpaperFile.remove();
    wallpaperDataFile.remove();
    wallpapers.removeOne(wallpaper);
}

const QMap<QString, int>& Wallpapers::getUniqueTags() {
    return uniqueTags;
}

QJsonArray Wallpapers::toJson() {
    QJsonArray wallpapersData;

    for(const auto& wallpaper : wallpapers) {
        wallpapersData.append(wallpaper.toJson());
    }

    return wallpapersData;
}

QVector<Wallpaper> Wallpapers::wallpapers;
QMap<QString, int> Wallpapers::uniqueTags;
