#include "gui/QWallpaperPreview.hpp"

#include <qguiapplication.h>
#include <QPushButton>
#include <QScreen>

#include "Util.hpp"
#include "Wallpaper.hpp"
#include "gui/MainWindow.hpp"


QWallpaperPreview::QWallpaperPreview(const QScreen *screen, const Wallpaper &wallpaper, QWidget *parent) {
    setText(wallpaper.getName().c_str());
    setParent(parent);
    setFixedSize(getAspectRatio(screen->geometry()) * 11);
}
