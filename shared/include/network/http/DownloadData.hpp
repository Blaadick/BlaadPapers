// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <string>

struct DownloadData {
    std::filesystem::path partFilePath;
    std::string eTag;
};
