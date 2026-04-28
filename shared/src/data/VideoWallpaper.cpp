// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/VideoWallpaper.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include "util/ToString.hpp"

VideoWallpaper::VideoWallpaper(
    QString id,
    QString filePath,
    QString name,
    const QSize resolution,
    const int frameRate,
    QString source,
    QVector<QString> tags
) {
    this->id = std::move(id);
    this->filePath = std::move(filePath);
    this->name = std::move(name);
    this->resolution = resolution;
    this->frameRate = frameRate;
    this->source = std::move(source);
    this->tags = std::move(tags);
}

int VideoWallpaper::getFrameRate() const {
    return frameRate;
}

QJsonObject VideoWallpaper::toJson() const {
    QJsonArray wallpaperTags;

    for(const auto& tag : tags) {
        wallpaperTags.append(tag);
    }

    return QJsonObject{
        {"id", id},
        {"path", filePath},
        {"name", name},
        {"resolution", QString::fromStdString(util::toString(resolution))},
        {"frame_rate", frameRate},
        {"source", source},
        {"tags", wallpaperTags},
        {"type", "Video"}
    };
}

QString VideoWallpaper::toString() const {
    return QString(
            "%1\n"
            "    Id: %2\n"
            "    Path: %3\n"
            "    Resolution: %4\n"
            "    FrameRate: %5\n"
            "    Source: %6\n"
            "    Tags: %7\n"
            "    Type: %8\n"
        )
        .arg(
            name,
            id,
            filePath,
            util::toString(resolution),
            std::to_string(frameRate),
            source,
            tags.join(", "),
            "Video"
        );
}
