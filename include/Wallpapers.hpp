#pragma once

#include <vector>
#include "Wallpaper.hpp"

class Wallpapers {
public:
    static void loadWallpapers();

    static const std::vector<Wallpaper>& getWallpapers();

private:
    static std::vector<Wallpaper> wallpapers;
};
