#include "gui/MainWindow.hpp"

#include <QListWidget>
#include <QScreen>

#include "Global.hpp"
#include "Util.hpp"
#include "gui/QWallpaperGrid.hpp"
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

    const auto wallpaperGrid = new QWallpaperGrid(ui->scrollArea);
    for(const auto &wallpaper: wallpapers) {
        wallpaperGrid->addPreview(new QWallpaperPreview(wallpaper, ui->scrollArea));
    }
    ui->scrollArea->setWidget(wallpaperGrid);
}

MainWindow &MainWindow::getInstance() {
    static MainWindow instance;
    return instance;
}
