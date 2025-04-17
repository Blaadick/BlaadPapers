#include <filesystem>
#include <fstream>
#include <QApplication>
#include <json/json.hpp>
#include "Defaults.hpp"
#include "Global.hpp"
#include "OptionExecutor.hpp"
#include "gui/MainWindow.hpp"

using namespace std;
using namespace filesystem;
using nlohmann::json;

void readConfig() {
    path configFilePath = configDir / "config.json";
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

int main(int argc, const char **argv) {
    readConfig();
    loadWallpapers();

    if(argc >= 2) {
        OptionExecutor::getInstance().execute(argc, argv);
        return 0;
    }

    QApplication app(argc, nullptr);
    MainWindow::getInstance().show();

    return QApplication::exec();
}
