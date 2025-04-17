#include "gui/QWallpaperGrid.hpp"

#include <QApplication>
#include <QResizeEvent>
#include <QWidget>
#include "Util.hpp"

QWallpaperGrid::QWallpaperGrid(QWidget *parent) : QWidget(parent) {
    grid = new QGridLayout(this);
    grid->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(10);

    setLayout(grid);
}

void QWallpaperGrid::addPreview(QWallpaperPreview *preview) {
    items.push_back(preview);
    grid->addWidget(preview);
}

const QSize previewSize = getAspectRatio(QGuiApplication::screens()[0]->geometry()) * 10.75;

void QWallpaperGrid::resizeEvent(QResizeEvent *event) {
    QLayoutItem *item;
    while((item = grid->takeAt(0)) != nullptr) {}

    const int columns = std::max(1, width() / previewSize.width());
    int row = 0;
    int col = 0;

    for(const auto &w: items) {
        grid->addWidget(w, row, col);
        if(++col >= columns) {
            col = 0;
            ++row;
        }
    }

    grid->setRowStretch(row + 1, 1);
}
