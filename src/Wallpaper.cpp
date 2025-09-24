#include "Wallpaper.hpp"

#include <QJsonArray>
#include "Config.hpp"

Wallpaper::Wallpaper(
    const QString& name,
    const QString& filePath,
    const QVector<QString>& tags
) : name(name), filePath(filePath), tags(tags) {}

const QString& Wallpaper::getName() const {
    return name;
}

const QString& Wallpaper::getFilePath() const {
    return filePath;
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
        {"tags", wallpaperTags}
    };
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return filePath == other.filePath;
}