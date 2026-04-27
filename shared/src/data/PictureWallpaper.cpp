// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/PictureWallpaper.hpp"

#include <QJsonArray>
#include "util/ToString.hpp"

PictureWallpaper::PictureWallpaper(
    QString id,
    QString filePath,
    QString name,
    const QSize resolution,
    QString source,
    QVector<QString> tags
) {
    this->id = std::move(id);
    this->filePath = std::move(filePath);
    this->name = std::move(name);
    this->resolution = resolution;
    this->source = std::move(source);
    this->tags = std::move(tags);
}

QJsonObject PictureWallpaper::toJson() const {
    QJsonArray wallpaperTags;

    for(const auto& tag : tags) {
        wallpaperTags.append(tag);
    }

    return QJsonObject{
        {"id", id},
        {"path", filePath},
        {"name", name},
        {"resolution", util::toString(resolution)},
        {"source", source},
        {"tags", wallpaperTags},
        {"type", "Picture"}
    };
}

QString PictureWallpaper::toString() const {
    return QString(
            "%1\n"
            "    Id: %2\n"
            "    Path: %3\n"
            "    Resolution: %4\n"
            "    Source: %5\n"
            "    Tags: %6\n"
            "    Type: %7\n"
        )
        .arg(
            name,
            id,
            filePath,
            util::toString(resolution),
            source,
            tags.join(", "),
            "Picture"
        );
}
