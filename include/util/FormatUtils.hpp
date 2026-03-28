// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDirIterator>
#include <QFile>
#include <QString>

namespace util {
    inline const QVector<QString> supportedPictureFormats = {"png", "jpeg", "webp"};
    inline const QVector<QString> supportedVideoFormats = {"mp4", "webm"};
    inline const QVector<QString> supportedFormats = supportedPictureFormats + supportedVideoFormats;

    inline QVector<QString> getFileMask(QVector<QString> vector) {
        for(auto& item : vector) {
            item.prepend("*.");
        }

        return vector;
    }

    /**
    * We are not in DOS time! I turn it off if it is too annoying.
    */
    inline void jpegUnifier() {
        QDirIterator dirIterator(Config::getWallpapersDirPath(), {"*.jpg", "*.jpe", "*.JPG"}, QDir::Files);

        while(dirIterator.hasNext()) {
            auto filePath = dirIterator.next();
            auto newFilePath = filePath;

            newFilePath.chop(3);
            newFilePath.append("jpeg");

            QFile::rename(filePath, newFilePath);
        }
    }
}
