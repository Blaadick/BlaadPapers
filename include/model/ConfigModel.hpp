// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QObject>

class ConfigModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool statusBarVisible READ getStatusBarVisible WRITE setStatusBarVisible NOTIFY statusBarVisibleChanged)

public:
    static ConfigModel& inst();

    Q_INVOKABLE QString getConfigFilePath();

    bool getStatusBarVisible();

    void setStatusBarVisible(bool isVisible);

signals:
    void statusBarVisibleChanged();
};
