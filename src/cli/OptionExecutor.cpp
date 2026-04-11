// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "cli/OptionExecutor.hpp"

#include <fcntl.h>
#include <QJsonArray>
#include <QRandomGenerator>
#include <spawn.h>
#include "Wallpapers.hpp"
#include "cli/HelpStrings.hpp"
#include "model/WallpapersModel.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"

using namespace std;

void helpOption(const set<char>&, const vector<char*>&) {
    cout << mainHelpMessage << endl;
}

void versionOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        const QJsonObject outputData{
            {"name", PROJECT_NAME},
            {"description", PROJECT_DESCRIPTION},
            {"version", PROJECT_VERSION}
        };

        std::println(QJsonDocument(outputData));
    } else {
        std::println("{} {}", PROJECT_NAME, PROJECT_VERSION);
    }
}

void startDaemonOption(const set<char>&, const vector<char*>& arguments) {
    //TODO Implement own renderer

    if(system("pgrep -x mpvpaper > /dev/null 2>&1") == 0) {
        std::println("Mpvpaper is already running");
        return;
    }

    std::string mpvArgs = "input-ipc-server=/tmp/blaadpapers-mpvpaper.sock loop-file=inf no-audio panscan=1.0 ";
    if(!arguments.empty()) {
        mpvArgs += arguments[0];
    }

    std::string currentWallpaperPath = util::getDefaultWallpaperPath().toStdString();

    if(QFile file(util::getCurrentWallpaperDataPath()); !file.exists()) {
        util::open(file, QIODeviceBase::WriteOnly);
        file.write("");
        file.close();
    } else {
        util::open(file, QIODeviceBase::ReadOnly);
        const QString currentWallpaperId = file.readAll();
        file.close();

        const auto currentWallpaper = Wallpapers::inst().get(currentWallpaperId);
        if(currentWallpaper) {
            currentWallpaperPath = currentWallpaper->getFilePath().toStdString();
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

void applyOption(const set<char>&, const vector<char*>& arguments) {
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

void randomOption(const set<char>& subOptions, const vector<char*>& arguments) {
    if(Wallpapers::inst().count() < 1) {
        std::println(stderr, "No Wallpapers");
        return;
    }

    const Wallpaper* wallpaperToApply;

    if(subOptions.contains('f')) {
        QVector<QString> includeTags;
        QVector<QString> excludeTags;
        QVector<const Wallpaper*> filteredWallpapers;

        if(!arguments.empty()) {
            for(auto tag : QJsonDocument::fromJson(arguments[0]).array()) {
                includeTags.append(tag.toString());
            }
        } else {
            std::println(stderr, "Array with include tags expected");
            return;
        }

        if(arguments.size() > 1) {
            for(auto tag : QJsonDocument::fromJson(arguments[1]).array()) {
                excludeTags.append(tag.toString());
            }
        }

        for(const auto& wallpaper : Wallpapers::inst()) {
            const bool containsIncludeTags = ranges::all_of(
                includeTags,
                [&wallpaper](const QString& tag) {
                    return wallpaper->getTags().contains(tag);
                }
            );

            const bool containsExcludeTags = ranges::any_of(
                excludeTags,
                [&wallpaper](const QString& tag) {
                    return wallpaper->getTags().contains(tag);
                }
            );

            if(containsIncludeTags && !containsExcludeTags) {
                filteredWallpapers.append(wallpaper.get());
            }
        }

        if(filteredWallpapers.empty()) {
            std::println(stderr, "No wallpapers found");
            return;
        }

        const auto randomIndex = QRandomGenerator::global()->bounded(filteredWallpapers.size());
        wallpaperToApply = filteredWallpapers[randomIndex];
    } else {
        const auto randomIndex = QRandomGenerator::global()->bounded(Wallpapers::inst().count());
        wallpaperToApply = Wallpapers::inst().get(randomIndex);
    }

    if(wallpaperToApply->apply()) {
        std::println("Wallpaper \"{}\" applied", wallpaperToApply->getId().toStdString());
    } else {
        std::println(stderr, "Wallpaper \"{}\" not found", wallpaperToApply->getId().toStdString());
    }
}

void deleteOption(const set<char>&, const vector<char*>& arguments) {
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

void infoOption(const set<char>& subOptions, const vector<char*>& arguments) {
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
        std::println(QJsonDocument(wallpaper->toJson()));
    } else {
        std::println(wallpaper->toString());
    }
}

void listOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        QJsonArray wallpapersData;

        for(const auto& wallpaper : Wallpapers::inst()) {
            wallpapersData.append(wallpaper->toJson());
        }
        std::println(QJsonDocument(wallpapersData));
    } else {
        QString output;

        for(const auto& wallpaper : Wallpapers::inst()) {
            output += wallpaper->toString() + '\n';
        }

        output.chop(2);
        std::println(output);
    }
}

void countOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        const QJsonObject outputData{
            {"wallpaper_count", Wallpapers::inst().count()}
        };
        std::println(QJsonDocument(outputData));
    } else {
        std::println(Wallpapers::inst().count());
    }
}

void OptionExecutor::execute(const int argc, char** argv) {
    const auto& option = argv[1][1];
    set<char> subOptions;

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
                cout << options[option].helpMessage << endl;
                return;
            }
            case 'q': {
                freopen("/dev/null/", "w", stdout);
                freopen("/dev/null/", "w", stderr);
                break;
            }
            default: {
                if(options[option].allowableSubOptions.contains(argv[1][i])) {
                    subOptions.insert(argv[1][i]);
                } else {
                    std::println(stderr, "Unknown sub option: {}", argv[1][i]);
                    return;
                }
            }
        }
    }

    const vector arguments(argv + 2, argv + argc);

    options[option].func(subOptions, arguments);
}

map<char, OptionExecutor::Option> OptionExecutor::options = {
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
