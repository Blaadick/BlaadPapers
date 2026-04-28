// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"
#include "OptionExecutor.hpp"
#include "WallpaperLoader.hpp"

int main(int argc, char** argv) {
    Config::load();
    WallpaperLoader::loadWallpapers();
    OptionExecutor::execute(argc, argv);
}
