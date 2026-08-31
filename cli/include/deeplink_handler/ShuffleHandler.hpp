// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DeeplinkHandler.hpp"
#include "WallpaperRepository.hpp"

class ShuffleHandler final : public DeeplinkHandler {
public:
    explicit ShuffleHandler(sptr<WallpaperRepository> wallpaperRepository);

    int handle(const boost::url_view& url) const override;

private:
    sptr<WallpaperRepository> wallpaperRepository;
};
