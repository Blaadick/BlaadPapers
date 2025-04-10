#include <filesystem>
#include <fstream>
#include <QApplication>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QScreen>
#include <QSplitter>
#include <QVBoxLayout>
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

QSize getAspectRatio(const QRect &geometry) {
    const int divisor = std::gcd(geometry.width(), geometry.height());
    return {geometry.width() / divisor, geometry.height() / divisor};
}

int main(int argc, const char **argv) {
    readConfig();
    loadWallpapers();

    if(argc >= 2) {
        OptionExecutor::getInstance().execute(argv);
    } else {
        QApplication app(argc, nullptr);
        QMainWindow mainWindow;

        for(const auto screen: QGuiApplication::screens()) {
            qDebug() << screen->name() << ":";
            qDebug() << "  Physical Size:" << screen->physicalSize().width() << "x" << screen->physicalSize().height() << "mm";
            qDebug() << "  Pixel Size:" << screen->geometry().width() << "x" << screen->geometry().height();
        }

        const auto splitter = new QSplitter();
        const auto wallpaperDetails = new QListWidget();
        wallpaperDetails->setFixedWidth(300);

        const auto topBatWidget = new QWidget();
        const auto topBarLayout = new QHBoxLayout(topBatWidget);
        const auto searchBox = new QLineEdit();
        const auto monitorCombo = new QComboBox();
        topBarLayout->addWidget(monitorCombo);
        topBarLayout->addWidget(searchBox);

        const auto rightWidget = new QWidget();
        const auto mainLayout = new QVBoxLayout(rightWidget);
        const auto wallpaperList = new QGridLayout();

        wallpaperList->setSpacing(10);
        wallpaperList->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(topBatWidget, 0, Qt::AlignTop);
        mainLayout->addLayout(wallpaperList, 0);

        splitter->addWidget(wallpaperDetails);
        splitter->addWidget(rightWidget);

        splitter->setChildrenCollapsible(false);

        mainWindow.setCentralWidget(splitter);

        int i = 0;
        for(const auto &wallpaper: wallpapers) {
            if(i >= 10) break;

            const auto wallpaperWidget = new QPushButton(QIcon(wallpaper.getFilePath().c_str()), wallpaper.getName().c_str());

            wallpaperWidget->setFixedSize(getAspectRatio(QGuiApplication::screens()[0]->geometry()) * 10);

            wallpaperList->addWidget(wallpaperWidget, i / 4, i % 4);

            i++;
        }

        mainWindow.show();

        return QApplication::exec();
    }
}
