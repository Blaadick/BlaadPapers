#include "WallpaperManager.hpp"

#include <filesystem>
#include <fstream>
#include "ConfigManager.hpp"
#include "Wallpaper.hpp"

using namespace std;
using namespace filesystem;
using nlohmann::json;

void WallpaperManager::loadWallpapers() {
    const path workingDir = ConfigManager::getWorkingDir();

    if(!exists(workingDir / ".index")) {
        create_directory(workingDir / ".index");
    }

    for(const auto& entry : directory_iterator(workingDir)) {
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
            dataFile << Wallpaper::defaultWallpaperData.dump(4);
            dataFile.close();

            data = Wallpaper::defaultWallpaperData;
        }

        wallpapers.push_back(Wallpaper(imageName, data));
    }
}

const std::vector<Wallpaper>& WallpaperManager::getWallpapers() {
    return wallpapers;
}

vector<Wallpaper> WallpaperManager::wallpapers;
