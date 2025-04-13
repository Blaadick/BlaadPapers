#include "gui/MainWindow.hpp"

#include <QListWidget>
#include <QScreen>

#include "Global.hpp"
#include "Util.hpp"
#include "gui/QWallpaperPreview.hpp"
#include "gui/ui_MainWindow.h"

MainWindow::MainWindow() : ui(new Ui::MainWindow) {
    selectedScreen = QGuiApplication::screens()[0];

    ui->setupUi(this);

    int i = 0;
    for(const auto &wallpaper: wallpapers) {
        if(i >= 35) break;

        const auto wallpaperPreview = new QWallpaperPreview(selectedScreen, wallpaper);

        ui->wallpaperGrid->addWidget(wallpaperPreview, i / 5, i % 5);

        connect(
            wallpaperPreview,
            &QAbstractButton::clicked,
            [&] {
                setWallpaper(wallpaper);
            }
        );

        i++;
    }

    for(const auto *screen: QGuiApplication::screens()) {
        ui->comboBox->addItem(screen->name());
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
