// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include "config/Config.hpp"

class ConfigModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString configFilePath READ getConfigFilePath)
    Q_PROPERTY(bool statusBarVisible READ getStatusBarVisible WRITE setStatusBarVisible NOTIFY statusBarVisibleChanged)
    Q_PROPERTY(bool badTaggedWallpapersVisible READ getBadTaggedWallpapersVisible WRITE setBadTaggedWallpapersVisible NOTIFY badTaggedWallpapersVisibleChanged)

public:
    explicit ConfigModel(sptr<Config> config);

    auto getConfigFilePath() -> QString;

    auto getStatusBarVisible() -> bool;

    void setStatusBarVisible(bool newVisibility);

    auto getBadTaggedWallpapersVisible() -> bool;

    void setBadTaggedWallpapersVisible(bool newVisibility);

private:
    sptr<Config> config;

signals:
    void statusBarVisibleChanged();

    void badTaggedWallpapersVisibleChanged();
};
