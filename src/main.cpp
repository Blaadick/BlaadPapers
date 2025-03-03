#include <iostream>
#include <vector>

#include "Wallpaper.hpp"

using namespace std;

int main(const int argc, const char *argv[]) {
    for(int i = 1; i < argc; i++) {
        string arg = argv[i];

        if(arg == "-h" || arg == "--help") {
            cout << "Usage: blaadpapers [options]" << endl;
            cout << "Options:" << endl;
            cout << "  -h: Show this help and exit." << endl;
            cout << "  -v: Show version and exit." << endl;

            return 0;
        }

        if(arg == "-v" || arg == "--version") {
            cout << "Version: 0.0.1-ALPHA" << endl;

            return 0;
        }

        cout << "Unknown option: " << arg << endl;
    }

    vector<Wallpaper> wallpapers;
}
