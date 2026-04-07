// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpapers.hpp"
//
// #ifdef Q_OS_LINUX
//     #include "unistd.h"
//     #include "sys/socket.h"
//     #include "sys/un.h"
// #endif
//
// #include <QImageReader>
// #include <QJsonDocument>
// #include <util/PathUtils.hpp>
// #include "Config.hpp"
// #include "data/PictureWallpaper.hpp"
// #include "util/Ffmpeg.hpp"
// #include "util/FormatUtils.hpp"
// #include "util/Loggers.hpp"
//
// namespace {
//     QJsonObject readWallpaperData(const QString& wallpaperId) {
//         const QString wallpaperDataPath = Config::getWallpapersDirPath() + ".index/" + wallpaperId + ".json";
//         const QJsonObject defaultWallpaperData{{"name", wallpaperId}, {"source", ""}, {"tags", QJsonArray{"General"}}};
//         QJsonObject wallpaperData;
//
//         if(QFile dataFile(wallpaperDataPath); dataFile.exists()) {
//             dataFile.open(QIODeviceBase::ReadOnly);
//             wallpaperData = QJsonDocument::fromJson(dataFile.readAll()).object();
//             dataFile.close();
//
//             auto isFull = true;
//
//             if(wallpaperData["name"].isNull()) {
//                 wallpaperData["name"] = defaultWallpaperData["name"];
//                 isFull = false;
//             }
//
//             if(wallpaperData["source"].isNull()) {
//                 wallpaperData["source"] = defaultWallpaperData["source"];
//                 isFull = false;
//             }
//
//             if(wallpaperData["tags"].isNull()) {
//                 wallpaperData["tags"] = defaultWallpaperData["tags"];
//                 isFull = false;
//             }
//
//             if(!isFull) {
//                 dataFile.open(QIODeviceBase::WriteOnly);
//                 dataFile.write(QJsonDocument(wallpaperData).toJson());
//                 dataFile.close();
//             }
//         } else {
//             dataFile.open(QIODeviceBase::WriteOnly);
//             dataFile.write(QJsonDocument(defaultWallpaperData).toJson());
//             dataFile.close();
//
//             wallpaperData = defaultWallpaperData;
//         }
//
//         return wallpaperData;
//     }
// }
//
// void Wallpapers::load() {
//     wallpapers.clear();
//     util::createDirIfNotExists(Config::getWallpaperDirPaths() + ".index/");
//     util::jpegUnifier();
//
//     QDirIterator dirPictureIterator(
//         Config::getWallpaperDirPaths(),
//         util::getFileMask(util::supportedPictureFormats),
//         QDir::Files
//     );
//
//     QDirIterator dirVideoIterator(
//         Config::getWallpaperDirPaths(),
//         util::getFileMask(util::supportedVideoFormats),
//         QDir::Files
//     );
//
//     while(dirPictureIterator.hasNext()) {
//         dirPictureIterator.next();
//         auto wallpaperId = dirPictureIterator.fileInfo().baseName();
//
//         QImageReader imageReader(dirPictureIterator.filePath());
//
//         wallpapers.append(
//             PictureWallpaper(
//                 wallpaperId,
//                 dirPictureIterator.filePath(),
//                 imageReader.size(),
//                 readWallpaperData(wallpaperId)
//             )
//         );
//     }
//
//     while(dirVideoIterator.hasNext()) {
//         dirVideoIterator.next();
//         auto wallpaperId = dirVideoIterator.fileInfo().baseName();
//
//         wallpapers.append(
//             Wallpaper(
//                 wallpaperId,
//                 dirVideoIterator.filePath(),
//                 getVideoResolution(dirVideoIterator.filePath()),
//                 readWallpaperData(wallpaperId)
//             )
//         );
//     }
//
//     std::ranges::sort(
//         wallpapers,
//         [](const Wallpaper& w1, const Wallpaper& w2) {
//             return w1.getName() < w2.getName();
//         }
//     );
// }
//
// void Wallpapers::deleteWallpaper(const Wallpaper& wallpaper) {
//     QFile(wallpaper.getFilePath()).remove();
//     QFile(Config::getWallpaperDirPaths() + ".index/" + wallpaper.getId() + ".json").remove();
//
//     QDirIterator dirIterator(
//         util::getPreviewsPath(),
//         {wallpaper.getId() + ".webp"},
//         QDir::Files,
//         QDirIterator::Subdirectories
//     );
//
//     while(dirIterator.hasNext()) {
//         dirIterator.next();
//         QFile(dirIterator.filePath()).remove();
//     }
//
//     wallpapers.removeOne(wallpaper);
// }
//
// void Wallpapers::deleteWallpaper(const QString& wallpaperId) {
//     const auto wallpaper = getWallpaper(wallpaperId);
//
//     if(!wallpaper.has_value()) {
//         util::logError("Wallpaper \"{}\" not found", wallpaperId.toStdString());
//         util::sendStatus("Wallpaper \"{}\" not found", wallpaperId.toStdString());
//         return;
//     }
//
//     deleteWallpaper(wallpaper->get());
// }
//
// bool Wallpapers::applyWallpaper(const Wallpaper& wallpaper) {
//     //TODO Implement own renderer
//
//     #if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//     const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
//     sockaddr_un sockAddr(AF_UNIX, "/tmp/blaadpapers-mpvpaper.sock");
//
//     if(connect(sock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) < 0) {
//         util::logError("Can't connect to mpvpaper socket");
//         close(sock);
//         return false;
//     }
//
//     const auto command = QString("{\"command\":[\"loadfile\", \"%1\"]}\n").arg(wallpaper.getFilePath()).toStdString();
//     if(write(sock, command.c_str(), command.size()) < 0) {
//         util::logError("Can't write data to mpvpaper socket");
//         close(sock);
//         return false;
//     }
//
//     close(sock);
//
//     QFile file(util::getCurrentWallpaperDataPath());
//     file.open(QIODeviceBase::WriteOnly);
//     file.write(wallpaper.getId().toStdString().c_str());
//     file.close();
//
//     system(
//         QString(R"(bash "%1" "%2" "%3")").arg(
//             Config::getPostSetScriptFilePath(),
//             wallpaper.getName(),
//             wallpaper.getFilePath()
//         ).toStdString().c_str()
//     );
//     #endif
//
//     return true;
// }
//
// bool Wallpapers::applyWallpaper(const QString& wallpaperId) {
//     const auto wallpaper = getWallpaper(wallpaperId);
//
//     if(!wallpaper.has_value()) {
//         util::logError("Wallpaper \"{}\" not found", wallpaperId.toStdString());
//         util::sendStatus("Wallpaper \"{}\" not found", wallpaperId.toStdString());
//         return false;
//     }
//
//     return applyWallpaper(wallpaper->get());
// }


Wallpapers& Wallpapers::inst() {
    static Wallpapers inst;
    return inst;
}

Wallpaper* Wallpapers::get(const int index) const {
    if(index >= wallpapers.size()) {
        return nullptr;
    }

    return wallpapers[index].get();
}

Wallpaper* Wallpapers::get(const QString& id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper.get();
        }
    }

    return nullptr;
}

void Wallpapers::add(uptr<Wallpaper> wallpaper) {
    wallpapers.push_back(std::move(wallpaper));
}

void Wallpapers::remove(const QString& id) {
    std::erase_if(
        wallpapers,
        [id](const uptr<Wallpaper>& wallpaper) {
            return wallpaper->getId() == id;
        }
    );
}

int Wallpapers::count() const {
    return static_cast<int>(wallpapers.size());
}

std::vector<uptr<Wallpaper>>::const_iterator Wallpapers::begin() const {
    return wallpapers.begin();
}

std::vector<uptr<Wallpaper>>::const_iterator Wallpapers::end() const {
    return wallpapers.end();
}
