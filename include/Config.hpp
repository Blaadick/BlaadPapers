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
    static QString getWallpapersDirPath();

    [[nodiscard]]
    static bool getStatusBarVisible();

    static void setStatusBarVisible(bool isVisible);

private:
    static QJsonObject defaultData;
    static QJsonObject data;

    [[nodiscard]]
    inline static QString getConfigPath();

    [[nodiscard]]
    inline static QJsonValueRef getValue(const QString& key);

    inline static void setValue(const QString& key, const QJsonValue& value);
};
