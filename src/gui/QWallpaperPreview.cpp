#include "gui/QWallpaperPreview.hpp"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>

#include "CacheLoader.hpp"
#include "Util.hpp"
#include "Wallpaper.hpp"

using namespace std;
using namespace filesystem;

QWallpaperPreview::QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent) : QAbstractButton(parent), wallpaper(wallpaper) {
    setFixedSize(getAspectRatio(screen()) * 18.4);
    connect(this, &QWallpaperPreview::clicked, this, [this] {
        setWallpaper("all", this->wallpaper);
    });
}

void QWallpaperPreview::refreshPreview() {
    const QImage image(CacheLoader::getPreviewPath(wallpaper, screen()->devicePixelRatio()).c_str());
    pixmap = QPixmap::fromImage(image);
}

void QWallpaperPreview::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    const QRect rect(0, 0, width(), height());

    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    painter.setClipPath(path);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawPixmap(rect, pixmap);

    if(!underMouse()) {
        QLinearGradient gradient(0, height(), 0, 0);
        gradient.setColorAt(0.1, Qt::black);
        gradient.setColorAt(0.3, Qt::transparent);
        painter.fillPath(path, gradient);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Monospace", 10, QFont::Bold));
        painter.drawText(rect.adjusted(10, 10, -10, -10), Qt::AlignLeft | Qt::AlignBottom, wallpaper.getName().c_str());
    }
}
