#include <iostream>
#include <random>
#include <Wallpaper.hpp>
#include <json/json.hpp>

#include "OptionExecutor.hpp"
#include "Defaults.hpp"
#include "Global.hpp"

void setWallpaper(const Wallpaper &wallpaper) {
    //TODO move away
    system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + "\"").c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + "\"").c_str());
}

void OptionExecutor::help(const std::pmr::set<char> &, const char **) {
    std::cout << mainHelpMessage << std::endl;
}

void OptionExecutor::version(const std::pmr::set<char> &subOptions, const char **) {
    if(subOptions.contains('j')) {
        std::cout << nlohmann::json {{"version", VERSION}} << std::endl;
    } else {
        std::cout << "Version: " << VERSION << std::endl;
    }
}

void OptionExecutor::set(const std::pmr::set<char> &, const char **arguments) {
    const char *imageName = arguments[2];

    if(imageName == nullptr) {
        std::cout << "Wallpaper name not set!" << std::endl;
        return;
    }

    const Wallpaper temp(imageName, defaultWallpaperData);
    const auto wallpaperToSet = wallpapers.lower_bound(temp);

    if(strcasecmp(wallpaperToSet->getName().c_str(), imageName) != 0) {
        std::cout << "No wallpaper found" << std::endl;
        return;
    }

    setWallpaper(*wallpaperToSet);

    std::cout << "Wallpaper " << wallpaperToSet->getName() << " set" << std::endl;
}

void OptionExecutor::random(const std::pmr::set<char> &, const char **) {
    std::random_device randomDevice;
    std::mt19937 rand(randomDevice());
    std::uniform_int_distribution randomDistribution(0, static_cast<int>(wallpapers.size()) - 1);
    const auto wallpaperToSet = std::next(wallpapers.begin(), randomDistribution(rand));

    setWallpaper(*wallpaperToSet);
    std::cout << "Wallpaper " << wallpaperToSet->getName() << " set" << std::endl;
}

void OptionExecutor::list(const std::pmr::set<char> &subOptions, const char **) {
    if(subOptions.contains('j')) {
        std::cout << "{";

        for(const auto &wallpaper: wallpapers) {
            std::cout << wallpaper.toJson() << ",";
        }

        std::cout << "\b" << "}" << std::endl;
    } else {
        for(const auto &wallpaper: wallpapers) {
            std::cout << wallpaper.getName() << std::endl;
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
    std::pmr::set<char> subOptions;

    for(int i = 2; i <= strlen(arguments[1]) - 1; i++) {
        switch(arguments[1][i]) {
            case 'h': {
                std::cout << options[mainOption].helpMessage << std::endl;
                return;
            }
            case 'q': {
                //TODO supress output
                std::cout << "Be Quiet!" << std::endl;
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
        std::cout << "Unknown main option: " << mainOption << std::endl;
    }
}
