// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpaper.hpp"

#include <QJsonArray>
#include "Config.hpp"
#include "util/FormatUtils.hpp"

Wallpaper::Wallpaper(
    const QString& id,
    const QString& format,
    const QString& name,
    const QVector<QString>& tags
) : id(id), format(format), name(name), tags(tags) {}

const QString& Wallpaper::getId() const {
    return id;
}

const QString& Wallpaper::getFormat() const {
    return format;
}

const QString& Wallpaper::getName() const {
    return name;
}

const QVector<QString>& Wallpaper::getTags() const {
    return tags;
}

QString Wallpaper::getFilePath() const {
    return Config::getWallpapersPath() + '/' + id + '.' + format;
}

bool Wallpaper::isAnimated() const {
    return util::supportedAnimatedFormats.contains(format);
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
