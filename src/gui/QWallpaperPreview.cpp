#include "gui/QWallpaperPreview.hpp"

#include <QPushButton>
#include <QScreen>

#include "Util.hpp"
#include "Wallpaper.hpp"

QWallpaperPreview::QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent): QPushButton(parent), wallpaper(wallpaper) {
    setText(this->wallpaper.getName().c_str());
    setFixedSize(getAspectRatio(QGuiApplication::screens()[0]->geometry()) * 10.75);

    connect(this, &QWallpaperPreview::clicked, this, [this] {
        setWallpaper("all", this->wallpaper);
    });
}
