#pragma once

#include <string>
#include <vector>
#include <json/json.hpp>

class Wallpaper {
public:
    Wallpaper(const std::string &name, const nlohmann::json &data);

    std::string getName() const;

    std::string getDescription() const;

    std::vector<std::string> getTags() const;

    std::string getFilePath() const;

    nlohmann::json toJson() const;

    bool operator<(const Wallpaper &other) const;

private:
    std::string name;
    std::string description;
    std::vector<std::string> tags;
};
