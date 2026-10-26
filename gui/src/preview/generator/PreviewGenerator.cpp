// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "preview/generator/PreviewGenerator.hpp"

PreviewGenerator::PreviewGenerator(const file::FileType& previewFileType) : previewFileType(previewFileType) {}

auto PreviewGenerator::getPreviewFileType() const noexcept -> const file::FileType& {
    return previewFileType;
}
