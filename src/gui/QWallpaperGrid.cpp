#include "gui/QWallpaperGrid.hpp"

#include <QWidget>

QWallpaperGrid::QWallpaperGrid(QWidget *parent) : QWidget(parent) {
    grid = new QFlowLayout(0, 10, 10, this);
}

void QWallpaperGrid::addPreview(QWallpaperPreview *preview) const {
    grid->addWidget(preview);
}
