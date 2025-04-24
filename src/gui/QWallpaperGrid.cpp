#include "gui/QWallpaperGrid.hpp"

QWallpaperGrid::QWallpaperGrid(QWidget *parent) : QWidget(parent) {
    grid = new QFlowLayout(0, 10, 10, this);
}

void QWallpaperGrid::addPreview(QWallpaperPreview *preview) const {
    grid->addWidget(preview);
}

void QWallpaperGrid::paintEvent(QPaintEvent *event) {
    static QScreen *lastScreen;

    if(lastScreen != screen()) {
        lastScreen = screen();

        for(int i = 0; i < grid->count(); i++) {
            dynamic_cast<QWallpaperPreview *>(grid->itemAt(i)->widget())->refreshPreview();
        }
    }
}
