#pragma once

inline std::string_view mainHelpMessage = R"#(Usage:
    blaadpapers -[MainOption][SubOptions]

Main options:
    H  ->  Shows this help
    V  ->  Shows program version
    S  ->  Sets the wallpaper
    R  ->  Sets the random wallpaper
    L  ->  Shows list of all available wallpapers

Universal sub options:
    h  ->  Shows help for main option and exit
    q  ->  Disables command output
    j  ->  Outputs some command outputs in JSON format)#";

inline std::string_view helpHelpMessage = "WTF bro?";

inline std::string_view versionHelpMessage = "It's just a version, nothing more.";

inline std::string_view setHelpMessage = R"#(Usage:
    blaadpapers -S[SubOptions] <WallpaperName>)#";

inline std::string_view randomHelpMessage = R"#(Usage:
    blaadpapers -R[SubOptions])#";

inline std::string_view listHelpMessage = R"#(Usage:
    blaadpapers -L[SubOptions])#";

inline nlohmann::json defaultConfig = {
    {"working_dir", "~/Pictures/Wallpapers/"}
};

inline nlohmann::json defaultWallpaperData = {
    {"description", ""},
    {"tags", {"General"}}
};
