#include "Wallpaper.hpp"

#include "ConfigReader.hpp"
#include "Global.hpp"

using namespace std;
using namespace filesystem;
using nlohmann::json;

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

path Wallpaper::getFilePath() const {
    return ConfigReader::getWorkingDir() / (name + ".png");
}

path Wallpaper::getPreviewPath(const double devicePixelRatio) const {
    return cacheDir / to_string(devicePixelRatio) / (name + ".png");
}

json Wallpaper::toJson() const {
    return {
        {"name", name},
        {"description", description},
        {"tags", tags}
    };
}

bool Wallpaper::operator<(const Wallpaper &other) const {
    return strcasecmp(name.c_str(), other.name.c_str()) < 0;
}
