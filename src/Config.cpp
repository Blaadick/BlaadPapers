#include "Config.hpp"

#include <fstream>
#include <iostream>

using namespace std;
using namespace filesystem;
using nlohmann::json;

void Config::readConfig() {
    path configFilePath = configDir / "config.json";

    if(exists(configFilePath)) {
        ifstream configFile(configFilePath);
        configFile >> configData;
        configFile.close();
    } else {
        ofstream configFile(configFilePath);
        configFile << defaultConfigData.dump(4);
        configFile.close();

        configData = defaultConfigData;
    }

    workingDir = readDirProperty("working_dir");
    badTags = readProperty<vector<string>>("bad_tags");
}

const path& Config::getWorkingDir() {
    return workingDir;
}

const std::vector<std::string>& Config::getBadTags() {
    return badTags;
}

path Config::getPostSetScriptPath() {
    return configDir / "post_set.sh";
}

json Config::configData;
path Config::workingDir;
vector<string> Config::badTags;

path Config::readDirProperty(const std::string& name) {
    const auto rawDir = readProperty<string>(name);
    return rawDir.starts_with('~') ? string(getenv("HOME")) + rawDir.substr(1) : rawDir;
}

template<typename T>
T Config::readProperty(const std::string& name) {
    const auto value = configData[name];
    return value.is_null() ? defaultConfigData[name] : value;
}
