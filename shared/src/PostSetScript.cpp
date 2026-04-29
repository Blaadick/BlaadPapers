// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "PostSetScript.hpp"

#include <format>
#include <fstream>
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

void PostSetScript::createIfNotExists() {
    const auto postSetScriptPath = PostSetScript::postSetScriptPath();
    if(fs::exists(postSetScriptPath)) {
        return;
    }

    std::ofstream postSetScriptFile(postSetScriptPath);
    postSetScriptFile << "#!/bin/bash\n\nwallpaperName=\"$1\"\nwallpaperFilePath=\"$2\"\n";
}

void PostSetScript::execute(const Wallpaper& wallpaper) {
    #ifdef __linux__
    system(
        std::format(
            R"(bash "{}" "{}" "{}")",
            postSetScriptPath().c_str(),
            wallpaper.getName(),
            wallpaper.getFilePath().c_str()
        ).c_str()
    );
    #endif
}

fs::path PostSetScript::postSetScriptPath() {
    return util::configDirPath().append("post_set.sh");
}
