// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "preview/generator/VideoPreviewGenerator.hpp"

VideoPreviewGenerator::VideoPreviewGenerator() : PreviewGenerator(file::mp4) {}

//TODO Use libswscale
auto VideoPreviewGenerator::createAndSavePreview(
    const Wallpaper& wallpaper,
    const Size& previewSize,
    const std::filesystem::path& previewPath
) const -> std::expected<void, std::string> {
    auto status = system(
        std::format(
            R"(ffmpeg -loglevel quiet -y -i "{0}" -t 5 -vf "fps=24,crop='min(iw,ih*{1}/{2}):min(ih,iw*{2}/{1})',scale={1}:{2}:flags=lanczos" -an -c:v libx264 -preset slow -tune fastdecode -crf 18 -pix_fmt yuv420p -movflags +faststart "{3}")",
            wallpaper.getFilePath().string(),
            previewSize.width,
            previewSize.height,
            previewPath.string()
        ).c_str()
    );

    if(status != 0) {
        return std::unexpected(std::format("ffmpeg exited with status {}", status));
    }

    return {};
}
