// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WallpaperRepository.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <fstream>
#include <random>
#include "network/Uri.hpp"
#include "util/PathUtils.hpp"
#include "util/Trash.hpp"
#include "wallpaper/PictureWallpaper.hpp"

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#elif _WIN32
#include <windows.h>
#endif

WallpaperRepository::WallpaperRepository(
    sptr<Config> config,
    sptr<ShellExecutor> shellExecutor
) : config(std::move(config)), shellExecutor(std::move(shellExecutor)) {}

auto WallpaperRepository::get(const int index) const noexcept -> sptr<Wallpaper> {
    if(index >= wallpapers.size()) {
        return nullptr;
    }

    return wallpapers[index];
}

auto WallpaperRepository::get(const std::string_view id) const noexcept -> sptr<Wallpaper> {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return wallpaper;
        }
    }

    return nullptr;
}

auto WallpaperRepository::shuffle(
    std::vector<std::string> includeTags,
    std::vector<std::string> excludeTags
) const -> std::optional<sptr<Wallpaper>> {
    if(wallpapers.empty()) {
        return std::nullopt;
    }

    std::mt19937 rnd(std::random_device{}());
    std::vector<sptr<Wallpaper>> filteredWallpapers = wallpapers;

    if(!includeTags.empty()) {
        const auto notContainsIncludeTags = [&includeTags](sptr<Wallpaper> wallpaper) {
            return !std::ranges::all_of(
                includeTags,
                [&wallpaper](const std::string& tag) {
                    return std::ranges::contains(wallpaper->getTags(), tag);
                }
            );
        };

        std::erase_if(filteredWallpapers, notContainsIncludeTags);
    }

    if(!excludeTags.empty()) {
        const auto containsExcludeTags = [&excludeTags](sptr<Wallpaper> wallpaper) {
            return std::ranges::any_of(
                excludeTags,
                [&wallpaper](const std::string& tag) {
                    return std::ranges::contains(wallpaper->getTags(), tag);
                }
            );
        };

        std::erase_if(filteredWallpapers, containsExcludeTags);
    }

    if(!filteredWallpapers.empty()) {
        const auto randomIndex = std::uniform_int_distribution(0, static_cast<int>(filteredWallpapers.size() - 1))(rnd);
        return filteredWallpapers[randomIndex];
    }

    return std::nullopt;
}

void WallpaperRepository::add(sptr<Wallpaper> wallpaper) {
    wallpapers.push_back(std::move(wallpaper));
}

auto WallpaperRepository::apply(const std::string_view id) const -> std::expected<void, std::string> {
    for(const auto& wallpaper : wallpapers) {
        if(wallpaper->getId() == id) {
            return apply(*wallpaper);
        }
    }

    return std::unexpected("Wallpaper with this id not found");
}

auto WallpaperRepository::apply(const Wallpaper& wallpaper) const -> std::expected<void, std::string> {
    #ifdef __linux__
    auto deEnv = getenv("XDG_CURRENT_DESKTOP");

    if(deEnv && std::strcmp(deEnv, "KDE") == 0) {
        if(dynamic_cast<const PictureWallpaper*>(&wallpaper)) {
            auto exitCode = shellExecutor->execute(
                std::format("plasma-apply-wallpaperimage \"{}\" > /dev/null 2>&1", wallpaper.getFilePath())
            );

            if(exitCode.has_value()) {
                if(exitCode == 0) {
                    return {};
                }

                return std::unexpected(std::format("plasma-apply-wallpaperimage exited with code {}", exitCode.value()));
            }

            return std::unexpected(std::format("Failed to execute plasma-apply-wallpaperimage ({})", exitCode.error()));
        }

        return std::unexpected("KDE only supports picture wallpapers");
    }

    if(deEnv && std::strstr(deEnv, "GNOME")) {
        if(dynamic_cast<const PictureWallpaper*>(&wallpaper)) {
            auto exitCode = shellExecutor->execute(
                std::format(
                    "gsettings set org.gnome.desktop.background picture-uri {0} > /dev/null 2>&1 && gsettings set org.gnome.desktop.background picture-uri-dark {0} > /dev/null 2>&1",
                    Uri::fromPath(wallpaper.getFilePath())
                )
            );

            if(exitCode.has_value()) {
                if(exitCode == 0) {
                    return {};
                }

                return std::unexpected(std::format("gsettings exited with code {}", exitCode.value()));
            }

            return std::unexpected(std::format("Failed to execute gsettings ({})", exitCode.error()));
        }

        return std::unexpected("GNOME only supports picture wallpapers");
    }

    const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un sockAddr(AF_UNIX, "/tmp/blaadpapers-mpvpaper.sock");

    if(connect(sock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) < 0) {
        close(sock);
        return std::unexpected("Failed to establish a connection with the mpvpaper socket");
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
        return std::unexpected("Failed to write the command to the mpvpaper socket");
    }

    close(sock);

    if(util::createDirIfNotExists(util::stateDir())) {
        std::ofstream currentWallpaperIdFile(util::currentWallpaperIdFilePath());
        currentWallpaperIdFile << wallpaper.getId();
    }

    std::ignore = shellExecutor->executeWithWallpaperData(config->getPostApplyScript(), wallpaper);

    return {};
    #endif

    #ifdef _WIN32
    auto out = SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        const_cast<wchar_t*>(wallpaper.getFilePath().c_str()),
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );

    std::ignore = shellExecutor->executeWithWallpaperData(config->getPostApplyScript(), wallpaper);

    if(!out) {
        return std::unexpected("Windows say that something wrong");
    }

    return {};
    #endif
}

auto WallpaperRepository::remove(const std::string_view id) -> std::expected<void, std::string> {
    const auto it = std::ranges::find_if(
        wallpapers,
        [&id](const auto& wallpaper) {
            return wallpaper->getId() == id;
        }
    );

    if(it == wallpapers.end()) {
        return std::unexpected("Wallpaper not found");
    }

    if(config->getDeleteWallpapersPermanently()) {
        std::error_code errorCode;
        std::filesystem::remove_all((*it)->getRootDirPath(), errorCode);
        if(errorCode) {
            return std::unexpected(std::format("Unable to delete wallpaper ({})", errorCode.message()));
        }
    } else if(!util::moveToTrash((*it)->getRootDirPath())) {
        return std::unexpected("Unable to move wallpaper to trash");
    }

    const auto wallpaper = std::move(*it);
    wallpapers.erase(it);

    std::ignore = shellExecutor->executeWithWallpaperData(config->getPostDeleteScript(), *wallpaper);

    return {};
}

void WallpaperRepository::sortByName() noexcept {
    std::ranges::sort(
        wallpapers,
        [](const sptr<Wallpaper>& w1, const sptr<Wallpaper>& w2) {
            return w1->getName() < w2->getName();
        }
    );
}

void WallpaperRepository::clear() noexcept {
    wallpapers.clear();
}

auto WallpaperRepository::count() const noexcept -> int {
    return static_cast<int>(wallpapers.size());
}

auto WallpaperRepository::begin() const noexcept -> std::vector<sptr<Wallpaper>>::const_iterator {
    return wallpapers.begin();
}

auto WallpaperRepository::end() const noexcept -> std::vector<sptr<Wallpaper>>::const_iterator {
    return wallpapers.end();
}
