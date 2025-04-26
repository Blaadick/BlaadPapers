#pragma once

#include <set>
#include "Wallpaper.hpp"

inline const std::filesystem::path cacheDir = std::string(getenv("HOME")) + "/.cache/blaadpapers/";
inline const nlohmann::json defaultWallpaperData = {
    {"description", ""},
    {"tags", {"General"}}
};
inline std::pmr::set<Wallpaper> wallpapers;
