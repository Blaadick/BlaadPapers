// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "network/Url.hpp"

class DeeplinkHandler {
public:
    virtual ~DeeplinkHandler() = default;

    virtual int handle(const Url& url) const = 0;
};
