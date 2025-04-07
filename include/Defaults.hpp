#pragma once

inline std::string_view mainHelpMessage = R"#(Usage:
    blaadpapers -[MainOption][SubOptions]

Options:
    H  ->  Shows this help
    V  ->  Shows program version
    S  ->  Sets the wallpaper
    R  ->  Sets the random wallpaper
    L  ->  Shows list of all available wallpapers

Universal sub options:
    h  ->  Shows help for main option and exit
    q  ->  Disables command output (it'll be working soon)
)#";

inline std::string_view versionHelpMessage = R"#(Usage:
    blaadpapers -V[SubOptions]

Sub options:
    j  ->  Outputs command result in JSON format)#";

inline std::string_view setHelpMessage = R"#(Usage:
    blaadpapers -S[SubOptions] <WallpaperName>)#";

inline std::string_view randomHelpMessage = R"#(Usage:
    blaadpapers -R[SubOptions]
    blaadpapers -Rf[SubOptions] <TagsListInJsonFormat>

Sub options:
    f  ->  Specifies the tags with which the wallpaper can be installed)#";

inline std::string_view listHelpMessage = R"#(Usage:
    blaadpapers -L[SubOptions]

Sub options:
    j  ->  Outputs command result in JSON format)#";

inline nlohmann::json defaultConfig = {
    {"working_dir", "~/Pictures/Wallpapers/"}
};

inline nlohmann::json defaultWallpaperData = {
    {"description", ""},
    {"tags", {"General"}}
};
