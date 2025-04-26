#include "ConfigReader.hpp"

#include <fstream>

using namespace std;
using namespace filesystem;
using nlohmann::json;

void ConfigReader::readConfig() {
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

path ConfigReader::getWorkingDir() {
    return workingDir;
}
