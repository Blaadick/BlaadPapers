// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <vips/vips.h>
#include "Config.hpp"
#include "DefaultWallpaper.hpp"
#include "OldOptionExecutor.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
#include "argument/Argument.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    vips_init(argv[0]);
    vips_cache_set_max(0);

    Config::load();
    PostSetScript::createIfNotExists();
    DefaultWallpaper::createIfNotExists();

    WallpaperLoader::loadWallpapers();
    OldOptionExecutor::execute(argc, argv);

    Argument<int> wallpaperId("dawd", true);

    vips_shutdown();
}
