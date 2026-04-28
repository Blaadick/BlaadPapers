// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeindex>
#include "data/Wallpaper.hpp"
#include "preview/PreviewGenerator.hpp"
#include "util/Pointers.hpp"

class PreviewManager {
public:
    static void createAndSavePreview(const uptr<Wallpaper>& wallpaper);

private:
    static std::unordered_map<std::type_index, sptr<PreviewGenerator>> generators;
};
