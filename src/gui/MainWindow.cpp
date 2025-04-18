#include "gui/MainWindow.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include "Global.hpp"
#include "gui/QWallpaperGrid.hpp"

int margin = 10;

MainWindow::MainWindow() {
    auto *centralWidget = new QWidget(this);

    auto *centralWidgetLayout = new QVBoxLayout(centralWidget);
    centralWidgetLayout->setContentsMargins(margin, margin, margin, margin);
    centralWidgetLayout->setSpacing(margin);
    setCentralWidget(centralWidget);

    auto *topLayout = new QHBoxLayout();
    topLayout->setSpacing(margin);
    centralWidgetLayout->addLayout(topLayout);

    auto *monitorCombo = new QComboBox(centralWidget);
    topLayout->addWidget(monitorCombo);

    auto *searchBox = new QLineEdit(centralWidget);
    topLayout->addWidget(searchBox);

    auto *wallpaperGrid = new QWallpaperGrid(centralWidget);
    for(const auto &wallpaper: wallpapers) {
        wallpaperGrid->addPreview(new QWallpaperPreview(wallpaper));
    }

    auto *scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(wallpaperGrid);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    centralWidgetLayout->addWidget(scrollArea);
}

MainWindow &MainWindow::getInstance() {
    static MainWindow instance;
    return instance;
}
