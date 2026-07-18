// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DeeplinkHandler.hpp"
#include "Wallpapers.hpp"

class ShuffleHandler final : public DeeplinkHandler {
public:
    explicit ShuffleHandler(sptr<Wallpapers> wallpapers);

    int handle(const Url& url) const override;

private:
    sptr<Wallpapers> wallpapers;
};
