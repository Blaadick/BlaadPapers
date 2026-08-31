// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <boost/url.hpp>

class DeeplinkHandler {
public:
    virtual ~DeeplinkHandler() = default;

    virtual int handle(const boost::url_view& url) const = 0;
};
