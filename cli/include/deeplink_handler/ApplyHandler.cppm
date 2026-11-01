// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module cli:ApplyHandler;

import blaadpapers;
import :DeeplinkHandler;

export class ApplyHandler final : public DeeplinkHandler {
public:
    explicit ApplyHandler(sptr<WallpaperRepository> wallpaperRepository);

    auto handle(const Uri& uri) const -> int override;

private:
    sptr<WallpaperRepository> wallpaperRepository;
};
