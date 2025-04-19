#pragma once

#include <set>
#include "Wallpaper.hpp"

inline std::filesystem::path configDir = std::string(getenv("HOME")) + "/.config/blaadpapers/";
inline std::filesystem::path cacheDir = std::string(getenv("HOME")) + "/.cache/blaadpapers/";
inline std::filesystem::path workingDir;
inline std::pmr::set<Wallpaper> wallpapers;
