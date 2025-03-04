#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "Strings.hpp"
#include "Wallpaper.hpp"

using namespace std;
namespace fs = filesystem;

fs::path workingDir;
vector<Wallpaper> wallpapers;

void readConfig() {
    fs::path configPath = string(getenv("HOME")) + "/.config/blaadpapers.json";
    json configData;

    if(exists(configPath)) {
        ifstream configFile(configPath);

        configFile >> configData;
        configFile.close();
    } else {
        ofstream configFile(configPath);
        configData = {
            {"working_dir", "/home/blaadick/Pictures/test/"}
        };

        configFile << configData.dump(4);
        configFile.close();
    }

    workingDir = configData["working_dir"].get<string>();
}

void loadWallpapers() {
    for(const auto &entry: fs::directory_iterator(workingDir)) {
        if(!is_regular_file(entry)) continue;

        string imageFormat = entry.path().extension().string().substr(1);
        string name = entry.path().stem().string();
        json data;
        bool dataFound = false;

        if(imageFormat != "png") continue;

        for(const auto &entry2: fs::directory_iterator(workingDir)) {
            if(entry2.path().filename().string() != name + ".json") continue;

            ifstream file(entry2.path().string());

            dataFound = true;
            file >> data;

            break;
        }

        if(!dataFound) {
            data = {
                {"description", ""},
                {"tags", {"SFW"}}
            };

            ofstream file(entry.path().parent_path() / string(name + ".json"));
            file << data.dump(4);
            file.close();
        }

        wallpapers.emplace_back(name, data);
    }
}

int main(const int argc, const char *argv[]) {
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

        cout << "Unknown option: " << arg << endl;
    }

    readConfig();
    loadWallpapers();

    for(auto &wallpaper: wallpapers) {
        cout << "\"" << wallpaper.getName() << "\": " << wallpaper.serialize().dump(4) << endl;
    }
}
