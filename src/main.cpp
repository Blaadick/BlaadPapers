#include <filesystem>
#include <fstream>
#include <json/json.hpp>

#include "Defaults.hpp"
#include "Global.hpp"
#include "OptionExecutor.hpp"

void readConfig() {
    std::filesystem::path configFilePath = std::string(getenv("HOME")) + "/.config/blaadpapers.json";
    nlohmann::json configData;

    if(exists(configFilePath)) {
        std::ifstream configFile(configFilePath);
        configFile >> configData;
        configFile.close();
    } else {
        configData = defaultConfig;
    }

    std::string rawWorkingDir = configData["working_dir"];

    if(rawWorkingDir[0] == '~') {
        workingDir = std::string(getenv("HOME")) + rawWorkingDir.substr(1);
    } else {
        workingDir = rawWorkingDir;
    }
}

void loadWallpapers() {
    if(!exists(workingDir / ".index")) {
        create_directory(workingDir / ".index");
    }

    for(const auto &entry: std::filesystem::directory_iterator(workingDir)) {
        if(entry.path().extension() != ".png") continue;

        std::string imageName = entry.path().stem();
        std::filesystem::path dataFilePath = workingDir / ".index" / (imageName + ".json");
        nlohmann::json data;

        if(exists(dataFilePath)) {
            std::ifstream dataFile(dataFilePath);
            dataFile >> data;
            dataFile.close();
        } else {
            std::ofstream dataFile(dataFilePath);
            dataFile << defaultWallpaperData.dump(4);
            dataFile.close();

            data = defaultWallpaperData;
        }

        wallpapers.emplace(imageName, data);
    }
}

int main(const int argc, const char **argv) {
    readConfig();
    loadWallpapers();

    if(argc >= 2 && argv[1][0] == '-') {
        OptionExecutor executor;

        executor.executeOption(argv);
    }
}
