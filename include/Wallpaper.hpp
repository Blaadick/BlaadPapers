#pragma once

#include <string>
#include <vector>
#include <json/json.hpp>

using namespace std;
using namespace nlohmann;
using namespace filesystem;

class Wallpaper {
    string name;
    string description;
    vector<string> tags;

public:
    Wallpaper(const string &name, const json &data);

    [[nodiscard]] string getName() const;

    [[nodiscard]] string getDescription() const;

    [[nodiscard]] vector<string> getTags() const;

    [[nodiscard]] string getFilePath() const;

    [[nodiscard]] json toJson() const;
};
