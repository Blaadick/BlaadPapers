#include <filesystem>
#include <fstream>
#include <QApplication>
#include <json/json.hpp>
#include "Defaults.hpp"
#include "Global.hpp"
#include "OptionExecutor.hpp"
#include "Util.hpp"
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

void loadCache() {
    if(!exists(cacheDir)) {
        create_directory(cacheDir);
    }

    for(const auto *screen: QApplication::screens()) {
        path screenCacheFolder = cacheDir / to_string(screen->devicePixelRatio());

        if(!exists(screenCacheFolder)) {
            create_directory(screenCacheFolder);
        }

        for(const auto &wallpaper: wallpapers) {
            path cachedPreviewPath = wallpaper.getPreviewPath(screen->devicePixelRatio());

            if(!exists(cachedPreviewPath)) {
                QImage maxSizedPreview = QImage(wallpaper.getFilePath().c_str()).scaled(
                    getAspectRatio(screen) * 18.4 * screen->devicePixelRatio(),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation
                );

                if(maxSizedPreview.save(cachedPreviewPath.c_str(), "PNG")) {
                    qDebug() << wallpaper.getName() << "with size" << screen->devicePixelRatio() << "preview caching success";
                } else {
                    qDebug() << wallpaper.getName() << "with size" << screen->devicePixelRatio() << "preview caching fail";
                }
            }
        }
    }
}

int main(int argc, const char *argv[]) {
    readConfig();
    loadWallpapers();

    if(argc >= 2) {
        OptionExecutor::getInstance().execute(argc, argv);
        return 0;
    }

    QApplication app(argc, nullptr);

    loadCache();

    MainWindow::getInstance().show();
    MainWindow::getInstance().fillWidgets();

    return QApplication::exec();
}
