// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Config.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include "unistd.h"
#include "sys/socket.h"
#include "sys/un.h"
#endif

class Wallpaper {
public:
    virtual ~Wallpaper() = default;

    [[nodiscard]]
    const QString& getId() const;

    [[nodiscard]]
    const QString& getFilePath() const;

    [[nodiscard]]
    const QString& getName() const;

    [[nodiscard]]
    const QSize& getResolution() const;

    [[nodiscard]]
    const QString& getSource() const;

    [[nodiscard]]
    const QVector<QString>& getTags() const;

    [[nodiscard]]
    bool isBad() const;

    bool apply() const {
        //TODO Implement own renderer

        #if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
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
        #endif

        return true;
    }

    [[nodiscard]]
    virtual QJsonObject toJson() const = 0;

    [[nodiscard]]
    virtual QString toString() const = 0;

    [[nodiscard]]
    bool operator==(const Wallpaper& other) const;

protected:
    QString id;
    QString filePath;
    QString name;
    QSize resolution;
    QString source;
    QVector<QString> tags;
};
