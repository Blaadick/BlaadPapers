// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DeeplinkHandler.hpp"
#include "Wallpapers.hpp"
#include "logger/Logger.hpp"

class ApplyHandler final : public DeeplinkHandler {
public:
    explicit ApplyHandler(sptr<Wallpapers> wallpapers, sptr<util::Logger> logger);

    int handle(const Url& url) const override;

private:
    sptr<Wallpapers> wallpapers;
    sptr<util::Logger> logger;
};
