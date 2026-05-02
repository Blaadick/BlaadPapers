// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/PicturePreviewGenerator.hpp"

#include <vips/vips8>

using namespace vips;

bool PicturePreviewGenerator::createAndSavePreview(
    const uptr<Wallpaper>& wallpaper,
    const Size& targetSize,
    const std::filesystem::path& targetPath
) {
    VImage preview = VImage::new_from_file(
        wallpaper->getFilePath().c_str(),
        VImage::option()->set("access", "sequential")->set("fail", true)
    );

    const int shrinkSize = std::max(
        1,
        std::min(
            preview.width() / targetSize.width,
            preview.height() / targetSize.height
        )
    );
    preview = preview.shrink(shrinkSize, shrinkSize);

    const float scale = std::max(
        static_cast<float>(targetSize.width) / static_cast<float>(preview.width()),
        static_cast<float>(targetSize.height) / static_cast<float>(preview.height())
    );
    preview = preview.resize(scale, VImage::option()->set("kernel", VIPS_KERNEL_LANCZOS3));

    preview = preview.crop(
        (preview.width() - targetSize.width) / 2,
        (preview.height() - targetSize.height) / 2,
        targetSize.width,
        targetSize.height
    );

    preview.write_to_file(
        targetPath.c_str(),
        VImage::option()->set("Q", 100)->set("effort", 6)->set("strip", true)->set("smart_subsample", true)
    );
    return true;
}
