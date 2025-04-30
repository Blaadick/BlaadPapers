#pragma once

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
    q  ->  Disables command output

Notes:
    Setting wallpapers for individual monitors does not work
    Quiet output does not work)#";

inline std::string_view versionHelpMessage = R"#(Usage:
    blaadpapers -V[SubOptions]

Sub options:
    j  ->  Outputs command result in JSON format)#";

inline std::string_view setHelpMessage = R"#(Usage:
    blaadpapers -S[SubOptions] <monitorName> <WallpaperName>)#";

inline std::string_view randomHelpMessage = R"#(Usage:
    blaadpapers -R[SubOptions] <monitorName>
    blaadpapers -Rf[SubOptions] <monitorName> <includeTags> [excludeTags]

Sub options:
    s  ->  Specifies the tags with which the wallpaper can be installed)#";

inline std::string_view listHelpMessage = R"#(Usage:
    blaadpapers -L[SubOptions]

Sub options:
    j  ->  Outputs command result in JSON format)#";
