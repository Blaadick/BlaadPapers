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
