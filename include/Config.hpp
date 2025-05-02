#pragma once

#include <filesystem>
#include <json/json.hpp>

class Config {
public:
    static void readConfig();

    static std::filesystem::path getConfigDir();

    static std::filesystem::path getWorkingDir();

    static std::filesystem::path getPostSetScriptPath();

private:
    static const std::filesystem::path configDir;
    static const nlohmann::json defaultConfig;
    static std::filesystem::path workingDir;
};
