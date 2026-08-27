// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "PostSetScript.hpp"

#include <format>
#include <fstream>
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

void PostSetScript::createIfNotExists() {
    const auto postSetScriptPath = postSetScriptFilePath();
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
            postSetScriptFilePath().c_str(),
            wallpaper.getName(),
            wallpaper.getFilePath().c_str()
        ).c_str()
    );
    #endif
}

fs::path PostSetScript::postSetScriptFilePath() {
    return util::configDir() / "post_set.sh";
}
