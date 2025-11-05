// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonObject>
#include <QSize>
#include <QString>
#include <QVector>
#include "data/WallpaperType.hpp"

class Wallpaper {
public:
    Wallpaper(
        const QString& id,
        const QString& filePath,
        const QSize& resolution,
        const WallpaperType& type,
        const QJsonObject& data
    );

    [[nodiscard]]
    const QString& getId() const;

    [[nodiscard]]
    const QString& getFilePath() const;

    [[nodiscard]]
    const QString& getName() const;

    void setName(const QString& newName);

    [[nodiscard]]
    const QSize& getResolution() const;

    [[nodiscard]]
    const QString& getSource() const;

    [[nodiscard]]
    const QVector<QString>& getTags() const;

    [[nodiscard]]
    const WallpaperType& getType() const;

    [[nodiscard]]
    bool isBad() const;

    [[nodiscard]]
    QJsonObject toJson() const;

    [[nodiscard]]
    QString toString() const;

    [[nodiscard]]
    bool operator==(const Wallpaper& other) const;

private:
    QString id;
    QString filePath;
    QString name;
    QSize resolution;
    QString source;
    QVector<QString> tags;
    WallpaperType type;
};
