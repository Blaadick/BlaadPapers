// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/Wallpaper.hpp"

class PictureWallpaper : public Wallpaper {
public:
    PictureWallpaper(
        QString id,
        QString filePath,
        QString name,
        QSize resolution,
        QString source,
        QVector<QString> tags
    );

    [[nodiscard]]
    QJsonObject toJson() const override;

    [[nodiscard]]
    QString toString() const override;
};
