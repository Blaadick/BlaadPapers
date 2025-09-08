#pragma once

#include <string_view>

inline std::string_view mainHelpMessage = R"#(Usage:
    blaadpapers
    blaadpapers -<Option>[SubOptions]

Options:
    H  ->  Shows this help
    V  ->  Shows program version
    S  ->  Sets the wallpaper
    R  ->  Sets the random wallpaper
    L  ->  Shows list of all available wallpapers

Universal sub options:
    h  ->  Shows help for option and exit
    q  ->  Disables command output)#";

inline std::string_view versionHelpMessage = R"#(Usage:
    blaadpapers -V[SubOptions]

Sub options:
    j  ->  Outputs command result in JSON format)#";

inline std::string_view setHelpMessage = R"#(Usage:
    blaadpapers -S[SubOptions] <WallpaperName>)#";

inline std::string_view randomHelpMessage = R"#(Usage:
    blaadpapers -R[SubOptions]
    blaadpapers -Rf[SubOptions] <includeTags> [excludeTags]
    blaadpapers -Rf '["General","Anime"]'

Sub options:
    f  ->  Specifies the tags with which the wallpaper can be installed)#";

inline std::string_view listHelpMessage = R"#(Usage:
    blaadpapers -L[SubOptions]

Sub options:
    j  ->  Outputs command result in JSON format)#";
