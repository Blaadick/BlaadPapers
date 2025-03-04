#pragma once

#include <string>
#include <vector>
#include <json/json.hpp>

using namespace std;
using namespace nlohmann;

class Wallpaper {
    string name;
    string description;
    vector<string> tags;

public:
    Wallpaper(const string &name, const json &data);

    string getName();

    string getDescription();

    vector<string> getTags();

    json serialize();
};
