#include "gui/MainWindow.hpp"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include "Global.hpp"
#include "gui/QWallpaperGrid.hpp"

MainWindow::MainWindow() {
    auto *centralWidget = new QWidget(this);
    auto *centralWidgetLayout = new QVBoxLayout(centralWidget);

    auto *wallpaperGrid = new QWallpaperGrid(centralWidget);
    for(const auto &wallpaper: wallpapers) {
        wallpaperGrid->addPreview(new QWallpaperPreview(wallpaper));
    }

    auto *scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(wallpaperGrid);

    centralWidgetLayout->addWidget(scrollArea);

    setCentralWidget(centralWidget);
}

MainWindow &MainWindow::getInstance() {
    static MainWindow instance;
    return instance;
}
