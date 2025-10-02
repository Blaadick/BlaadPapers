// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string_view>

inline std::string_view mainHelpMessage = R"#(Usage:
    blaadpapers
    blaadpapers -<option>[sub_options]

Options:
    H  ->  Shows this help
    V  ->  Shows program version
    S  ->  Sets the wallpaper
    R  ->  Sets the random wallpaper
    D  ->  Deletes the wallpaper
    L  ->  Shows list of all available wallpapers

Universal sub options:
    h  ->  Shows help for option and exit
    q  ->  Disables command output)#";

inline std::string_view versionHelpMessage = R"#(Usage:
    blaadpapers -V[sub_options]

Sub options:
    j  ->  Outputs command result in JSON format)#";

inline std::string_view setHelpMessage = R"#(Usage:
    blaadpapers -S[sub_options] <wallpaper_id>)#";

inline std::string_view randomHelpMessage = R"#(Usage:
    blaadpapers -R[sub_options]
    blaadpapers -Rf[sub_options] <include_tags> [exclude_tags]
    blaadpapers -Rf '["General","Anime"]'

Sub options:
    f  ->  Specifies the tags with which the wallpaper can be installed)#";

inline std::string_view deleteHelpMessage = R"#(Usage:
    blaadpapers -D[sub_options] <wallpaper_id>)#";

inline std::string_view listHelpMessage = R"#(Usage:
    blaadpapers -L[sub_options]

Sub options:
    t  ->  Outputs list of unique tags instead wallpapers
    j  ->  Outputs command result in JSON format)#";
