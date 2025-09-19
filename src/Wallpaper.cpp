#include "Wallpaper.hpp"

#include <QJsonArray>
#include "Config.hpp"

Wallpaper::Wallpaper(
    const QString& name,
    const QString& description,
    const QString& picturePath,
    const QVector<QString>& tags
) : name(name), description(description), picturePath(picturePath), tags(tags) {}

const QString& Wallpaper::getName() const {
    return name;
}

const QString& Wallpaper::getDescription() const {
    return description;
}

const QString& Wallpaper::getPicturePath() const {
    return picturePath;
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
        {"description", description},
        {"tags", wallpaperTags}
    };
}
