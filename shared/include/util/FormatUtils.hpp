// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <unordered_set>

namespace util {
    inline const std::pmr::unordered_set<std::string> supportedPictureFormats = {".png", ".apng", ".jpeg", ".webp"};
    inline const std::pmr::unordered_set<std::string> supportedVideoFormats = {".mp4", ".webm"};
}
