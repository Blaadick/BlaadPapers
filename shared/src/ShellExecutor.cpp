// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ShellExecutor.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <string>
#include <vector>
#include "util/StringUtils.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <spawn.h>
#include <sys/wait.h>
extern char** environ;
#endif

namespace {
    auto isOverridden(const std::string_view var, const std::vector<std::string>& env) -> bool {
        const auto name = var.substr(0, var.find('=') + 1);
        return std::ranges::any_of(
            env,
            [&name](const std::string& envVar) {
                return envVar.starts_with(name);
            }
        );
    }
}

ShellExecutor::ShellExecutor(sptr<util::Logger> logger) : logger(std::move(logger)) {}

auto ShellExecutor::execute(const std::string_view command, std::vector<std::string> customEnv) const -> int {
    if(command.empty()) {
        return 0;
    }

    #ifdef _WIN32
    std::string envBlock;
    for(const auto& var : customEnv) {
        envBlock += var;
        envBlock += '\0';
    }
    for(auto var = _environ; *var; ++var) {
        if(!isOverridden(*var, customEnv)) {
            envBlock += *var;
            envBlock += '\0';
        }
    }
    envBlock += '\0';

    auto commandLine = std::format(R"("{}" /d /s /c "{}")", "cmd.exe", command);

    STARTUPINFOA startupInfo{.cb = sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION processInfo{};
    if(!CreateProcessA(nullptr, commandLine.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, envBlock.data(), nullptr, &startupInfo, &processInfo)) {
        logger->logWarning(std::format("Failed to run \"{}\" with {}", command, "cmd.exe"));
        return -1;
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);

    DWORD exitCode = -1;
    GetExitCodeProcess(processInfo.hProcess, &exitCode);

    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);

    return static_cast<int>(exitCode);
    #else
    std::vector<char*> env;
    for(auto& var : customEnv) {
        env.push_back(var.data());
    }
    for(auto var = environ; *var; ++var) {
        if(!isOverridden(*var, customEnv)) {
            env.push_back(*var);
        }
    }
    env.push_back(nullptr);

    auto shell = const_cast<char*>("/bin/sh");
    std::array argv = {shell, const_cast<char*>("-c"), const_cast<char*>(command.data()), static_cast<char*>(nullptr)};

    pid_t pid;
    if(posix_spawnp(&pid, shell, nullptr, nullptr, argv.data(), env.data()) != 0) {
        logger->logWarning(std::format("Failed to run \"{}\" with \"{}\"", command, shell));
        return -1;
    }

    int status;
    if(waitpid(pid, &status, 0) < 0 || !WIFEXITED(status)) {
        return -1;
    }

    return WEXITSTATUS(status);
    #endif
}

auto ShellExecutor::executeWithWallpaperData(const std::string_view command, const Wallpaper& wallpaper) const -> int {
    return execute(
        command,
        std::vector{
            std::format("WALLPAPER_ID={}", wallpaper.getId()),
            std::format("WALLPAPER_NAME={}", wallpaper.getName()),
            std::format("WALLPAPER_TAGS={}", util::join(wallpaper.getTags(), ",")),
            std::format("WALLPAPER_SOURCE={}", wallpaper.getSource()),
            std::format("WALLPAPER_FILE_PATH={}", wallpaper.getFilePath().string())
        }
    );
}
