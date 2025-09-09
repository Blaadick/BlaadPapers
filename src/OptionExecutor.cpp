#include "OptionExecutor.hpp"

#include <cstring>
#include <iostream>
#include <QRandomGenerator>
#include "HelpStrings.hpp"
#include "Wallpapers.hpp"
#include "model/WallpaperList.hpp"
#include "util/Operators.hpp"
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

        cout << QJsonDocument(outputData).toJson(QJsonDocument::Compact) << endl;
    } else {
        cout << PROJECT_VERSION << endl;
    }
}

void setOption(const set<char>&, const vector<char*>& arguments) {
    const auto& wallpapers = Wallpapers::getWallpapers();

    if(arguments.size() < 1) {
        cerr << "Wallpaper name expected" << endl;
        return;
    }

    for(const auto& wallpaper : wallpapers) {
        if(wallpaper.getName() == arguments[0]) {
            applyWallpaper(wallpaper.getPicturePath());
            return;
        }
    }

    cerr << "Wallpaper not found" << endl;
}

void randomOption(const set<char>& subOptions, const vector<char*>& arguments) {
    const auto& wallpapers = Wallpapers::getWallpapers();

    if(wallpapers.empty()) {
        cerr << "No wallpapers" << endl;
        return;
    }

    if(subOptions.contains('f')) {
        QVector<QString> includeTags;
        QVector<QString> excludeTags;
        QVector<Wallpaper> filteredWallpapers;

        if(arguments.size() > 0) {
            for(auto tag : QJsonDocument::fromJson(arguments[0]).array()) {
                includeTags.append(tag.toString());
            }
        } else {
            cerr << "Array with include tags expected" << endl;
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

        for(const auto& wallpaper : filteredWallpapers) {
            qDebug() << "Filtered: " << wallpaper.getName();
        }

        if(filteredWallpapers.empty()) {
            cout << "No wallpapers found" << endl;
            return;
        }

        const auto randomIndex = QRandomGenerator::global()->bounded(filteredWallpapers.size());
        applyWallpaper(filteredWallpapers[randomIndex].getPicturePath());
    } else {
        const auto randomIndex = QRandomGenerator::global()->bounded(wallpapers.size());
        applyWallpaper(wallpapers[randomIndex].getPicturePath());
    }
}

void listOption(const set<char>& subOptions, const vector<char*>&) {
    if(subOptions.contains('j')) {
        cout << QJsonDocument(Wallpapers::toJson()).toJson(QJsonDocument::Compact) << endl;
    } else {
        for(const auto& wallpaper : Wallpapers::getWallpapers()) {
            cout << wallpaper.getName() << endl;
        }
    }
}

void OptionExecutor::execute(const int argc, char** argv) {
    const auto& option = argv[1][1];
    set<char> subOptions;

    if(argv[1][0] != '-') {
        cerr << "Unknown argument: " << argv[1] << endl;
        return;
    }

    if(strlen(argv[1]) < 2) {
        cerr << "Option not specified" << endl;
        return;
    }

    if(!options.contains(option)) {
        cerr << "Unknown option: " << option << endl;
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
                    cerr << "Unknown sub option: " << argv[1][i] << endl;
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
