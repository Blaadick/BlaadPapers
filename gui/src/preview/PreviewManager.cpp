// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/PreviewManager.hpp"

#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "model/StatusModel.hpp"
#include "preview/PicturePreviewGenerator.hpp"
#include "preview/VideoPreviewGenerator.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"
#include "util/ToString.hpp"

namespace fs = std::filesystem;

Size getScreenAspectRatio(const QScreen* screen) {
    const auto screenGdc = std::gcd(screen->size().width(), screen->size().height());
    return Size(
        screen->size().width() / screenGdc,
        screen->size().height() / screenGdc
    );
}

void PreviewManager::createAndSavePreview(const uptr<Wallpaper>& wallpaper) {
    const auto previewsDirPath = wallpaper->getDirPath() / "preview";

    if(!util::createDirIfNotExists(previewsDirPath)) {
        util::logWarn("Failed to create directory \"{}\"", previewsDirPath.c_str());
        return;
    }

    for(const QScreen* screen : QGuiApplication::screens()) {
        const auto previewFilePath = previewsDirPath / (util::toString(screen) + ".webp");
        const auto previewSize = getScreenAspectRatio(screen) * 20 * static_cast<int>(screen->devicePixelRatio());

        if(fs::exists(previewFilePath)) {
            continue;
        }

        const auto isSaved = generators[typeid(*wallpaper)]->createAndSavePreview(wallpaper, previewSize, previewFilePath);
        if(isSaved) {
            util::logInfo("Preview of \"{}\" saved for {}", wallpaper->getId(), util::toString(screen));
        } else {
            util::logWarn("Unable to save preview file \"{}\"", previewFilePath.c_str());
            util::sendStatus("Unable to save preview file \"{}\"", previewFilePath.c_str());
        }
    }
}

//TODO check shared. Maybe use unique or ptr
std::unordered_map<std::type_index, sptr<PreviewGenerator>> PreviewManager::generators = {
    {typeid(PictureWallpaper), std::make_shared<PicturePreviewGenerator>()},
    {typeid(VideoWallpaper), std::make_shared<VideoPreviewGenerator>()}
};
