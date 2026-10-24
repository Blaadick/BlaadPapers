// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include "wallpaper/Wallpaper.hpp"

class PostSetScript final {
public:
    static void createIfNotExists();

    static void execute(const Wallpaper& wallpaper);

    static auto postSetScriptFilePath() -> std::filesystem::path;
};
