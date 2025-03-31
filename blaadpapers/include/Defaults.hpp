#pragma once

inline std::string_view helpMessage = R"#(Usage:
    blaadpapers <operation>
Operations:
    -h, --help       -> Shows this help and exit
    -v, --version    -> Shows version and exit
    -S, --set        -> Sets the wallpaper
    -R, --random     -> Sets the random wallpaper
    -l, --list       -> Shows list of all available wallpapers
    -L, --list-json  -> Shows list of all available wallpapers in json format
)#";

inline nlohmann::json defaultConfig = {
    {"working_dir", "~/Pictures/Wallpapers/"}
};

inline nlohmann::json defaultWallpaperData = {
    {"description", ""},
    {"tags", {"SFW"}}
};
