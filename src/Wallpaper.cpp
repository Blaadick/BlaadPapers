#include "Wallpaper.hpp"

#include "Config.hpp"

using namespace std;
using namespace filesystem;
using nlohmann::json;

Wallpaper::Wallpaper(const string& name, const json& data) {
    this->name = name;
    this->description = data["description"];
    this->tags = data["tags"];
}

const string& Wallpaper::getName() const {
    return name;
}

const string& Wallpaper::getDescription() const {
    return description;
}

const vector<string>& Wallpaper::getTags() const {
    return tags;
}

path Wallpaper::getFilePath() const {
    return Config::getWorkingDir() / (name + ".png");
}

json Wallpaper::toJson() const {
    return {
        {"name", name},
        {"description", description},
        {"tags", tags}
    };
}

bool Wallpaper::operator<(const Wallpaper& other) const {
    return strcasecmp(name.c_str(), other.name.c_str()) < 0;
}
