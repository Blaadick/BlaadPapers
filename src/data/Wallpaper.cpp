// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

const QString& Wallpaper::getId() const {
    return id;
}

const QString& Wallpaper::getFilePath() const {
    return filePath;
}

const QString& Wallpaper::getName() const {
    return name;
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

bool Wallpaper::isBad() const {
    return std::ranges::any_of(
        Config::getBadTags(),
        [this](const QString& tag) {
            return tags.contains(tag);
        }
    );
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return id == other.getId();
}
