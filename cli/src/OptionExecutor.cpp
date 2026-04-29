// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "OptionExecutor.hpp"

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <print>
#include <random>
#include <spawn.h>
#include "HelpStrings.hpp"
#include "Wallpapers.hpp"
#include "util/PathUtils.hpp"
#include "util/Prints.hpp"

namespace fs = std::filesystem;
namespace rng = std::ranges;

void helpOption(const std::unordered_set<char>&, const std::vector<char*>&) {
    std::println(mainHelpMessage);
}

void versionOption(const std::unordered_set<char>& subOptions, const std::vector<char*>&) {
    if(subOptions.contains('j')) {
        const nlohmann::json outputData = {
            {"name", PROJECT_NAME},
            {"description", PROJECT_DESCRIPTION},
            {"version", PROJECT_VERSION}
        };

        std::println(outputData.dump());
    } else {
        std::println("{} {}\n{}", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION);
    }
}

//TODO Implement own renderer
void startDaemonOption(const std::unordered_set<char>&, const std::vector<char*>& arguments) {
    if(system("pgrep -x mpvpaper > /dev/null 2>&1") == 0) {
        std::println("Mpvpaper is already running");
        return;
    }

    std::string mpvArgs = "input-ipc-server=/tmp/blaadpapers-mpvpaper.sock loop-file=inf no-audio panscan=1.0 ";
    if(!arguments.empty()) {
        mpvArgs += arguments[0];
    }

    auto currentWallpaperPath = util::defaultWallpaperPath();

    if(!fs::exists(util::currentWallpaperIdPath())) {
        std::ofstream currentWallpaperIdFile(util::currentWallpaperIdPath());
        currentWallpaperIdFile << "null";
    } else {
        std::string currentWallpaperId;
        std::ifstream currentWallpaperIdFile(util::currentWallpaperIdPath());
        std::getline(currentWallpaperIdFile, currentWallpaperId);

        const auto currentWallpaper = Wallpapers::inst().get(currentWallpaperId);
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
}

void applyOption(const std::unordered_set<char>&, const std::vector<char*>& arguments) {
    if(arguments.empty()) {
        std::println(stderr, "Wallpaper id expected");
        return;
    }

    if(Wallpapers::inst().apply(arguments[0])) {
        std::println("Wallpaper \"{}\" applied", arguments[0]);
    } else {
        std::println(stderr, "Wallpaper \"{}\" not found", arguments[0]);
    }
}

void randomOption(const std::unordered_set<char>& subOptions, const std::vector<char*>& arguments) {
    if(Wallpapers::inst().count() < 1) {
        std::println(stderr, "No Wallpapers");
        return;
    }

    std::mt19937 rng(std::random_device{}());
    const Wallpaper* wallpaperToApply;

    if(subOptions.contains('f')) {
        std::vector<std::string> includeTags;
        std::vector<std::string> excludeTags;
        std::vector<const Wallpaper*> filteredWallpapers;

        const auto excludeTagsData = nlohmann::json::parse(arguments[1]);

        if(!arguments.empty()) {
            const auto includeTagsData = nlohmann::json::parse(arguments[0]);
            if(includeTagsData.is_discarded()) {
                std::println(stderr, "Failed to parse include tags");
                return;
            }

            includeTags = includeTagsData;
        } else {
            std::println(stderr, "Array with include tags expected");
            return;
        }

        if(arguments.size() > 1) {
            const auto includeTagsData = nlohmann::json::parse(arguments[0]);
            if(includeTagsData.is_discarded()) {
                std::println(stderr, "Failed to parse exclude tags");
                return;
            }

            includeTags = includeTagsData;
        }

        for(const auto& wallpaper : Wallpapers::inst()) {
            const bool containsIncludeTags = rng::all_of(
                includeTags,
                [&wallpaper](const std::string& tag) {
                    return rng::contains(wallpaper->getTags(), tag);
                }
            );

            const bool containsExcludeTags = rng::any_of(
                excludeTags,
                [&wallpaper](const std::string& tag) {
                    return rng::contains(wallpaper->getTags(), tag);
                }
            );

            if(containsIncludeTags && !containsExcludeTags) {
                filteredWallpapers.emplace_back(wallpaper.get());
            }
        }

        if(filteredWallpapers.empty()) {
            std::println(stderr, "No wallpapers found");
            return;
        }

        const auto randomIndex = std::uniform_int_distribution(0, static_cast<int>(filteredWallpapers.size()))(rng);
        wallpaperToApply = filteredWallpapers[randomIndex];
    } else {
        const auto randomIndex = std::uniform_int_distribution(0, Wallpapers::inst().count())(rng);
        wallpaperToApply = Wallpapers::inst().get(randomIndex);
    }

    if(Wallpapers::inst().apply(wallpaperToApply->getId())) {
        std::println("Wallpaper \"{}\" applied", wallpaperToApply->getId());
    } else {
        std::println(stderr, "Wallpaper \"{}\" not found", wallpaperToApply->getId());
    }
}

void deleteOption(const std::unordered_set<char>&, const std::vector<char*>& arguments) {
    if(arguments.empty()) {
        std::println(stderr, "Wallpaper id expected");
        return;
    }

    if(Wallpapers::inst().remove(arguments[0])) {
        std::println("Wallpaper \"{}\" deleted", arguments[0]);
    } else {
        std::println(stderr, "Wallpaper \"{}\" not found", arguments[0]);
    }
}

void infoOption(const std::unordered_set<char>& subOptions, const std::vector<char*>& arguments) {
    if(arguments.empty()) {
        std::println(stderr, "Wallpaper id expected");
        return;
    }

    const auto wallpaper = Wallpapers::inst().get(arguments[0]);

    if(!wallpaper) {
        std::println(stderr, "Wallpaper \"{}\" not found", arguments[0]);
        return;
    }

    if(subOptions.contains('j')) {
        std::println(wallpaper->toJson().dump());
    } else {
        std::print(wallpaper->toString());
    }
}

void listOption(const std::unordered_set<char>& subOptions, const std::vector<char*>&) {
    if(subOptions.contains('j')) {
        nlohmann::json wallpapersData;
        for(const auto& wallpaper : Wallpapers::inst()) {
            wallpapersData.emplace_back(wallpaper->toJson());
        }

        std::println(wallpapersData.dump());
    } else {
        std::string output;
        for(const auto& wallpaper : Wallpapers::inst()) {
            output += wallpaper->toString() + '\n';
        }

        std::println(output.erase(output.size() - 2));
    }
}

void countOption(const std::unordered_set<char>& subOptions, const std::vector<char*>&) {
    if(subOptions.contains('j')) {
        const nlohmann::json outputData = {
            {"wallpaper_count", Wallpapers::inst().count()}
        };
        std::println(outputData.dump());
    } else {
        std::println(Wallpapers::inst().count());
    }
}

void OptionExecutor::execute(int argc, char** argv) {
    if(argc < 2) {
        std::println(mainHelpMessage);
        return;
    }

    const auto& option = argv[1][1];
    std::unordered_set<char> subOptions;

    if(argv[1][0] != '-') {
        std::println(stderr, "Unknown argument: {}", argv[1]);
        return;
    }

    if(strlen(argv[1]) < 2) {
        std::println(stderr, "Option not specified");
        return;
    }

    if(!options.contains(option)) {
        std::println(stderr, "Unknown option: {}", option);
        return;
    }

    for(auto i = 2; i < strlen(argv[1]); i++) {
        switch(argv[1][i]) {
            case 'h': {
                std::println(options[option].helpMessage);
                return;
            }
            case 'q': {
                freopen("/dev/null/", "w", stdout);
                freopen("/dev/null/", "w", stderr);
                break;
            }
            default: {
                if(options[option].allowableSubOptions.contains(argv[1][i])) {
                    subOptions.emplace(argv[1][i]);
                } else {
                    std::println(stderr, "Unknown sub option: {}", argv[1][i]);
                    return;
                }
            }
        }
    }

    const std::vector arguments(argv + 2, argv + argc);
    options[option].func(subOptions, arguments);
}

std::unordered_map<char, Option> OptionExecutor::options = {
    {'h', {helpOption, {}, "Ha ha!"}}, // Because it's familiar
    {'H', {helpOption, {}, "WTF bro? You really need help with it?"}},
    {'V', {versionOption, {'j'}, versionHelpMessage}},
    {'S', {startDaemonOption, {}, startDaemonHelpMessage}},
    {'A', {applyOption, {}, applyHelpMessage}},
    {'R', {randomOption, {'f'}, randomHelpMessage}},
    {'D', {deleteOption, {}, deleteHelpMessage}},
    {'I', {infoOption, {'j'}, infoHelpMessage}},
    {'L', {listOption, {'j'}, listHelpMessage}},
    {'C', {countOption, {'j'}, countHelpMessage}}
};
