#include <fstream>
#include <iostream>
#include <json/json.hpp>
#include <random>

#include "Defaults.hpp"
#include "Global.hpp"
#include "Wallpaper.hpp"

using namespace std;
using namespace nlohmann;
using namespace filesystem;

void readConfig() {
    path configFilePath = string(getenv("HOME")) + "/.config/blaadpapers.json";
    json configData;

    if(exists(configFilePath)) {
        ifstream configFile(configFilePath);
        configFile >> configData;
        configFile.close();
    } else {
        configData = defaultConfig;
    }

    string rawWorkingDir = configData["working_dir"];

    if(rawWorkingDir[0] == '~') {
        workingDir = string(getenv("HOME")) + rawWorkingDir.substr(1);
    } else {
        workingDir = rawWorkingDir;
    }
}

void loadWallpapers() {
    if(!exists(workingDir / ".index")) {
        create_directory(workingDir / ".index");
    }

    for(const auto &entry: directory_iterator(workingDir)) {
        if(entry.path().extension() != ".png") continue;

        string imageName = entry.path().stem();
        path dataFilePath = workingDir / ".index" / (imageName + ".json");
        json data;

        if(exists(dataFilePath)) {
            ifstream dataFile(dataFilePath);
            dataFile >> data;
            dataFile.close();
        } else {
            ofstream dataFile(dataFilePath);
            dataFile << defaultWallpaperData.dump(4);
            dataFile.close();

            data = defaultWallpaperData;
        }

        wallpapers.emplace(imageName, data);
    }
}

int main(const int argc, const char *argv[]) {
    readConfig();
    loadWallpapers();

    const string arg = argv[1];

    if(arg == "-h" || arg == "--help") {
        cout << helpMessage << endl;
        return 0;
    }

    if(arg == "-v" || arg == "--version") {
        cout << "Version: " << VERSION << endl;
        return 0;
    }

    if(arg == "-S" || arg == "--set") {
        const char *imageName = argv[2];

        if(imageName == nullptr) {
            cout << helpMessage << endl;
            return 0;
        }

        const Wallpaper temp(imageName, defaultWallpaperData);
        const auto iterator = wallpapers.lower_bound(temp);
        const Wallpaper *wallpaperToSet = nullptr;

        if(strcasecmp(iterator->getName().c_str(), imageName) == 0) {
            wallpaperToSet = &*iterator;
        }

        if(wallpaperToSet == nullptr) {
            cout << "No wallpaper found" << endl;
        } else {
            cout << "Wallpaper " << wallpaperToSet->getName() << " set" << endl;
        }

        return 0;
    }

    if(arg == "-R" || arg == "--random") {
        random_device randomDevice;
        mt19937 rand(randomDevice());
        uniform_int_distribution randomDistribution(0, static_cast<int>(wallpapers.size()) - 1);

        const auto iterator = next(wallpapers.begin(), randomDistribution(rand));
        const Wallpaper *wallpaperToSet = &*iterator;

        cout << "Wallpaper " << wallpaperToSet->getName() << " set" << endl;

        return 0;
    }

    if(arg == "-l" || arg == "--list") {
        cout << "All available wallpapers: " << endl;

        for(const auto &wallpaper: wallpapers) {
            cout << wallpaper.getName() << endl;
        }

        return 0;
    }

    if(arg == "-L" || arg == "--list-json") {
        cout << "All available wallpapers:" << endl;

        for(const auto &wallpaper: wallpapers) {
            cout << "\"" << wallpaper.getName() << "\": " << wallpaper.toJson().dump(4) << endl;
        }

        return 0;
    }

    cout << "Unknown option: " << arg << endl;
}
