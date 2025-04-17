#include "gui/QWallpaperGrid.hpp"

#include <QWidget>

QWallpaperGrid::QWallpaperGrid(QWidget *parent) : QWidget(parent) {
    grid = new QGridLayout(this);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(10);
}

void QWallpaperGrid::addPreview(QWallpaperPreview *preview) const {
    grid->addWidget(preview);
}

void QWallpaperGrid::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}
