#include "OptionExecutor.hpp"

#include <cstring>
#include <iostream>
#include <QRandomGenerator>
#include "HelpStrings.hpp"
#include "Wallpapers.hpp"
#include "model/WallpaperList.hpp"
#include "util/Seters.hpp"

using namespace std;

void helpOption(const set<char>&, const vector<char*>&) {
    cout << mainHelpMessage;
}

void versionOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        const QJsonObject outputData{
            {"name", PROJECT_NAME},
            {"description", PROJECT_DESCRIPTION},
            {"version", PROJECT_VERSION}
        };
        logInfo(QJsonDocument(outputData));
    } else {
        logInfo("{} {}", PROJECT_NAME, PROJECT_VERSION);
    }
}

void setOption(const set<char>&, const vector<char*>& arguments) {
    if(arguments.empty()) {
        logError("Wallpaper name expected");
        return;
    }

    applyWallpaper(arguments[0]);
}

void randomOption(const set<char>& subOptions, const vector<char*>& arguments) {
    const auto& wallpapers = Wallpapers::getWallpapers();

    if(wallpapers.empty()) {
        logInfo("No Wallpapers");
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
            logError("Array with include tags expected");
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
            logInfo("No wallpapers found");
            return;
        }

        const auto randomIndex = QRandomGenerator::global()->bounded(filteredWallpapers.size());
        applyWallpaper(filteredWallpapers[randomIndex].getName());
    } else {
        const auto randomIndex = QRandomGenerator::global()->bounded(wallpapers.size());
        applyWallpaper(wallpapers[randomIndex].getName());
    }
}

void listOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        logInfo(QJsonDocument(Wallpapers::toJson()));
    } else {
        for(const auto& wallpaper : Wallpapers::getWallpapers()) {
            logInfo(wallpaper.getName());
        }
    }
}

void OptionExecutor::execute(const int argc, char** argv) {
    const auto& option = argv[1][1];
    set<char> subOptions;

    if(argv[1][0] != '-') {
        logError("Unknown argument: {}", argv[1]);
        return;
    }

    if(strlen(argv[1]) < 2) {
        logError("Option not specified");
        return;
    }

    if(!options.contains(option)) {
        logError("Unknown option: {}", option);
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
                    logError("Unknown sub option: {}", argv[1][i]);
                    return;
                }
            }
        }
    }

    const vector arguments(argv + 2, argv + argc);

    options[option].func(subOptions, arguments);
}

map<char, OptionExecutor::Option> OptionExecutor::options = {
    {'H', {helpOption, {}, "WTF bro? You really need help with it?"}},
    {'h', {helpOption, {}, "Ha ha!"}}, // Because it's familiar
    {'V', {versionOption, {'j'}, versionHelpMessage}},
    {'S', {setOption, {}, setHelpMessage}},
    {'R', {randomOption, {'f'}, randomHelpMessage}},
    {'L', {listOption, {'j'}, listHelpMessage}},
};
