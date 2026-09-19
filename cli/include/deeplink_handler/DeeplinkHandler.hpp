// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "network/Uri.hpp"

class DeeplinkHandler {
public:
    virtual ~DeeplinkHandler() = default;

    virtual auto handle(const Uri& uri) const -> int = 0;
};
