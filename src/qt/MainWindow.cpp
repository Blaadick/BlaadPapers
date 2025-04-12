#include "qt/MainWindow.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QScreen>
#include <QVBoxLayout>

#include "Global.hpp"
#include "qt/QWallpaperPreview.hpp"

QScreen *MainWindow::getSelectedScreen() {
    return selectedScreen;
}

MainWindow &MainWindow::getInstance() {
    static MainWindow instance;
    return instance;
}

MainWindow::MainWindow() {
    selectedScreen = QGuiApplication::screens()[0];

    const auto mainWidget = new QWidget;
    const auto mainLayout = new QVBoxLayout(mainWidget);

    const auto topBar = new QWidget;
    const auto topBarLayout = new QHBoxLayout(topBar);
    const auto monitorCombo = new QComboBox;
    const auto searchBox = new QLineEdit;
    topBarLayout->setContentsMargins(0, 0, 0, 10);
    topBarLayout->addWidget(monitorCombo);
    topBarLayout->addWidget(searchBox);
    mainLayout->addWidget(topBar, 0, Qt::AlignTop);

    const auto wallpaperList = new QWidget;
    const auto wallpaperListLayout = new QGridLayout(wallpaperList);
    wallpaperListLayout->setSpacing(10);
    wallpaperListLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(wallpaperList, 1, Qt::AlignTop | Qt::AlignHCenter);

    this->setCentralWidget(mainWidget);

    int i = 0;
    for(const auto &wallpaper: wallpapers) {
        if(i >= 35) break;

        const auto wallpaperWidget = new QWallpaperPreview(wallpaper);

        wallpaperListLayout->addWidget(wallpaperWidget, i / 5, i % 5);

        i++;
    }

    for(const auto *screen: QGuiApplication::screens()) {
        monitorCombo->addItem(screen->name());
    }
}
