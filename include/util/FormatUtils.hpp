// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace util {
    inline const QVector<QString> supportedPictureFormats = {
        "png",
        "webp",
        "jpeg"
    };

    inline const QVector<QString> supportedVideoFormats = {
        "mp4",
        "webm"
    };

    inline const QVector<QString> supportedSceneFormats = {
        "tar"
    };

    inline const QVector<QString> supportedFormats = supportedPictureFormats + supportedVideoFormats + supportedSceneFormats;

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
        QDirIterator dirIterator(
            Config::getWallpapersDirPath(),
            {"*.jpg", "*.jpe", "*.JPG"},
            QDir::NoFilter
        );

        while(dirIterator.hasNext()) {
            auto filePath = dirIterator.next();
            auto newFilePath = filePath;

            newFilePath.chop(3);
            newFilePath.append("jpeg");

            QFile::rename(filePath, newFilePath);
        }
    }
}
