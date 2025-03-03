#pragma once

#include <string>
#include <vector>

using namespace std;

class Wallpaper {
    string name;
    string description;
    vector<string> tags;

public:
    string getName();

    string getDescription();

    vector<string> getTags();

    void serialize();

    static Wallpaper deserialize();
};
