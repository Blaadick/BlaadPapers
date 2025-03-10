#pragma once

inline std::string_view helpMessage = R"#(Usage: blaadpapers [options]
Options:
    --help    (-h) -> Shows this help and exit
    --version (-v) -> Shows version and exit
    --list    (-l) -> Shows list of all available wallpapers
)#";

inline nlohmann::json defaultConfig = {
    {"working_dir", "~/Pictures/Wallpapers/"}
};

inline nlohmann::json defaultWallpaperData = {
    {"description", ""},
    {"tags", {"SFW"}}
};
