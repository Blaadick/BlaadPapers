#include "Config.hpp"

#include <fstream>

using namespace std;
using namespace filesystem;
using nlohmann::json;

void Config::readConfig() {
    path configFilePath = configDir / "config.json";
    json configData;

    if(exists(configFilePath)) {
        ifstream configFile(configFilePath);
        configFile >> configData;
        configFile.close();
    } else {
        configData = defaultConfig;
    }

    const string rawWorkingDir = configData["working_dir"];

    if(rawWorkingDir[0] == '~') {
        workingDir = string(getenv("HOME")) + rawWorkingDir.substr(1);
    } else {
        workingDir = rawWorkingDir;
    }
}

path Config::getWorkingDir() {
    return workingDir;
}

path Config::getPostSetScriptPath() {
    return configDir / "post_set.sh";
}

const path Config::configDir = std::string(getenv("HOME")) + "/.config/blaadpapers/";
const json Config::defaultConfig = {
    {"working_dir", "~/Pictures/Wallpapers/"}
};
path Config::workingDir;
