// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "option/ApplyOption.hpp"
#include "option/CountOption.hpp"
#include "option/HelpOption.hpp"
#include "option/InfoOption.hpp"
#include "option/ListOption.hpp"
#include "option/Option.hpp"
#include "option/RemoveOption.hpp"
#include "option/RunRendererOption.hpp"
#include "option/ShuffleOption.hpp"
#include "option/VersionOption.hpp"

class OptionExecutor {
public:
    OptionExecutor(
        sptr<WallpaperLoader> wallpaperLoader,
        sptr<Wallpapers> wallpapers,
        sptr<util::Logger> logger
    ) : wallpaperLoader(wallpaperLoader), wallpapers(wallpapers), logger(logger) {
        options.emplace("apply", std::make_unique<ApplyOption>(wallpapers, logger));
        options.emplace("count", std::make_unique<CountOption>(wallpapers, logger));
        options.emplace("help", std::make_unique<HelpOption>(logger));
        options.emplace("info", std::make_unique<InfoOption>(wallpapers, logger));
        options.emplace("list", std::make_unique<ListOption>(wallpapers, logger));
        options.emplace("remove", std::make_unique<RemoveOption>(wallpapers, logger));
        options.emplace("run-renderer", std::make_unique<RunRendererOption>(wallpapers, logger));
        options.emplace("shuffle", std::make_unique<ShuffleOption>(wallpapers, logger));
        options.emplace("version", std::make_unique<VersionOption>(logger));
    }

    int execute(int argc, const char** argv);

private:
    std::unordered_map<const char*, uptr<Option>> options;
    sptr<WallpaperLoader> wallpaperLoader;
    sptr<Wallpapers> wallpapers;
    sptr<util::Logger> logger;
};
