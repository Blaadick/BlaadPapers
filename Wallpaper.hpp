#ifndef WALLPAPER_HPP
#define WALLPAPER_HPP
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
};


#endif //WALLPAPER_HPP
