#include "Wallpaper.hpp"
#include "Global.hpp"

Wallpaper::Wallpaper(const std::string &name, const nlohmann::json &data) {
    this->name = name;
    this->description = data["description"];
    this->tags = data["tags"];
}

std::string Wallpaper::getName() const {
    return name;
}

std::string Wallpaper::getDescription() const {
    return description;
}

std::vector<std::string> Wallpaper::getTags() const {
    return tags;
}

std::string Wallpaper::getFilePath() const {
    return workingDir / (name + ".png");
}

nlohmann::json Wallpaper::toJson() const {
    return {
        {"name", name},
        {"description", description},
        {"tags", tags}
    };
}

bool Wallpaper::operator<(const Wallpaper &other) const {
    return strcasecmp(name.c_str(), other.name.c_str()) < 0;
}
