// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

class Wallpaper {
public:
    Wallpaper(
        const QString& id,
        const QString& format,
        const QString& name,
        const QVector<QString>& tags
    );

    [[nodiscard]]
    const QString& getId() const;

    [[nodiscard]]
    const QString& getFormat() const;

    [[nodiscard]]
    const QString& getName() const;

    [[nodiscard]]
    const QVector<QString>& getTags() const;

    [[nodiscard]]
    QString getFilePath() const;

    [[nodiscard]]
    bool isAnimated() const;

    [[nodiscard]]
    bool isBad() const;

    [[nodiscard]]
    QJsonObject toJson() const;

    [[nodiscard]]
    bool operator==(const Wallpaper& other) const;

private:
    QString id;
    QString format;
    QString name;
    QVector<QString> tags;
};
