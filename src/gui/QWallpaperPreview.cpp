#include "gui/QWallpaperPreview.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include "Global.hpp"
#include "Util.hpp"
#include "Wallpaper.hpp"

QWallpaperPreview::QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent): QAbstractButton(parent), wallpaper(wallpaper) {
    setFixedSize(getAspectRatio(QGuiApplication::screens()[0]->geometry()) * 13.6);
    connect(this, &QWallpaperPreview::clicked, this, [this] {
        setWallpaper("all", this->wallpaper);
    });

    pixmap = loadCachedPreview();
}

QPixmap QWallpaperPreview::loadCachedPreview() const {
    const std::filesystem::path cachedWallpaperPreview = cacheDir / (wallpaper.getName() + ".png");
    QImage image;

    if(!exists(cacheDir)) {
        create_directory(cacheDir);
    }

    if(exists(cachedWallpaperPreview)) {
        image = QImage(cachedWallpaperPreview.c_str());
    } else {
        image = QImage(wallpaper.getFilePath().c_str()).scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        if(image.save(cachedWallpaperPreview.c_str(), "PNG")) {
            qDebug() << wallpaper.getName() << "preview cached";
        } else {
            qDebug() << wallpaper.getName() << "failed";
        }
    }

    return QPixmap::fromImage(image);
}

void QWallpaperPreview::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    const QRect rect(0, 0, width(), height());

    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    painter.setClipPath(path);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!wallpaper.getTags().data()->contains("General")) {
        painter.setBrush(Qt::black);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);
        painter.setPen(Qt::red);
        painter.drawText(rect, Qt::AlignCenter, "PORNO!");
    } else {
        painter.drawPixmap(rect, pixmap);
    }

    if(!underMouse()) {
        QLinearGradient gradient(0, height(), 0, 0);
        gradient.setColorAt(0.1, Qt::black);
        gradient.setColorAt(0.5, Qt::transparent);
        painter.fillPath(path, gradient);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Monospace", 8, QFont::Bold));
        painter.drawText(rect.adjusted(10, 10, -10, -10), Qt::AlignLeft | Qt::AlignBottom, wallpaper.getName().c_str());
    }
}
