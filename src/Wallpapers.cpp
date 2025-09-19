#include "Wallpapers.hpp"

#include <iostream>
#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QStandardPaths>
#include "Config.hpp"

void Wallpapers::load() {
    const QString wallpapersDataPath = Config::getWorkingPath() + ".index/";
    QDirIterator dirIterator(Config::getWorkingPath(), {"*.png", "*.jpg"});

    if(QDir wallpapersDataDir(wallpapersDataPath); !wallpapersDataDir.exists()) {
        wallpapersDataDir.mkpath(wallpapersDataPath);
    }

    wallpapers.clear();

    while(dirIterator.hasNext()) {
        dirIterator.next();

        QString wallpaperName = dirIterator.fileInfo().baseName();
        QJsonObject wallpaperData;

        if(QFile dataFile(wallpapersDataPath + wallpaperName + ".json"); dataFile.exists()) {
            dataFile.open(QIODeviceBase::ReadOnly);
            wallpaperData = QJsonDocument::fromJson(dataFile.readAll()).object();
            dataFile.close();
        } else {
            dataFile.open(QIODeviceBase::WriteOnly);
            dataFile.write(QJsonDocument(defaultWallpaperData).toJson());
            dataFile.close();

            wallpaperData = defaultWallpaperData;
        }

        QVector<QString> wallpaperTags{};

        for(auto tag : wallpaperData["tags"].toArray()) {
            wallpaperTags.append(tag.toString());
        }

        wallpapers.append(
            Wallpaper(
                wallpaperName,
                wallpaperData["description"].toString(),
                dirIterator.filePath(),
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

const Wallpaper* Wallpapers::getWallpaper(const QString& name) {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper.getName() == name) {
            return &wallpaper;
        }
    }

    return nullptr;
}

const QMap<QString, int>& Wallpapers::getUniqueTags() {
    return uniqueTags;
}

QJsonArray Wallpapers::toJson() {
    QJsonArray wallpapersData;

    for(const auto& wallpaper : wallpapers) {
        auto wallpaperData = wallpaper.toJson();

        wallpaperData["name"] = wallpaper.getName();
        wallpapersData.append(wallpaperData);
    }

    return wallpapersData;
}

QVector<Wallpaper> Wallpapers::wallpapers;
QMap<QString, int> Wallpapers::uniqueTags;
