// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "preview/generator/PicturePreviewGenerator.hpp"

#include <vips/vips8>

using namespace vips;

PicturePreviewGenerator::PicturePreviewGenerator() : PreviewGenerator(file::webp) {}

auto PicturePreviewGenerator::createAndSavePreview(
    const Wallpaper& wallpaper,
    const Size& previewSize,
    const std::filesystem::path& previewPath
) const -> std::expected<void, std::string> {
    try {
        VImage preview = VImage::new_from_file(
            wallpaper.getFilePath().string().c_str(),
            VImage::option()->set("access", "sequential")
        );

        const int shrinkSize = std::max(
            1,
            std::min(preview.width() / previewSize.width, preview.height() / previewSize.height)
        );
        preview = preview.shrink(shrinkSize, shrinkSize);

        const float scale = std::max(
            static_cast<float>(previewSize.width) / static_cast<float>(preview.width()),
            static_cast<float>(previewSize.height) / static_cast<float>(preview.height())
        );
        preview = preview.resize(scale, VImage::option()->set("kernel", VIPS_KERNEL_LANCZOS3));

        preview = preview.crop(
            (preview.width() - previewSize.width) / 2,
            (preview.height() - previewSize.height) / 2,
            previewSize.width,
            previewSize.height
        );

        preview.write_to_file(
            previewPath.string().c_str(),
            VImage::option()->set("Q", 100)->set("effort", 6)->set("strip", true)->set("smart_subsample", true)
        );
    } catch(const VError& error) {
        return std::unexpected(error.what());
    }

    return {};
}
