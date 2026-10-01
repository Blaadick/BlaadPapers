// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TagRepository.hpp"

#include <print>
#include <ranges>

auto TagRepository::getTaggedWallpaper(const std::string& tag) const -> std::unordered_set<sptr<Wallpaper>> {
    auto it = tagsToWallpapers.find(tag);
    if(it == tagsToWallpapers.end()) {
        return {};
    }

    return it->second;
}

auto TagRepository::getWallpaperTags(sptr<Wallpaper> wallpaper) const -> std::unordered_set<std::string> {
    auto it = wallpapersToTags.find(wallpaper);
    if(it == wallpapersToTags.end()) {
        return {};
    }

    return it->second;
}

void TagRepository::tagWallpaper(sptr<Wallpaper> wallpaper, const std::string& tag) {
    auto itT = tagsToWallpapers.find(tag);
    if(itT == tagsToWallpapers.end()) {
        tagsToWallpapers.emplace(tag, std::unordered_set{wallpaper});
    } else {
        itT->second.emplace(wallpaper);
    }

    auto itI = wallpapersToTags.find(wallpaper);
    if(itI == wallpapersToTags.end()) {
        wallpapersToTags.emplace(wallpaper, std::unordered_set{tag});
    } else {
        itI->second.emplace(tag);
    }
}

void TagRepository::untagWallpaper(sptr<Wallpaper> wallpaper, const std::string& tag) {
    auto itT = tagsToWallpapers.find(tag);
    if(itT != tagsToWallpapers.end()) {
        itT->second.erase(wallpaper);

        if(itT->second.empty()) {
            tagsToWallpapers.erase(itT);
        }
    }

    auto itI = wallpapersToTags.find(wallpaper);
    if(itI != wallpapersToTags.end()) {
        itI->second.erase(tag);

        if(itI->second.empty()) {
            wallpapersToTags.erase(itI);
        }
    }
}

void TagRepository::toString() const {
    std::println("TagsToWallpapers ({}):", tagsToWallpapers.size());
    for(auto& item : tagsToWallpapers) {
        std::println(
            "    {:45}: {}",
            item.first,
            item.second | std::views::transform(
                [](const sptr<Wallpaper>& w) {
                    return w->getId();
                }
            )
        );
    }

    std::println("\nWallpapersToTags ({}):", wallpapersToTags.size());
    for(auto& item : wallpapersToTags) {
        std::println("    {:45}: {}", item.first->getId(), item.second);
    }
}
