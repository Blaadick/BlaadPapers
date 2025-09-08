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
            {"name", "BlaadPapers"},
            {"version", PROJECT_VERSION}
        };

        cout << QJsonDocument(outputData).toJson(QJsonDocument::Compact) << endl;
    } else {
        cout << PROJECT_VERSION << endl;
    }
}

void setOption(const set<char>&, const vector<char*>& arguments) {
    const auto& wallpapers = Wallpapers::getWallpapers();
    const auto& targetWallpaperName = arguments[0];

    if(wallpapers.empty()) {
        cerr << "No wallpapers found" << endl;
        return;
    }

    if(targetWallpaperName == nullptr) {
        cerr << "Wallpaper name expected" << endl;
        return;
    }

    for(const auto& wallpaper : wallpapers) {
        if(wallpaper.getName() == targetWallpaperName) {
            applyWallpaper(wallpaper.getPicturePath());
            return;
        }
    }

    cerr << "Wallpaper " << targetWallpaperName << " not found" << endl;
}

void randomOption(const set<char>& subOptions, const vector<char*>& arguments) {
    const auto& wallpapers = Wallpapers::getWallpapers();

    if(wallpapers.empty()) {
        cerr << "No wallpapers found" << endl;
        return;
    }

    if(subOptions.contains('f')) {
        QVector<QString> includeTags;
        QVector<QString> excludeTags;
        QVector<Wallpaper> filteredWallpapers;

        if(arguments[0] != nullptr) {
            for(auto tag : QJsonDocument::fromJson(arguments[0]).array()) {
                includeTags.append(tag.toString());
            }
        } else {
            cerr << "Array with include tags expected" << endl;
            return;
        }

        if(arguments[1] != nullptr) {
            for(auto tag : QJsonDocument::fromJson(arguments[1]).array()) {
                excludeTags.append(tag.toString());
            }
        }

        for(const auto& wallpaper : wallpapers) {
            if(
                ranges::all_of(
                    includeTags,
                    [&wallpaper](const QString& tag) {
                        return wallpaper.getTags().contains(tag);
                    }
                )
            ) {
                if(
                    !ranges::any_of(
                        excludeTags,
                        [&wallpaper](const QString& tag) {
                            return wallpaper.getTags().contains(tag);
                        }
                    )
                ) {
                    filteredWallpapers.emplace_back(wallpaper);
                }
            }
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
