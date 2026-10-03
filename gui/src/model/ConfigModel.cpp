// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model/ConfigModel.hpp"

#include "config/Config.hpp"
#include "util/FormatUtils.hpp"

ConfigModel::ConfigModel(sptr<Config> config) : config(std::move(config)) {}

QString ConfigModel::getConfigFilePath() {
    return QString::fromStdString(config->generalConfigFilePath().string());
}

bool ConfigModel::getStatusBarVisible() {
    return config->getStatusBarVisible();
}

void ConfigModel::setStatusBarVisible(const bool newVisibility) {
    config->setStatusBarVisible(newVisibility);
    emit statusBarVisibleChanged();
}

auto ConfigModel::getBadTaggedWallpapersVisible() -> bool {
    return config->getBadTaggedWallpapersVisible();
}

void ConfigModel::setBadTaggedWallpapersVisible(const bool newVisibility) {
    config->setBadTaggedWallpapersVisible(newVisibility);
    emit badTaggedWallpapersVisibleChanged();
}
