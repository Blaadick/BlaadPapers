// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDir>
#include <QStandardPaths>

namespace util {
    inline bool createDirIfNotExists(const QString& path) {
        if(const QDir dir(path); !dir.exists()) {
            if(!dir.mkpath(path)) {
                //TODO REMOVE
                // logWarn("Can't create directory \"{}\"", path.toStdString());
                return false;
            }
        }

        return true;
    }

    inline bool open(QFile& file, const QFile::OpenMode& flags) {
        if(!file.open(flags)) {
            //TODO REMOVE
            // logWarn("Can't open file \"{}\"", file.filesystemFileName().string());
            return false;
        }

        return true;
    }

    inline QString getDataPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }

    //TODO Move this shit to gui
    inline QString getPreviewsPath() {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview";
    }

    inline QString getCurrentWallpaperDataPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/current-wallpaper";
    }

    inline QString getDefaultWallpaperPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/default-wallpaper.png";
    }
}
