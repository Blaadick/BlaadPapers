#include "qt/QWallpaperPreview.hpp"

#include <QFontMetrics>
#include <qguiapplication.h>
#include <QPainter>
#include <qpen.h>
#include <QPushButton>
#include <QScreen>

QSize getAspectRatio(const QRect &geometry) {
    const int divisor = std::gcd(geometry.width(), geometry.height());
    return {geometry.width() / divisor, geometry.height() / divisor};
}

QWallpaperPreview::QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent) {
    setIcon(QIcon(wallpaper.getFilePath().c_str()));
    setText(wallpaper.getName().c_str());
    setParent(parent);
    setCursor(Qt::PointingHandCursor);
    setFixedSize(getAspectRatio(QGuiApplication::screens()[0]->geometry()) * 15);
}
