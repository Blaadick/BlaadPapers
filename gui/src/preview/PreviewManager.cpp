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

void PreviewManager::createAndSavePreview(const uptr<Wallpaper>& wallpaper) {
    for(const auto screen : QGuiApplication::screens()) {
        const auto previewPath = util::getPreviewsPath(screen) + '/' + wallpaper->getId() + ".webp";
        const auto screenAspectRatio = screen->size() / std::gcd(screen->size().width(), screen->size().height());
        const auto previewSize = screenAspectRatio * 20 * screen->devicePixelRatio();

        if(std::filesystem::exists(previewPath.toStdString())) {
            continue;
        }

        const bool isSaved = generators[typeid(wallpaper)]->createAndSavePreview(wallpaper, previewSize, previewPath, screen);
        if(isSaved) {
            util::logInfo("Preview of \"{}\" saved for {}", wallpaper->getId().toStdString(), util::toString(screen));
        } else {
            util::logWarn("Unable to save preview file \"{}\"", previewPath.toStdString());
            util::sendStatus("Unable to save preview file \"{}\"", previewPath.toStdString());
        }
    }
}

//TODO check shared. Maybe use unique or ptr
std::unordered_map<std::type_index, sptr<PreviewGenerator>> PreviewManager::generators = {
    {typeid(PictureWallpaper), std::make_shared<PicturePreviewGenerator>()},
    {typeid(VideoWallpaper), std::make_shared<VideoPreviewGenerator>()}
};
