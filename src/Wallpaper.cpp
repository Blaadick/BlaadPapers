#include "Wallpaper.hpp"

string Wallpaper::getName() {
    return name;
}

string Wallpaper::getDescription() {
    return description;
}

vector<string> Wallpaper::getTags() {
    return tags;
}

void Wallpaper::serialize() {}

Wallpaper Wallpaper::deserialize() {
    return {};
}
