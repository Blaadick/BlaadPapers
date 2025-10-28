// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

#include <QJsonArray>
#include "Config.hpp"
#include "data/WallpaperType.hpp"
#include "util/PathUtils.hpp"

Wallpaper::Wallpaper(const QString& id, const QString& filePath, const WallpaperType& type, const QJsonObject& data) {
    this->id = id;
    this->filePath = filePath;
    this->type = type;
    this->name = data["name"].toString();
    this->source = data["source"].toString();

    for(auto tag : data["tags"].toArray()) {
        tags.append(tag.toString());
    }
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
        {"resolution", toString(resolution)},
        {"source", source},
        {"tags", wallpaperTags},
        {"type", toString(type)}
    };
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return id == other.getId();
}
