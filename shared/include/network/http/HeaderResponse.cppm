// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:HeaderResponse;

import std;

export struct HeaderResponse {
    std::string fileName;
    std::string contentType;
};
