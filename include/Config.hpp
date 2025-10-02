// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonObject>

class Config {
public:
    static void load();

    [[nodiscard]]
    static QString getConfigFilePath();

    [[nodiscard]]
    static QString getPostSetScriptFilePath();

    [[nodiscard]]
    static QVector<QString> getBadTags();

    [[nodiscard]]
    static QString getStyle();

    [[nodiscard]]
    static QString getWallpapersPath();

private:
    static QJsonObject defaultData;
    static QJsonObject data;

    [[nodiscard]]
    inline static QString getConfigPath();

    [[nodiscard]]
    inline static QJsonValueRef getValue(const QString& key);
};
