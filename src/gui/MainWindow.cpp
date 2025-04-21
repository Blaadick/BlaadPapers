#include "gui/MainWindow.hpp"

#include <QScreen>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include "Global.hpp"

MainWindow &MainWindow::getInstance() {
    static MainWindow instance;
    return instance;
}

void MainWindow::fillWidgets() const {
    for(const auto &wallpaper: wallpapers) {
        wallpaperGrid->addPreview(new QWallpaperPreview(wallpaper, wallpaperGrid));
    }
}

MainWindow::MainWindow() {
    auto *centralWidget = new QWidget(this);

    auto *centralWidgetLayout = new QVBoxLayout(centralWidget);
    centralWidgetLayout->setContentsMargins(10, 10, 10, 10);
    centralWidgetLayout->setSpacing(10);
    setCentralWidget(centralWidget);

    auto *topLayout = new QHBoxLayout();
    topLayout->setSpacing(10);
    centralWidgetLayout->addLayout(topLayout);

    monitorCombo = new QComboBox(centralWidget);
    topLayout->addWidget(monitorCombo);

    searchBox = new QLineEdit(centralWidget);
    topLayout->addWidget(searchBox);

    wallpaperGrid = new QWallpaperGrid(centralWidget);

    auto *scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(wallpaperGrid);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    centralWidgetLayout->addWidget(scrollArea);
}
