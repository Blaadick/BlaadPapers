// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/PreviewManager.hpp"

#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "model/StatusModel.hpp"
#include "preview/PicturePreviewGenerator.hpp"
#include "preview/VideoPreviewGenerator.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtilsExtra.hpp"

namespace fs = std::filesystem;

Size getScreenAspectRatio(const QScreen* screen) {
    const int screenGdc = std::gcd(screen->size().width(), screen->size().height());
    return Size(
        screen->size().width() / screenGdc,
        screen->size().height() / screenGdc
    );
}

void PreviewManager::createAndSavePreview(const uptr<Wallpaper>& wallpaper) {
    for(const auto screen : QGuiApplication::screens()) {
        const auto previewPath = util::previewsDirPath(screen).append(wallpaper->getId() + ".webp");
        const auto previewSize = getScreenAspectRatio(screen) * 20 * screen->devicePixelRatio();

        if(!fs::exists(previewPath.parent_path()) || fs::exists(previewPath)) {
            continue;
        }

        const auto isSaved = generators[typeid(*wallpaper)]->createAndSavePreview(wallpaper, previewSize, previewPath);
        if(isSaved) {
            util::logInfo("Preview of \"{}\" saved for {}", wallpaper->getId(), util::toString(screen));
        } else {
            util::logWarn("Unable to save preview file \"{}\"", previewPath.c_str());
            util::sendStatus("Unable to save preview file \"{}\"", previewPath.c_str());
        }
    }
}

//TODO check shared. Maybe use unique or ptr
std::unordered_map<std::type_index, sptr<PreviewGenerator>> PreviewManager::generators = {
    {typeid(PictureWallpaper), std::make_shared<PicturePreviewGenerator>()},
    {typeid(VideoWallpaper), std::make_shared<VideoPreviewGenerator>()}
};
