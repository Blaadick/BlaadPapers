#pragma once

#include <string>
#include <vector>
#include <json/json.hpp>

class Wallpaper {
public:
    inline static const nlohmann::json defaultWallpaperData = {
        {"description", ""},
        {"tags", {"General"}}
    };

    Wallpaper(const std::string& name, const nlohmann::json& data);

    const std::string& getName() const;

    const std::string& getDescription() const;

    const std::vector<std::string>& getTags() const;

    std::filesystem::path getFilePath() const;

    nlohmann::json toJson() const;

    bool operator<(const Wallpaper& other) const;

private:
    std::string name;
    std::string description;
    std::vector<std::string> tags;
};
