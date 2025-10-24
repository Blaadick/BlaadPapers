// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpaper.hpp"

#include <QJsonArray>
#include "Config.hpp"

Wallpaper::Wallpaper(const QString& id, const QString& filePath, const QJsonObject& data) {
    this->id = id;
    this->filePath = filePath;
    this->name = data["name"].toString();

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

const QVector<QString>& Wallpaper::getTags() const {
    return tags;
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
        {"name", name},
        {"tags", wallpaperTags}
    };
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return id == other.getId();
}
