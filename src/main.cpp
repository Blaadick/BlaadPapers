#include <fstream>
#include <iostream>
#include <vector>
#include <json/json.hpp>

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
    for(const auto &entry: directory_iterator(workingDir)) {
        if(!is_regular_file(entry)) continue;
        if(entry.path().extension() != ".png") continue;

        string imageName = entry.path().stem().string();
        path dataFilePath = entry.path().parent_path() / (imageName + ".json");
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

        wallpapers.emplace_back(imageName, data);
    }
}

int main(const int argc, const char *argv[]) {
    readConfig();
    loadWallpapers();

    for(int i = 1; i < argc; i++) {
        string arg = argv[i];

        if(arg == "-h" || arg == "--help") {
            cout << helpMessage << endl;
            return 0;
        }

        if(arg == "-v" || arg == "--version") {
            cout << "Version: " << VERSION << endl;
            return 0;
        }

        if(arg == "-l" || arg == "--list") {
            cout << "All available: " << VERSION << endl;
            return 0;
        }

        cout << "Unknown option: " << arg << endl;
    }

    for(const auto &wallpaper: wallpapers) {
        cout << "\"" << wallpaper.getName() << "\": " << wallpaper.toJson().dump(4) << endl;
    }
}
