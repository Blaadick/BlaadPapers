// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include "Config.hpp"
#include "OptionExecutor.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    Config::load();
    PostSetScript::createIfNotExists();

    if(!fs::exists(util::localDataDirPath())) {
        if(!fs::create_directory(util::localDataDirPath())) {
            std::println(stderr, "Can't create \"{}\" dir", util::localDataDirPath().c_str());
        }
    }

    if(!fs::exists(util::cacheDirPath())) {
        if(!fs::create_directory(util::cacheDirPath())) {
            std::println(stderr, "Can't create \"{}\" dir", util::cacheDirPath().c_str());
        }
    }

    WallpaperLoader::loadWallpapers();
    OptionExecutor::execute(argc, argv);

    return 0;
}
