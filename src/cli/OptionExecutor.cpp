// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "cli/OptionExecutor.hpp"

#include <QRandomGenerator>
#include <cstring>
#include <iostream>
#include "Tags.hpp"
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
        util::logInfo(QJsonDocument(outputData));
    } else {
        util::logInfo("{} {}", PROJECT_NAME, PROJECT_VERSION);
    }
}

void applyOption(const set<char>&, const vector<char*>& arguments) {
    if(arguments.empty()) {
        util::logError("Wallpaper id expected");
        return;
    }

    Wallpapers::applyWallpaper(arguments[0]);
}

void randomOption(const set<char>& subOptions, const vector<char*>& arguments) {
    const auto& wallpapers = Wallpapers::getWallpapers();

    if(wallpapers.empty()) {
        util::logInfo("No Wallpapers");
        return;
    }

    if(subOptions.contains('f')) {
        QVector<QString> includeTags;
        QVector<QString> excludeTags;
        QVector<Wallpaper> filteredWallpapers;

        if(!arguments.empty()) {
            for(auto tag : QJsonDocument::fromJson(arguments[0]).array()) {
                includeTags.append(tag.toString());
            }
        } else {
            util::logError("Array with include tags expected");
            return;
        }

        if(arguments.size() > 1) {
            for(auto tag : QJsonDocument::fromJson(arguments[1]).array()) {
                excludeTags.append(tag.toString());
            }
        }

        for(const auto& wallpaper : wallpapers) {
            const bool containsIncludeTags = ranges::all_of(
                includeTags,
                [&wallpaper](const QString& tag) {
                    return wallpaper.getTags().contains(tag);
                }
            );

            const bool containsExcludeTags = ranges::any_of(
                excludeTags,
                [&wallpaper](const QString& tag) {
                    return wallpaper.getTags().contains(tag);
                }
            );

            if(containsIncludeTags && !containsExcludeTags) {
                filteredWallpapers.append(wallpaper);
            }
        }

        if(filteredWallpapers.empty()) {
            util::logInfo("No wallpapers found");
            return;
        }

        const auto randomIndex = QRandomGenerator::global()->bounded(filteredWallpapers.size());
        Wallpapers::applyWallpaper(filteredWallpapers[randomIndex].getId());
    } else {
        const auto randomIndex = QRandomGenerator::global()->bounded(wallpapers.size());
        Wallpapers::applyWallpaper(wallpapers[randomIndex].getId());
    }
}

void deleteOption(const set<char>&, const vector<char*>& arguments) {
    if(arguments.empty()) {
        util::logError("Wallpaper id expected");
        return;
    }

    Wallpapers::deleteWallpaper(arguments[0]);
}

void infoOption(const set<char>& subOptions, const vector<char*>& arguments) {
    if(arguments.empty()) {
        util::logError("Wallpaper id expected");
        return;
    }

    const auto wallpaper = Wallpapers::getWallpaper(arguments[0]);

    if(!wallpaper) {
        util::logError("Wallpaper {} not found", arguments[0]);
        return;
    }

    if(subOptions.contains('j')) {
        util::logInfo(QJsonDocument(wallpaper->toJson()));
    } else {
        util::logInfo(
            "{}\n"
            "    Id: {}\n"
            "    Path: {}\n"
            "    Resolution: {}\n"
            "    Source: {}\n"
            "    Tags: {}\n"
            "    Type: {}",
            wallpaper->getName().toStdString(),
            wallpaper->getId().toStdString(),
            wallpaper->getFilePath().toStdString(),
            toString(wallpaper->getResolution()).toStdString(),
            wallpaper->getSource().toStdString(),
            wallpaper->getTags().join(", ").toStdString(),
            toString(wallpaper->getType()).toStdString()
        );
    }
}

void listOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('t')) {
        if(subOptions.contains('j')) {
            QJsonArray outputData;

            for(const auto& [name, quantity] : Tags::getUniqueTags().asKeyValueRange()) {
                outputData.append(QJsonObject{
                    {"name", name},
                    {"quantity", quantity}
                });
            }

            util::logInfo(QJsonDocument(outputData));
        } else {
            for(const auto& [name, quantity] : Tags::getUniqueTags().asKeyValueRange()) {
                util::logInfo("{:->4}: {}", quantity, name.toStdString());
            }
        }

        return;
    }

    if(subOptions.contains('j')) {
        util::logInfo(QJsonDocument(Wallpapers::toJson()));
    } else {
        for(const auto& wallpaper : Wallpapers::getWallpapers()) {
            util::logInfo(wallpaper.getId());
        }
    }
}

void countOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        const QJsonObject outputData{
            {"wallpaper_count", Wallpapers::count()}
        };

        util::logInfo(QJsonDocument(outputData));
    } else {
        util::logInfo("{}", Wallpapers::count());
    }
}

void OptionExecutor::execute(const int argc, char** argv) {
    const auto& option = argv[1][1];
    set<char> subOptions;

    if(argv[1][0] != '-') {
        util::logError("Unknown argument: {}", argv[1]);
        return;
    }

    if(strlen(argv[1]) < 2) {
        util::logError("Option not specified");
        return;
    }

    if(!options.contains(option)) {
        util::logError("Unknown option: {}", option);
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
                    util::logError("Unknown sub option: {}", argv[1][i]);
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
    {'A', {applyOption, {}, applyHelpMessage}},
    {'R', {randomOption, {'f'}, randomHelpMessage}},
    {'D', {deleteOption, {}, deleteHelpMessage}},
    {'I', {infoOption, {'j'}, infoHelpMessage}},
    {'L', {listOption, {'t', 'j'}, listHelpMessage}},
    {'C', {countOption, {'j'}, countHelpMessage}}
};
