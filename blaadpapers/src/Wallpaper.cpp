#include <fstream>
#include <json/json.hpp>

#include "Wallpaper.hpp"
#include "Global.hpp"

Wallpaper::Wallpaper(const string &name, const json &data) {
    this->name = name;
    this->description = data["description"];
    this->tags = data["tags"];
}

string Wallpaper::getName() const {
    return name;
}

string Wallpaper::getDescription() const {
    return description;
}

vector<string> Wallpaper::getTags() const {
    return tags;
}

string Wallpaper::getFilePath() const {
    return workingDir / (name + ".png");
}

json Wallpaper::toJson() const {
    return {
        {"description", description},
        {"tags", tags}
    };
}

bool Wallpaper::operator<(const Wallpaper &other) const {
    return strcasecmp(name.c_str(), other.name.c_str()) < 0;
}
