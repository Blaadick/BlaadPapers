// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

class Wallpaper {
public:
    enum WallpaperType {
        PICTURE,
        VIDEO,
        SCENE
    };

    Wallpaper(const QString& id, const QString& filePath, const WallpaperType& type, const QJsonObject& data);

    [[nodiscard]]
    const QString& getId() const;

    [[nodiscard]]
    const QString& getFilePath() const;

    [[nodiscard]]
    const WallpaperType& getType() const;

    [[nodiscard]]
    const QString& getName() const;

    void setName(const QString& name);

    [[nodiscard]]
    const QVector<QString>& getTags() const;

    [[nodiscard]]
    bool isBad() const;

    [[nodiscard]]
    QJsonObject toJson() const;

    [[nodiscard]]
    bool operator==(const Wallpaper& other) const;

private:
    QString id;
    QString filePath;
    WallpaperType type;
    QString name;
    QVector<QString> tags;
};
