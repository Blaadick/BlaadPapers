// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/Wallpaper.hpp"
#include "util/Pointers.hpp"

class Wallpapers {
public:
    static Wallpapers& inst();

    [[nodiscard]]
    Wallpaper* get(int index) const;

    [[nodiscard]]
    Wallpaper* get(const std::string& id) const;

    void add(uptr<Wallpaper> wallpaper);

    bool apply(const std::string& id) const;

    bool remove(const std::string& id);

    void sortByName();

    void clear();

    [[nodiscard]]
    int count() const;

    [[nodiscard]]
    std::vector<uptr<Wallpaper>>::const_iterator begin() const;

    [[nodiscard]]
    std::vector<uptr<Wallpaper>>::const_iterator end() const;

private:
    std::vector<uptr<Wallpaper>> wallpapers;
};
