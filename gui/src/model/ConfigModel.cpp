// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/ConfigModel.hpp"

#include "Config.hpp"
#include "util/FormatUtils.hpp"
#include "util/PathUtils.hpp"

ConfigModel::ConfigModel(sptr<Config> config) : config(config) {}

QString ConfigModel::getConfigFilePath() {
    return QString::fromStdString(util::configFilePath());
}

bool ConfigModel::getStatusBarVisible() {
    return config->getStatusBarVisible();
}

void ConfigModel::setStatusBarVisible(const bool isVisible) {
    config->setStatusBarVisible(isVisible);
    emit statusBarVisibleChanged();
}
