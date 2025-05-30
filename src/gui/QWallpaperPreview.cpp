#include "gui/QWallpaperPreview.hpp"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include "CacheLoader.hpp"
#include "Wallpaper.hpp"
#include "util/ScreenUtils.hpp"

using namespace std;
using namespace filesystem;

QWallpaperPreview::QWallpaperPreview(const Wallpaper& wallpaper, QWidget* parent) : QAbstractButton(parent), wallpaper(wallpaper) {
    refreshSize();

    connect(this, &QWallpaperPreview::clicked, this, [this] {
        setWallpaper("all", this->wallpaper);
    });
}

void QWallpaperPreview::refreshPreview() {
    const QImage image(CacheLoader::getPreviewPath(wallpaper, screen()->devicePixelRatio()).c_str());
    pixmap = QPixmap::fromImage(image);

    isBadWallpaper = ranges::any_of(Config::getBadTags(), [&](const auto& tag) {
        return wallpaper.getTags().data()->contains(tag);
    });
}

void QWallpaperPreview::refreshSize() {
    setFixedSize(getAspectRatio(screen()) * 18.4);
}

void QWallpaperPreview::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    const QRect rect(0, 0, width(), height());

    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    painter.setClipPath(path);
    painter.setRenderHint(QPainter::Antialiasing);

    if(isBadWallpaper && !underMouse()) {
        painter.fillRect(rect, Qt::black);
        painter.setPen(QColor("#3d020a"));
        painter.setFont(QFont("Monospace", 175, QFont::Bold));
        painter.drawText(rect, Qt::AlignCenter, "BAD");
    } else {
        painter.drawPixmap(rect, pixmap);
    }

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
