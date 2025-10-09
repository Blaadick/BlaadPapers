// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/ConfigModel.hpp"

#include "Config.hpp"

ConfigModel& ConfigModel::inst() {
    static ConfigModel instance;
    return instance;
}

QString ConfigModel::getConfigFilePath() {
    return Config::getConfigFilePath();
}

bool ConfigModel::getStatusBarVisible() {
    return Config::getStatusBarVisible();
}

void ConfigModel::setStatusBarVisible(const bool isVisible) {
    Config::setStatusBarVisible(isVisible);
    emit statusBarVisibleChanged();
}
