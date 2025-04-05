#include <iostream>
#include <random>
#include <Wallpaper.hpp>
#include <json/json.hpp>

#include "OptionExecutor.hpp"
#include "Defaults.hpp"
#include "Global.hpp"

using namespace std;
using nlohmann::json;

void setWallpaper(const Wallpaper &wallpaper) {
    //TODO move away
    system("hyprctl -q hyprpaper unload all");
    system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + "\"").c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + "\"").c_str());
}

OptionExecutor::OptionExecutor() {
    options['H'] = {help, {}, "WTF bro?"};
    options['h'] = {help, {}, "WTF bro?"}; // Because it's familiar
    options['V'] = {version, {'j'}, versionHelpMessage};
    options['S'] = {set, {}, setHelpMessage};
    options['R'] = {random, {'f'}, randomHelpMessage};
    options['L'] = {list, {'j'}, listHelpMessage};
}

void OptionExecutor::help(const pmr::set<char> &, const char **) {
    cout << mainHelpMessage << endl;
}

void OptionExecutor::version(const pmr::set<char> &subOptions, const char **) {
    if(subOptions.contains('j')) {
        cout << json {{"version", VERSION}} << endl;
    } else {
        cout << "Version: " << VERSION << endl;
    }
}

void OptionExecutor::set(const pmr::set<char> &, const char **arguments) {
    const char *imageName = arguments[2];

    if(imageName == nullptr) {
        cout << "Wallpaper name not set!" << endl;
        return;
    }

    const Wallpaper temp(imageName, defaultWallpaperData);
    const auto &wallpaperToSet = *wallpapers.lower_bound(temp);

    if(strcasecmp(wallpaperToSet.getName().c_str(), imageName) != 0) {
        cout << "No wallpaper found" << endl;
        return;
    }

    setWallpaper(wallpaperToSet);

    cout << "Wallpaper " << wallpaperToSet.getName() << " set" << endl;
}

void OptionExecutor::random(const pmr::set<char> &subOptions, const char **arguments) {
    mt19937 rand(random_device {}());
    const Wallpaper *wallpaperToSet;

    if(subOptions.contains('f')) {
        const vector<string> includeTags = json::parse(arguments[2]);
        pmr::set<const Wallpaper *> filteredWallpapers;

        for(const auto &wallpaper: wallpapers) {
            if(ranges::all_of(
                includeTags,
                [&wallpaper](const string &tag) {
                    auto wallpaperTags = wallpaper.getTags();
                    return ranges::find(wallpaperTags, tag) != wallpaperTags.end();
                }
            )) {
                filteredWallpapers.insert(&wallpaper);
            }
        }

        if(filteredWallpapers.empty()) {
            cout << "No Wallpaper found" << endl;
            return;
        }

        uniform_int_distribution<> randomDis(0, filteredWallpapers.size() - 1);
        auto it = filteredWallpapers.begin();
        advance(it, randomDis(rand));

        wallpaperToSet = *it;
    } else {
        uniform_int_distribution<> randomDis(0, wallpapers.size() - 1);
        auto it = wallpapers.begin();
        advance(it, randomDis(rand));

        wallpaperToSet = &*it;
    }

    setWallpaper(*wallpaperToSet);
    cout << "Wallpaper " << wallpaperToSet->getName() << " set" << endl;
}

void OptionExecutor::list(const pmr::set<char> &subOptions, const char **) {
    if(subOptions.contains('j')) {
        cout << "{";

        for(const auto &wallpaper: wallpapers) {
            cout << wallpaper.toJson() << ",";
        }

        cout << "\b" << "}" << endl;
    } else {
        for(const auto &wallpaper: wallpapers) {
            cout << wallpaper.getName() << endl;
        }
    }
}

OptionExecutor &OptionExecutor::getInstance() {
    static OptionExecutor instance;
    return instance;
}

void OptionExecutor::executeOption(const char **arguments) {
    const char &mainOption = arguments[1][1];
    pmr::set<char> subOptions;

    if(arguments[1][0] != '-') {
        cout << "Unknown argument: " << arguments[1] << endl;
        return;
    }

    if(strlen(arguments[1]) < 2) {
        cout << "Main option not specified" << endl;
        return;
    }

    if(!options.contains(mainOption)) {
        cout << "Unknown main option: " << mainOption << endl;
        return;
    }

    for(int i = 2; i < strlen(arguments[1]); i++) {
        switch(arguments[1][i]) {
            case 'h': {
                cout << options[mainOption].helpMessage << endl;
                return;
            }
            case 'q': {
                //TODO supress output
                break;
            }
            default: {
                if(options[mainOption].allowableSubOptions.contains(arguments[1][i])) {
                    subOptions.insert(arguments[1][i]);
                } else {
                    cout << "Unknown sub option: " << arguments[1][i] << endl;
                    return;
                }
            }
        }
    }

    options[mainOption].func(subOptions, arguments);
}
