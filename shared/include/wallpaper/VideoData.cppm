// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:VideoData;

import :Size;

export struct VideoData {
    Size resolution;
    int frameRate = -1;
};
