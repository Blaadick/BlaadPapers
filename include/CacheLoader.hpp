#pragma once

#include <filesystem>

#include "Wallpaper.hpp"

class CacheLoader {
public:
    static void loadCache();

    static std::filesystem::path getPreviewPath(const Wallpaper &wallpaper, double devicePixelRatio);

private:
    inline static const std::filesystem::path cacheDir = std::string(getenv("HOME")) + "/.cache/blaadpapers/";
};
