#pragma once

#include <string>
#include <vector>
#include <json/json.hpp>

class Wallpaper {
    std::string name;
    std::string description;
    std::vector<std::string> tags;

public:
    Wallpaper(const std::string &name, const nlohmann::json &data);

    [[nodiscard]] std::string getName() const;

    [[nodiscard]] std::string getDescription() const;

    [[nodiscard]] std::vector<std::string> getTags() const;

    [[nodiscard]] std::string getFilePath() const;

    [[nodiscard]] nlohmann::json toJson() const;

    [[nodiscard]] bool operator<(const Wallpaper &other) const;
};
