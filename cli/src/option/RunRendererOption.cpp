// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RunRendererOption.hpp"

#include <fcntl.h>
#include <fstream>
#include <spawn.h>
#include <unistd.h>
#include "DefaultWallpaper.hpp"
#include "logger/Logger.hpp"
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

RunRendererOption::RunRendererOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option(), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::string RunRendererOption::getHelpMessage() const {
    return "run-renderer help";
}

int RunRendererOption::execute(const std::vector<std::string>& arguments) {
    if(system("pgrep -x mpvpaper > /dev/null 2>&1") == 0) {
        logger->logWarning("Mpvpaper is already running");
        return 1;
    }

    std::string mpvArgs = "input-ipc-server=/tmp/blaadpapers-mpvpaper.sock loop-file=inf no-audio panscan=1.0 ";
    if(!arguments.empty()) {
        mpvArgs += arguments[0];
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
        const_cast<char*>(currentWallpaperPath.c_str()),
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
}
