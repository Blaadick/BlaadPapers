// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

#include <format>
#include <fstream>
#include "Config.hpp"
#include "PostSetScript.hpp"
#include "data/PictureWallpaper.hpp"
#include "util/PathUtils.hpp"
#ifdef __linux__
#include "unistd.h"
#include "sys/socket.h"
#include "sys/un.h"
#endif

namespace fs = std::filesystem;
namespace rng = std::ranges;

// TODO Move away, add loging
bool Wallpaper::apply() const {
    #ifdef __linux__
    if(getenv("XDG_CURRENT_DESKTOP") == "KDE") {
        if(dynamic_cast<const PictureWallpaper*>(this)) {
            system(std::format("plasma-apply-wallpaperimage {}", filePath.c_str()).c_str());
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

    const auto command = nlohmann::json(
        {
            {"command", {"loadfile", filePath}}
        }
    ).dump().append("\n");

    if(write(sock, command.c_str(), command.size()) < 0) {
        close(sock);
        return false;
    }

    close(sock);

    std::ofstream currentWallpaperIdFile(util::currentWallpaperIdPath());
    currentWallpaperIdFile << id;

    PostSetScript::execute(*this);

    return true;
    #endif
}

const std::string& Wallpaper::getId() const {
    return id;
}

const fs::path& Wallpaper::getFilePath() const {
    return filePath;
}

const std::string& Wallpaper::getName() const {
    return name;
}

const Size& Wallpaper::getResolution() const {
    return resolution;
}

const std::string& Wallpaper::getSource() const {
    return source;
}

const std::vector<std::string>& Wallpaper::getTags() const {
    return tags;
}

bool Wallpaper::isBad() const {
    return rng::any_of(
        Config::getBadTags(),
        [this](const std::string& tag) {
            return rng::contains(tags, tag);
        }
    );
}

bool Wallpaper::operator==(const Wallpaper& other) const {
    return id == other.getId();
}
