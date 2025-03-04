#include <fstream>
#include <json/json.hpp>

#include "Wallpaper.hpp"

Wallpaper::Wallpaper(const string &name, const string &description, const vector<string> &tags) {
    this->name = name;
    this->description = description;
    this->tags = tags;
}

Wallpaper::Wallpaper(const string &name, const json &data) {
    this->name = name;
    this->description = data["description"];
    this->tags = data["tags"];
}

string Wallpaper::getName() {
    return name;
}

string Wallpaper::getDescription() {
    return description;
}

vector<string> Wallpaper::getTags() {
    return tags;
}

json Wallpaper::serialize() {
    json data = {
        {"description", description},
        {"tags", tags}
    };

    return data;
}
