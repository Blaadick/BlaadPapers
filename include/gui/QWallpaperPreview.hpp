#pragma once

#include <QPushButton>

#include "Wallpaper.hpp"

class QWallpaperPreview final : public QPushButton {
public:
    explicit QWallpaperPreview(const Wallpaper &wallpaper);

private:
    const Wallpaper &wallpaper;
};
