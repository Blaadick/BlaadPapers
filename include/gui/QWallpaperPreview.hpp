#pragma once

#include <QPushButton>
#include "Wallpaper.hpp"

class QWallpaperPreview final : public QPushButton {
    Q_OBJECT

public:
    explicit QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent = nullptr);

private:
    const Wallpaper &wallpaper;
};
