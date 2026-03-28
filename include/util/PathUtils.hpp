// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDir>
#include <QScreen>
#include <QStandardPaths>
#include "model/StatusModel.hpp"
#include "util/Loggers.hpp"

inline QString toString(const QSize& size) {
    return QString::number(size.width()) + 'x' + QString::number(size.height());
}

namespace util {
    inline void createDirIfNotExists(const QString& path) {
        if(const QDir dir(path); !dir.exists()) {
            if(!dir.mkpath(path)) {
                logError("Can't create directory \"{}\"", path.toStdString());
                sendStatus("Can't create directory \"{}\"", path.toStdString());
            }
        }
    }

    inline QString getPreviewsPath() {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview/";
    }

    inline QString getPreviewsPath(const QScreen* screen) {
        return getPreviewsPath() + toString(screen->size() * screen->devicePixelRatio()) + '/';
    }

    inline QString getDataPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + '/';
    }

    inline QString getCurrentWallpaperDataPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/current-wallpaper";
    }

    inline QString getDefaultWallpaperPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/default-wallpaper.png";
    }
}
