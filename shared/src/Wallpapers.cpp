// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Wallpapers.hpp"

#include "Config.hpp"
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

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

Wallpaper* Wallpapers::get(const std::string& id) const {
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

bool Wallpapers::apply(const std::string& id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper->apply();
        }
    }

    return false;
}

bool Wallpapers::remove(const std::string& id) {
    const auto it = std::ranges::find_if(
        wallpapers,
        [=](const auto& wallpaper) {
            return wallpaper->getId() == id;
        }
    );

    if(it == wallpapers.end()) {
        return false;
    }

    const auto wallpaper = std::move(*it);
    wallpapers.erase(it);

    fs::remove(wallpaper->getFilePath());
    fs::remove(wallpaper->getFilePath().root_directory().append(".index/" + wallpaper->getId() + ".json"));

    //TODO Idk, it shouldn't be here. Mb move it
    for(const auto& entry : fs::recursive_directory_iterator(util::previewsDirPath())) {
        if(entry.path().stem() == wallpaper->getId()) {
            fs::remove(entry.path());
        }
    }

    return true;
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
