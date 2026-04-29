// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "util/PathUtils.hpp"
#include "util/ToString.hpp"

namespace util {
    inline std::filesystem::path previewsDirPath(const QScreen* screen) {
        return previewsDirPath().append(toString(screen));
    }
}
