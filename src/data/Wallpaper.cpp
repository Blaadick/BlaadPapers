// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

#include <QDirIterator>
#include <qprocess.h>

#include "data/PictureWallpaper.hpp"

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include "unistd.h"
#include "sys/socket.h"
#include "sys/un.h"
#endif

bool Wallpaper::apply() const {
    #if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    if(qgetenv("XDG_CURRENT_DESKTOP") == "KDE") {
        if(dynamic_cast<const PictureWallpaper*>(this)) {
            QProcess plasmaSeter;
            plasmaSeter.start("plasma-apply-wallpaperimage", {filePath});
            plasmaSeter.waitForFinished(-1);
            return true;
        }

        util::logWarn("Non-picture wallpapers not yet working on KDE");
        return false;
    }

    const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un sockAddr(AF_UNIX, "/tmp/blaadpapers-mpvpaper.sock");

    if(connect(sock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) < 0) {
        util::logError("Can't connect to mpvpaper socket");
        close(sock);
        return false;
    }

    const auto command = QString("{\"command\":[\"loadfile\", \"%1\"]}\n").arg(filePath).toStdString();
    if(write(sock, command.c_str(), command.size()) < 0) {
        util::logError("Can't write data to mpvpaper socket");
        close(sock);
        return false;
    }

    close(sock);

    QFile file(util::getCurrentWallpaperDataPath());
    util::open(file, QIODeviceBase::WriteOnly);
    file.write(id.toStdString().c_str());
    file.close();

    system(
        QString(R"(bash "%1" "%2" "%3")").arg(
            Config::getPostSetScriptFilePath(),
            name,
            filePath
        ).toStdString().c_str()
    );

    return true;

    #elif defined(Q_OS_WINDOWS)

    util::logWarn("WINDOWS STILL NOT IMPLEMENTED");
    return false;

    #endif
}

void Wallpaper::remove() const {
    QFile(filePath).remove();

    for(const auto& path : Config::getWallpaperDirPaths()) {
        QFile(path + "/.index/" + id + ".json").remove();
    }

    QDirIterator dirIterator(
        util::getPreviewsPath(),
        {id + ".webp"},
        QDir::Files,
        QDirIterator::Subdirectories
    );
    while(dirIterator.hasNext()) {
        dirIterator.next();
        QFile(dirIterator.filePath()).remove();
    }
}

const QString& Wallpaper::getId() const {
    return id;
}

const QString& Wallpaper::getFilePath() const {
    return filePath;
}

const QString& Wallpaper::getName() const {
    return name;
}

const QSize& Wallpaper::getResolution() const {
    return resolution;
}

const QString& Wallpaper::getSource() const {
    return source;
}

const QVector<QString>& Wallpaper::getTags() const {
    return tags;
}

bool Wallpaper::isBad() const {
    return std::ranges::any_of(
        Config::getBadTags(),
        [this](const QString& tag) {
            return tags.contains(tag);
        }
    );
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return id == other.getId();
}
