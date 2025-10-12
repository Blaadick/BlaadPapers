// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace util {
    inline const QVector<QString> supportedStaticFormats = {
        "png",
        "webp",
        "jpeg"
    };

    inline const QVector<QString> supportedAnimatedFormats = {
        "mp4",
        "webm"
    };

    inline const QVector<QString> supportedFormats = supportedStaticFormats + supportedAnimatedFormats;

    inline QVector<QString> getFileMask(QVector<QString> vector) {
        for(auto& item : vector) {
            item.prepend("*.");
        }

        return vector;
    }
}
