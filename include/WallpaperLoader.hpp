#pragma once

#include <vector>
#include "Wallpaper.hpp"

class WallpaperLoader {
public:
    static void loadWallpapers();

private:
    static std::vector<Wallpaper> wallpapers;
};
