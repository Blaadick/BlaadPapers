// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonObject>
#include <QSize>
#include <QVector>

class Wallpaper {
public:
    virtual ~Wallpaper() = default;

    [[nodiscard]]
    const QString& getId() const;

    [[nodiscard]]
    const QString& getFilePath() const;

    [[nodiscard]]
    const QString& getName() const;

    [[nodiscard]]
    const QSize& getResolution() const;

    [[nodiscard]]
    const QString& getSource() const;

    [[nodiscard]]
    const QVector<QString>& getTags() const;

    [[nodiscard]]
    bool isBad() const;

    bool apply() const;

    void remove() const;

    [[nodiscard]]
    virtual QJsonObject toJson() const = 0;

    [[nodiscard]]
    virtual QString toString() const = 0;

    [[nodiscard]]
    bool operator==(const Wallpaper& other) const;

protected:
    QString id;
    QString filePath;
    QString name;
    QSize resolution;
    QString source;
    QVector<QString> tags;
};
