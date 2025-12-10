// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

#include <QJsonArray>
#include "Config.hpp"
#include "data/WallpaperType.hpp"
#include "util/PathUtils.hpp"

Wallpaper::Wallpaper(
    const QString& id,
    const QString& filePath,
    const QSize& resolution,
    const WallpaperType& type,
    const QJsonObject& data
) {
    this->id = id;
    this->filePath = filePath;
    this->name = data["name"].toString();
    this->resolution = resolution;
    this->source = data["source"].toString();

    for(auto tag : data["tags"].toArray()) {
        tags.append(tag.toString());
    }

    this->type = type;
}

const QString& Wallpaper::getId() const {
    return id;
}

const QString& Wallpaper::getFilePath() const {
    return filePath;
}

const QString& Wallpaper::getName() const {
    return name;
}

void Wallpaper::setName(const QString& newName) {
    name = newName;
}

const QSize& Wallpaper::getResolution() const {
    return resolution;
}

const QString& Wallpaper::getSource() const {
    return source;
}

const QVector<QString>& Wallpaper::getTags() const {
    return tags;
}

const WallpaperType& Wallpaper::getType() const {
    return type;
}

bool Wallpaper::isBad() const {
    return std::ranges::any_of(Config::getBadTags(), [this](const QString& tag) {
        return tags.contains(tag);
    });
}

QJsonObject Wallpaper::toJson() const {
    QJsonArray wallpaperTags;

    for(const auto& tag : tags) {
        wallpaperTags.append(tag);
    }

    return QJsonObject{
        {"id", id},
        {"path", filePath},
        {"name", name},
        {"resolution", ::toString(resolution)},
        {"source", source},
        {"tags", wallpaperTags},
        {"type", ::toString(type)}
    };
}

QString Wallpaper::toString() const {
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
        ::toString(resolution),
        source,
        tags.join(", "),
        ::toString(type)
    );
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return id == other.getId();
}
