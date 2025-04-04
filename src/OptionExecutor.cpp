#include <fstream>
#include <iostream>
#include <random>

#include "OptionExecutor.hpp"
#include "Defaults.hpp"
#include "Global.hpp"

void setWallpaper(const Wallpaper &wallpaper) {
    //TODO move away
    system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + "\"").c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + "\"").c_str());
}

void OptionExecutor::help(const char **, const bool &) {
    cout << mainHelpMessage << endl;
}

void OptionExecutor::version(const char **, const bool &jsonOutput) {
    if(jsonOutput) {
        cout << json {{"version", VERSION}} << endl;
    } else {
        cout << "Version: " << VERSION << endl;
    }
}

void OptionExecutor::set(const char **arguments, const bool &) {
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

void OptionExecutor::random(const char **, const bool &) {
    random_device randomDevice;
    mt19937 rand(randomDevice());
    uniform_int_distribution randomDistribution(0, static_cast<int>(wallpapers.size()) - 1);
    const auto wallpaperToSet = next(wallpapers.begin(), randomDistribution(rand));

    setWallpaper(*wallpaperToSet);
    cout << "Wallpaper " << wallpaperToSet->getName() << " set" << endl;
}

void OptionExecutor::list(const char **, const bool &jsonOutput) {
    if(jsonOutput) {
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

void OptionExecutor::executeOption(const char **arguments) {
    const string arg = arguments[1];
    const char mainOption = arg[1];
    const string subOptions = arg.substr(2);
    bool isJsonOutput = false;

    for(const char &opt: subOptions) {
        switch(opt) {
            case 'h': {
                cout << options[mainOption].helpMessage << endl;
                return;
            }
            case 'q': {
                //TODO suppress program output
                break;
            }
            case 'j': {
                isJsonOutput = true;
                break;
            }
            default: {
                cout << "Unknown option: " << opt << endl;
                return;
            }
        }
    }

    if(options.contains(mainOption)) {
        options[mainOption].func(arguments, isJsonOutput);
    } else {
        cout << "Unknown main option: " << mainOption << endl;
    }
}
