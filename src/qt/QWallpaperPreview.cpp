#include "qt/QWallpaperPreview.hpp"

#include <qguiapplication.h>
#include <QPushButton>
#include <QScreen>

#include "Wallpaper.hpp"
#include "qt/MainWindow.hpp"

QSize getAspectRatio(const QRect &geometry) {
    const int divisor = std::gcd(geometry.width(), geometry.height());
    return {geometry.width() / divisor, geometry.height() / divisor};
}

QWallpaperPreview::QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent) {
    setIcon(QIcon(wallpaper.getFilePath().c_str()));
    setText(wallpaper.getName().c_str());
    setParent(parent);
    setFixedSize(getAspectRatio(MainWindow::getSelectedScreen()->geometry()) * 11);
}
