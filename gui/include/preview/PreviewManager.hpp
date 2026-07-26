// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeindex>
#include <unordered_map>
#include "data/Wallpaper.hpp"
#include "generator/PreviewGenerator.hpp"
#include "logger/Logger.hpp"
#include "util/Pointers.hpp"

class PreviewManager {
public:
    explicit PreviewManager(sptr<util::Logger> logger);

    void createAndSavePreviews(const Wallpaper& wallpaper) const;

    template<std::derived_from<Wallpaper> T>
    void addGenerator(uptr<PreviewGenerator> generator);

private:
    std::unordered_map<std::type_index, uptr<PreviewGenerator>> generators;
    sptr<util::Logger> logger;
};
