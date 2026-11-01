// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:DownloadData;

import std;

export struct DownloadData {
    std::filesystem::path partFilePath;
    std::string eTag;
};
