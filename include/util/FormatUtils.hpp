// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDirIterator>
#include <QFile>
#include <QString>

namespace util {
    inline const QVector<QString> supportedPictureFormats = {"png", "apng", "jpeg", "webp"};
    inline const QVector<QString> supportedVideoFormats = {"mp4", "webm"};
    inline const QVector<QString> supportedFormats = supportedPictureFormats + supportedVideoFormats;

    inline QVector<QString> getFileMask(QVector<QString> vector) {
        for(auto& item : vector) {
            item.prepend("*.");
        }

        return vector;
    }
}
