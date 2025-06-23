#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include "util/PathUtils.hpp"

class Config {
public:
    static void readConfig();

    static const std::filesystem::path& getWorkingDir();

    static const std::vector<std::string>& getBadTags();

    static std::filesystem::path getPostSetScriptPath();

private:
    inline static const std::filesystem::path configDir = getConfigPath() / "blaadpapers";
    inline static const nlohmann::json defaultConfigData = {
        {"bad_tags", {"Sensitive", "Questionable", "Explicit"}},
        {"working_dir", getPicturesPath() / "Wallpapers"}
    };
    static nlohmann::json configData;
    static std::filesystem::path workingDir;
    static std::vector<std::string> badTags;

    template<typename T>
    static T readProperty(const std::string& name);

    static std::filesystem::path readDirProperty(const std::string& name);
};
