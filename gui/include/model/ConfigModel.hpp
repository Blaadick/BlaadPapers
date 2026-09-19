// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include "config/Config.hpp"

class ConfigModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString configFilePath READ getConfigFilePath)
    Q_PROPERTY(bool statusBarVisible READ getStatusBarVisible WRITE setStatusBarVisible NOTIFY statusBarVisibleChanged)

public:
    explicit ConfigModel(sptr<Config> config);

    auto getConfigFilePath() -> QString;

    auto getStatusBarVisible() -> bool;

    void setStatusBarVisible(bool isVisible);

private:
    sptr<Config> config;

signals:
    void statusBarVisibleChanged();
};
