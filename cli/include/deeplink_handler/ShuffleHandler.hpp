// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "DeeplinkHandler.hpp"
#include "WallpaperRepository.hpp"

class ShuffleHandler final : public DeeplinkHandler {
public:
    explicit ShuffleHandler(sptr<WallpaperRepository> wallpaperRepository);

    auto handle(const Uri& uri) const -> int override;

private:
    sptr<WallpaperRepository> wallpaperRepository;
};
