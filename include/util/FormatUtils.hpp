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
}
