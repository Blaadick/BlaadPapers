#include <filesystem>
#include <fstream>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <json/json.hpp>

#include "Defaults.hpp"
#include "Global.hpp"
#include "OptionExecutor.hpp"

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
        OptionExecutor::getInstance().execute(argv);
    } else {
        QApplication app(argc, nullptr);
        QWidget window;

        QGridLayout grid(&window);
        grid.setSpacing(10);

        QPushButton btn00("00", &window);
        QPushButton btn01("01", &window);
        QPushButton btn02("02", &window);
        QPushButton btn10("10", &window);
        QPushButton btn11("11", &window);
        QPushButton btn12("12", &window);
        QPushButton btn20("20", &window);
        QPushButton btn21("21", &window);
        QPushButton btn22("22", &window);

        const auto buttons = {&btn00, &btn01, &btn02, &btn10, &btn11, &btn12, &btn20, &btn21, &btn22};

        for(QPushButton *button: buttons) {
            button->setFixedSize(50, 50);
        }

        grid.addWidget(&btn00, 0, 0);
        grid.addWidget(&btn01, 0, 1);
        grid.addWidget(&btn02, 0, 2);

        grid.addWidget(&btn10, 1, 0);
        grid.addWidget(&btn11, 1, 1);
        grid.addWidget(&btn12, 1, 2);

        grid.addWidget(&btn20, 2, 0);
        grid.addWidget(&btn21, 2, 1);
        grid.addWidget(&btn22, 2, 2);

        window.show();

        return QApplication::exec();
    }
}
