// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:WallpaperData;

import std;

export struct WallpaperData {
    std::string name;
    std::string source;
    std::vector<std::string> tags;
};
