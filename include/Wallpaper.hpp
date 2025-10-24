// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

class Wallpaper {
public:
    Wallpaper(const QString& id, const QString& filePath, const QJsonObject& data);

    Wallpaper() = default;

    virtual ~Wallpaper() = default;

    [[nodiscard]]
    const QString& getId() const;

    [[nodiscard]]
    const QString& getFilePath() const;

    [[nodiscard]]
    const QString& getName() const;

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
    QString name;
    QVector<QString> tags;
};
