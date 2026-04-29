// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include "data/Wallpaper.hpp"

class PostSetScript {
public:
    static void createIfNotExists();

    static void execute(const Wallpaper& wallpaper);

    static std::filesystem::path postSetScriptPath();
};
