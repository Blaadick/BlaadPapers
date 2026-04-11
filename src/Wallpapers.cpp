// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpapers.hpp"

Wallpapers& Wallpapers::inst() {
    static Wallpapers inst;
    return inst;
}

Wallpaper* Wallpapers::get(const int index) const {
    if(index >= wallpapers.size()) {
        return nullptr;
    }

    return wallpapers[index].get();
}

Wallpaper* Wallpapers::get(const QString& id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper.get();
        }
    }

    return nullptr;
}

void Wallpapers::add(uptr<Wallpaper> wallpaper) {
    wallpapers.push_back(std::move(wallpaper));
}

bool Wallpapers::apply(const QString& id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper->apply();
        }
    }

    return false;
}

bool Wallpapers::remove(const QString& id) {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            wallpaper->remove();
            std::erase(wallpapers, wallpaper);
            return true;
        }
    }

    return false;
}

void Wallpapers::sortByName() {
    std::ranges::sort(
        wallpapers,
        [](const uptr<Wallpaper>& w1, const uptr<Wallpaper>& w2) {
            return w1->getName() < w2->getName();
        }
    );
}

void Wallpapers::clear() {
    wallpapers.clear();
}

int Wallpapers::count() const {
    return static_cast<int>(wallpapers.size());
}

std::vector<uptr<Wallpaper>>::const_iterator Wallpapers::begin() const {
    return wallpapers.begin();
}

std::vector<uptr<Wallpaper>>::const_iterator Wallpapers::end() const {
    return wallpapers.end();
}
