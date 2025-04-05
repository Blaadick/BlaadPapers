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
    system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + "\"").c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + "\"").c_str());
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
    const auto wallpaperToSet = wallpapers.lower_bound(temp);

    if(strcasecmp(wallpaperToSet->getName().c_str(), imageName) != 0) {
        cout << "No wallpaper found" << endl;
        return;
    }

    setWallpaper(*wallpaperToSet);

    cout << "Wallpaper " << wallpaperToSet->getName() << " set" << endl;
}

void OptionExecutor::random(const pmr::set<char> &, const char **) {
    random_device randomDevice;
    mt19937 rand(randomDevice());
    uniform_int_distribution randomDistribution(0, static_cast<int>(wallpapers.size()) - 1);
    const auto wallpaperToSet = next(wallpapers.begin(), randomDistribution(rand));

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

OptionExecutor::OptionExecutor() {
    options['H'] = {help, helpHelpMessage};
    options['V'] = {version, versionHelpMessage};
    options['S'] = {set, setHelpMessage};
    options['R'] = {random, randomHelpMessage};
    options['L'] = {list, listHelpMessage};
}

void OptionExecutor::executeOption(const char **arguments) {
    const char &mainOption = arguments[1][1];
    pmr::set<char> subOptions;

    for(int i = 2; i <= strlen(arguments[1]) - 1; i++) {
        switch(arguments[1][i]) {
            case 'h': {
                cout << options[mainOption].helpMessage << endl;
                return;
            }
            case 'q': {
                //TODO supress output
                cout << "Be Quiet!" << endl;
                break;
            }
            default: {
                subOptions.insert(arguments[1][i]);
                break;
            }
        }
    }

    if(options.contains(mainOption)) {
        options[mainOption].func(subOptions, arguments);
    } else {
        cout << "Unknown main option: " << mainOption << endl;
    }
}
