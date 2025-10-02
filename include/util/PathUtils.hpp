// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDir>
#include <QScreen>
#include <QStandardPaths>
#include <QString>
#include "util/Loggers.hpp"

namespace util {
    inline void createDirIfNotExists(const QString& path) {
        if(const QDir dir(path); !dir.exists()) {
            if(!dir.mkpath(path)) {
                logError("Can't create directory \"{}\"", path.toStdString());
                sendStatus("Can't create directory \"{}\"", path.toStdString());
            }
        }
    }

    inline QString getPreviewPath(const QScreen* screen, const QString& wallpaperId) {
        const QString previewsPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview/";
        const QString screenPreviewsPath = previewsPath + QString::number(screen->geometry().width() * screen->devicePixelRatio()) + "x" + QString::number(screen->geometry().height() * screen->devicePixelRatio()) + '/';

        createDirIfNotExists(screenPreviewsPath);

        return screenPreviewsPath + wallpaperId + ".webp";
    }
}
