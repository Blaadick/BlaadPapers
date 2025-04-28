#pragma once

#include <filesystem>

class CacheLoader {
public:
    static void loadCache();

private:
    inline static const std::filesystem::path cacheDir = std::string(getenv("HOME")) + "/.cache/blaadpapers/";
};
