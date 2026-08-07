// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DeeplinkHandler.hpp"
#include "WallpaperRepository.hpp"

class ApplyHandler final : public DeeplinkHandler {
public:
    explicit ApplyHandler(sptr<WallpaperRepository> wallpaperRepository);

    int handle(const Url& url) const override;

private:
    sptr<WallpaperRepository> wallpaperRepository;
};
