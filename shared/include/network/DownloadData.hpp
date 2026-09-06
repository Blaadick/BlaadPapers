// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>

struct DownloadData {
    std::filesystem::path finalPath;
    std::string eTag;
};
