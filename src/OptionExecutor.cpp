#include "OptionExecutor.hpp"

#include <fstream>
#include <iostream>
#include <random>
#include <Wallpaper.hpp>
#include <json/json.hpp>
#include "HelpStrings.hpp"
#include "Util.hpp"
#include "WallpaperManager.hpp"

using namespace std;
using nlohmann::json;

std::pmr::map<char, OptionExecutor::Option> OptionExecutor::options = {
    {'H', {help, {}, "WTF bro?"}},
    {'h', {help, {}, "WTF bro?"}}, // Because it's familiar
    {'V', {version, {'j'}, versionHelpMessage}},
    {'S', {set, {}, setHelpMessage}},
    {'R', {random, {'s'}, randomHelpMessage}},
    {'L', {list, {'j'}, listHelpMessage}},
};

void OptionExecutor::help(const pmr::set<char> &, const int argNumber, char *arguments[]) {
    if(argNumber > 2) {
        cerr << "Unknown argument: " << arguments[2] << endl;
        return;
    }

    cout << mainHelpMessage << endl;
}

void OptionExecutor::version(const pmr::set<char> &subOptions, const int argNumber, char *arguments[]) {
    if(argNumber > 2) {
        cerr << "Unknown argument: " << arguments[2] << endl;
        return;
    }

    if(subOptions.contains('j')) {
        cout << json {{"version", VERSION}} << endl;
    } else {
        cout << "Version: " << VERSION << endl;
    }
}

void OptionExecutor::set(const pmr::set<char> &, const int argNumber, char *arguments[]) {
    const auto &wallpapers = WallpaperManager::getWallpapers();
    const char *monitorName = arguments[2];
    const char *imageName = arguments[3];

    if(argNumber > 4) {
        cerr << "Unknown argument: " << arguments[4] << endl;
        return;
    }

    if(monitorName == nullptr) {
        cerr << "Monitor not provided" << endl;
        return;
    }

    if(imageName == nullptr) {
        cerr << "Wallpaper name not set" << endl;
        return;
    }

    if(wallpapers.empty()) {
        cerr << "No wallpaper found" << endl;
        return;
    }

    const auto wallpaperToSet = *ranges::find_if(wallpapers, [imageName](const Wallpaper &wallpaper){
        return wallpaper.getName() == imageName;
    });

    if(strcasecmp(wallpaperToSet.getName().c_str(), imageName) != 0) {
        cerr << "No wallpaper found" << endl;
        return;
    }

    setWallpaper(monitorName, wallpaperToSet);

    cout << "Wallpaper " << wallpaperToSet.getName() << " set" << endl;
}

void OptionExecutor::random(const pmr::set<char> &subOptions, const int argNumber, char *arguments[]) {
    const auto &wallpapers = WallpaperManager::getWallpapers();
    const char *monitorName = arguments[2];
    mt19937 rand(random_device {}());
    const Wallpaper *wallpaperToSet;

    if(monitorName == nullptr) {
        cerr << "Monitor not provided" << endl;
        return;
    }

    if(wallpapers.empty()) {
        cerr << "No wallpapers found" << endl;
        return;
    }

    if(subOptions.contains('s')) {
        vector<string> includeTags;
        vector<string> excludeTags;

        try {
            includeTags = json::parse(arguments[3]);

            if(arguments[4] != nullptr) {
                excludeTags = json::parse(arguments[4]);
            }
        } catch(...) {
            cerr << "Failed to parse passed tags" << endl;
            return;
        }

        if(argNumber > 5) {
            cerr << "Unknown argument: " << arguments[5] << endl;
            return;
        }

        pmr::set<const Wallpaper *> filteredWallpapers;

        for(const auto &wallpaper: wallpapers) {
            if(ranges::all_of(
                includeTags,
                [&wallpaper](const string &tag) {
                    auto wallpaperTags = wallpaper.getTags();
                    return ranges::find(wallpaperTags, tag) != wallpaperTags.end();
                }
            )) {
                if(!ranges::any_of(
                    excludeTags,
                    [&wallpaper](const string &tag) {
                        auto wallpaperTags = wallpaper.getTags();
                        return ranges::find(wallpaperTags, tag) != wallpaperTags.end();
                    }
                )) {
                    filteredWallpapers.insert(&wallpaper);
                }
            }
        }

        if(filteredWallpapers.empty()) {
            cerr << "No wallpaper found" << endl;
            return;
        }

        uniform_int_distribution randomDis(0, static_cast<int>(filteredWallpapers.size()) - 1);
        auto it = filteredWallpapers.begin();
        advance(it, randomDis(rand));
        wallpaperToSet = *it;
    } else {
        if(argNumber > 3) {
            cerr << "Unknown argument: " << arguments[3] << endl;
            return;
        }

        uniform_int_distribution randomDis(0, static_cast<int>(wallpapers.size()) - 1);
        auto it = wallpapers.begin();
        advance(it, randomDis(rand));
        wallpaperToSet = &*it;
    }

    setWallpaper(monitorName, *wallpaperToSet);
    cout << "Wallpaper " << wallpaperToSet->getName() << " set" << endl;
}

void OptionExecutor::list(const pmr::set<char> &subOptions, const int argNumber, char *arguments[]) {
    const auto &wallpapers = WallpaperManager::getWallpapers();

    if(argNumber > 2) {
        cerr << "Unknown argument: " << arguments[2] << endl;
        return;
    }

    if(wallpapers.empty()) {
        return;
    }

    if(subOptions.contains('j')) {
        cout << "[";

        int i = 0;
        for(const auto &wallpaper: wallpapers) {
            cout << wallpaper.toJson();

            if(++i != wallpapers.size()) {
                cout << ",";
            }
        }

        cout << "]" << endl;
    } else {
        for(const auto &wallpaper: wallpapers) {
            cout << wallpaper.getName() << endl;
        }
    }
}

void OptionExecutor::execute(const int argNumber, char *arguments[]) {
    const char &option = arguments[1][1];
    pmr::set<char> subOptions;

    if(arguments[1][0] != '-') {
        cerr << "Unknown argument: " << arguments[1] << endl;
        return;
    }

    if(strlen(arguments[1]) < 2) {
        cerr << "Option not specified" << endl;
        return;
    }

    if(!options.contains(option)) {
        cerr << "Unknown option: " << option << endl;
        return;
    }

    for(int i = 2; i < strlen(arguments[1]); i++) {
        switch(arguments[1][i]) {
        case 'h': {
            cout << options[option].helpMessage << endl;
            return;
        }
        case 'q': {
            //TODO supress output
            break;
        }
        default: {
            if(options[option].allowableSubOptions.contains(arguments[1][i])) {
                subOptions.insert(arguments[1][i]);
            } else {
                cerr << "Unknown sub option: " << arguments[1][i] << endl;
                return;
            }
        }
        }
    }

    options[option].func(subOptions, argNumber, arguments);
}
