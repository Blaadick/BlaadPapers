#include "Wallpapers.hpp"

#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QStandardPaths>

void Wallpapers::load() {
    const QString wallpapersPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/Wallpapers/";
    const QString wallpapersDataPath = wallpapersPath + ".index/";
    QDirIterator dirIterator(wallpapersPath, {"*.png", "*.jpg"});

    if(QDir wallpapersDataDir(wallpapersDataPath); !wallpapersDataDir.exists()) {
        wallpapersDataDir.mkpath(wallpapersDataPath);
    }

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
            wallpaperTags.emplace_back(tag.toString());
        }

        wallpapers.append(
            Wallpaper(
                wallpaperName,
                wallpaperData["description"].toString(),
                dirIterator.filePath(),
                wallpaperTags
            )
        );
    }

    std::ranges::sort(wallpapers, [](const Wallpaper& w1, const Wallpaper& w2) {
        return w1.getName() < w2.getName();
    });
}

QVector<Wallpaper> Wallpapers::getWallpapers() {
    return wallpapers;
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
