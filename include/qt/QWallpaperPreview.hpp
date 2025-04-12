#pragma once

#include <QPushButton>
#include <qwidget.h>

#include "Wallpaper.hpp"

class QWallpaperPreview final : public QPushButton {
public:
    explicit QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent = nullptr);
};
