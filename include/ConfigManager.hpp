#pragma once

#include <filesystem>
#include <json/json.hpp>

class ConfigManager {
public:
    static void readConfig();

    static std::filesystem::path getWorkingDir();

private:
    inline static const std::filesystem::path configDir = std::string(getenv("HOME")) + "/.config/blaadpapers/";
    inline static const nlohmann::json defaultConfig = {
        {"working_dir", "~/Pictures/Wallpapers/"}
    };
    inline static std::filesystem::path workingDir;
};
