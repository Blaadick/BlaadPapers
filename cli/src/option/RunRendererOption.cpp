// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RunRendererOption.hpp"

#include <fstream>
#include "DefaultWallpaper.hpp"
#include "logger/Logger.hpp"
#include "util/PathUtils.hpp"

#ifdef __linux__
#include <fcntl.h>
#include <spawn.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

RunRendererOption::RunRendererOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Starts the renderer daemon"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::vector<std::string_view> RunRendererOption::getUsageStrings() const {
    return {
        "[mpv_args...]",
        "no-interpolation hwdec=vaapi"
    };
}

int RunRendererOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    #ifdef __linux__
    if(system("pgrep -x mpvpaper > /dev/null 2>&1") == 0) {
        logger->logWarning("Mpvpaper is already running");
        return 1;
    }

    std::string mpvArgs = "input-ipc-server=/tmp/blaadpapers-mpvpaper.sock loop-file=inf no-audio panscan=1.0 ";
    for(const auto& argument : arguments) {
        mpvArgs += argument;
        mpvArgs += ' ';
    }

    auto currentWallpaperPath = DefaultWallpaper::defaultWallpaperPath();

    if(!fs::exists(util::currentWallpaperIdPath())) {
        std::ofstream currentWallpaperIdFile(util::currentWallpaperIdPath());
        currentWallpaperIdFile << "null";
    } else {
        std::string currentWallpaperId;
        std::ifstream currentWallpaperIdFile(util::currentWallpaperIdPath());
        std::getline(currentWallpaperIdFile, currentWallpaperId);

        const auto currentWallpaper = wallpapers->get(currentWallpaperId);
        if(currentWallpaper) {
            currentWallpaperPath = currentWallpaper->getFilePath();
        }
    }

    char* args[] = {
        const_cast<char*>("/usr/bin/mpvpaper"),
        const_cast<char*>("-f"),
        const_cast<char*>("-o"),
        const_cast<char*>(mpvArgs.c_str()),
        const_cast<char*>("all"),
        const_cast<char*>(currentWallpaperPath.string().c_str()),
        nullptr
    };

    const int devNull = open("/dev/null", O_WRONLY);
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_adddup2(&actions, devNull, STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&actions, devNull, STDERR_FILENO);
    posix_spawn_file_actions_addclose(&actions, devNull);

    pid_t pid;
    posix_spawn(&pid, "/usr/bin/mpvpaper", &actions, nullptr, args, environ);
    posix_spawn_file_actions_destroy(&actions);

    return 0;
    #elif _WIN32
    logger->logError("Mpvpaper is not available on windows");
    return 2;
    #endif
}
