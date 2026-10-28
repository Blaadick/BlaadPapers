// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>

namespace util {
    auto moveToTrash(const std::filesystem::path& path) -> bool;
}
