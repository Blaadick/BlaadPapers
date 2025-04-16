#include "gui/MainWindow.hpp"

#include <QListWidget>
#include <QScreen>

#include "Global.hpp"
#include "Util.hpp"
#include "gui/QWallpaperPreview.hpp"
#include "gui/ui_MainWindow.h"

MainWindow::MainWindow() : ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->monitorCombo->hide();
    if(QApplication::screens().count() > 1) {
        ui->monitorCombo->show();

        for(const auto *screen: QGuiApplication::screens()) {
            ui->monitorCombo->addItem(screen->name());
        }
    }

    int i = 0;
    for(const auto &wallpaper: wallpapers) {
        const auto wallpaperPreview = new QWallpaperPreview(wallpaper);
        ui->wallpaperGridLayout->addWidget(wallpaperPreview, i / 5, i % 5);
        i++;
    }
}

MainWindow &MainWindow::getInstance() {
    static MainWindow instance;
    return instance;
}
