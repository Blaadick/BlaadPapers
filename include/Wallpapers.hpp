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
    Wallpaper* get(const QString& id) const;

    void add(uptr<Wallpaper> wallpaper);

    void remove(const QString& id);

    void sortByName() {
        std::ranges::sort(
            wallpapers,
            [](const uptr<Wallpaper>& w1, const uptr<Wallpaper>& w2) {
                return w1->getName() < w2->getName();
            }
        );
    }

    void clear() {
        wallpapers.clear();
    }

    [[nodiscard]]
    int count() const;

    [[nodiscard]]
    std::vector<uptr<Wallpaper>>::const_iterator begin() const;

    [[nodiscard]]
    std::vector<uptr<Wallpaper>>::const_iterator end() const;

private:
    std::vector<uptr<Wallpaper>> wallpapers;
};
