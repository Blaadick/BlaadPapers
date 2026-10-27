// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "preview/generator/VideoPreviewGenerator.hpp"
#include "util/PathUtils.hpp"

VideoPreviewGenerator::VideoPreviewGenerator(
    sptr<ShellExecutor> shellExecutor
) : PreviewGenerator(file::mp4), shellExecutor(std::move(shellExecutor)) {}

//TODO Use libswscale
auto VideoPreviewGenerator::createAndSavePreview(
    const Wallpaper& wallpaper,
    const Size& previewSize,
    const std::filesystem::path& previewPath
) const -> std::expected<void, std::string> {
    auto status = shellExecutor->execute(
        std::format(
            R"(ffmpeg -loglevel quiet -y -i "{0}" -t 5 -vf "fps=24,crop='min(iw,ih*{1}/{2}):min(ih,iw*{2}/{1})',scale={1}:{2}:flags=lanczos" -an -c:v libx264 -preset slow -tune fastdecode -crf 18 -pix_fmt yuv420p -movflags +faststart "{3}")",
            wallpaper.getFilePath(),
            previewSize.width,
            previewSize.height,
            previewPath
        )
    );

    if(status.has_value()) {
        if(status == 0) {
            return {};
        }

        return std::unexpected(std::format("Ffmpeg exited with {} code", *status));
    }

    return std::unexpected(std::format("Failed to execute ffmpeg ({})", status.error()));
}
