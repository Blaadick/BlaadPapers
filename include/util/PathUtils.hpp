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

    inline QString getScreenPreviewsPath(const QScreen* screen) {
        return getPreviewsPath() + toString(screen->size() * screen->devicePixelRatio()) + '/';
    }

    inline QString getLocalDataPath() {
        return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/blaadpapers/";
    }

    inline QString getCurrentWallpaperDataPath() {
        return getLocalDataPath() + "current-wallpaper";
    }

    inline QString getDefaultWallpaperPath() {
        return getLocalDataPath() + "default-wallpaper.png";
    }
}
