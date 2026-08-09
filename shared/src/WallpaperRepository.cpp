// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "WallpaperRepository.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <fstream>
#include <random>
#include <yyjson.h>
#include "PostSetScript.hpp"
#include "config/Config.hpp"
#include "data/PictureWallpaper.hpp"
#include "util/PathUtils.hpp"
#ifdef __linux__
#include "unistd.h"
#include "sys/socket.h"
#include "sys/un.h"
#elif _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;
namespace rng = std::ranges;

Wallpaper* WallpaperRepository::get(const int index) const {
    if(index >= wallpapers.size()) {
        return nullptr;
    }

    return wallpapers[index].get();
}

Wallpaper* WallpaperRepository::get(const std::string_view id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper.get();
        }
    }

    return nullptr;
}

Wallpaper* WallpaperRepository::shuffle(
    std::optional<std::vector<std::string>> includeTags,
    std::optional<std::vector<std::string>> excludeTags
) const {
    if(wallpapers.empty()) {
        return nullptr;
    }

    std::mt19937 rnd(std::random_device{}());
    std::vector<Wallpaper*> filteredWallpapers;

    for(const auto& wallpaper : wallpapers) {
        filteredWallpapers.emplace_back(wallpaper.get());
    }

    if(includeTags.has_value()) {
        const auto notContainsIncludeTags = [&includeTags](Wallpaper* wallpaper) {
            return !rng::all_of(
                *includeTags,
                [&wallpaper](const std::string& tag) {
                    return rng::contains(wallpaper->getTags(), tag);
                }
            );
        };

        std::erase_if(filteredWallpapers, notContainsIncludeTags);
    }

    if(excludeTags.has_value()) {
        const auto containsExcludeTags = [&excludeTags](Wallpaper* wallpaper) {
            return rng::any_of(
                *excludeTags,
                [&wallpaper](const std::string& tag) {
                    return rng::contains(wallpaper->getTags(), tag);
                }
            );
        };

        std::erase_if(filteredWallpapers, containsExcludeTags);
    }

    if(!filteredWallpapers.empty()) {
        const auto randomIndex = std::uniform_int_distribution(0, static_cast<int>(filteredWallpapers.size() - 1))(rnd);
        return filteredWallpapers[randomIndex];
    }

    return nullptr;
}

void WallpaperRepository::add(uptr<Wallpaper> wallpaper) {
    wallpapers.push_back(std::move(wallpaper));
}

bool WallpaperRepository::apply(const std::string_view id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return apply(*wallpaper);
        }
    }

    return false;
}

bool WallpaperRepository::apply(const Wallpaper& wallpaper) const {
    #ifdef __linux__
    if(std::strcmp(getenv("XDG_CURRENT_DESKTOP"), "KDE") == 0) {
        if(dynamic_cast<const PictureWallpaper*>(&wallpaper)) {
            system(std::format("plasma-apply-wallpaperimage \"{}\" > /dev/null 2>&1", wallpaper.getFilePath()).c_str());
            return true;
        }

        return false;
    }

    const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un sockAddr(AF_UNIX, "/tmp/blaadpapers-mpvpaper.sock");

    if(connect(sock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) < 0) {
        close(sock);
        return false;
    }

    const auto doc = yyjson_mut_doc_new(nullptr);
    const auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    const auto commandData = yyjson_mut_arr(doc);
    yyjson_mut_arr_add_str(doc, commandData, "loadfile");
    yyjson_mut_arr_add_str(doc, commandData, wallpaper.getFilePath().c_str());

    yyjson_mut_obj_add_val(doc, root, "command", commandData);

    const auto command = yyjson_mut_write(doc, YYJSON_WRITE_NOFLAG | YYJSON_WRITE_NEWLINE_AT_END, nullptr);
    yyjson_mut_doc_free(doc);

    if(write(sock, command, strlen(command)) < 0) {
        close(sock);
        return false;
    }

    close(sock);

    if(util::createDirIfNotExists(util::localDataDirPath())) {
        std::ofstream currentWallpaperIdFile(util::currentWallpaperIdPath());
        currentWallpaperIdFile << wallpaper.getId();
    }

    PostSetScript::execute(wallpaper);

    return true;
    #endif

    #ifdef _WIN32
    return SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID) wallpaper.getFilePath().c_str(),
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );
    #endif
}

bool WallpaperRepository::remove(const std::string_view id) {
    const auto it = std::ranges::find_if(
        wallpapers,
        [&id](const auto& wallpaper) {
            return wallpaper->getId() == id;
        }
    );

    if(it == wallpapers.end()) {
        return false;
    }

    const auto wallpaper = std::move(*it);
    wallpapers.erase(it);

    fs::remove_all(wallpaper->getDirPath());
    return true;
}

void WallpaperRepository::sortByName() {
    std::ranges::sort(
        wallpapers,
        [](const uptr<Wallpaper>& w1, const uptr<Wallpaper>& w2) {
            return w1->getName() < w2->getName();
        }
    );
}

void WallpaperRepository::clear() {
    wallpapers.clear();
}

int WallpaperRepository::count() const {
    return static_cast<int>(wallpapers.size());
}

std::vector<uptr<Wallpaper>>::const_iterator WallpaperRepository::begin() const {
    return wallpapers.begin();
}

std::vector<uptr<Wallpaper>>::const_iterator WallpaperRepository::end() const {
    return wallpapers.end();
}

Wallpaper* WallpaperRepository::operator[](const int index) const {
    if(index >= wallpapers.size()) {
        return nullptr;
    }

    return wallpapers[index].get();
}

Wallpaper* WallpaperRepository::operator[](const std::string_view id) const {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper.get();
        }
    }

    return nullptr;
}
