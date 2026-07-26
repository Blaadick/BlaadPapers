// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/PreviewManager.hpp"

#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
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

PreviewManager::PreviewManager(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void PreviewManager::createAndSavePreviews(const Wallpaper& wallpaper) const {
    const auto previewsDirPath = wallpaper.getDirPath() / "preview";

    if(!util::createDirIfNotExists(previewsDirPath)) {
        logger->logWarning("Failed to create directory \"" + previewsDirPath.string() + "\"");
        return;
    }

    for(const QScreen* screen : QGuiApplication::screens()) {
        const auto previewFilePath = previewsDirPath / (util::toString(screen) + ".webp");
        const auto previewSize = getScreenAspectRatio(screen) * 20 * static_cast<int>(screen->devicePixelRatio());

        if(fs::exists(previewFilePath)) {
            continue;
        }

        const auto it = generators.find(typeid(wallpaper));
        if(it == generators.end()) {
            logger->logError("No preview generator found for wallpaper \"" + wallpaper.getId() + '\"');
            return;
        }

        const auto isSaved = it->second->createAndSavePreview(wallpaper, previewSize, previewFilePath);
        if(isSaved) {
            logger->logInfo("Preview of \"" + wallpaper.getId() + "\" saved for " + util::toString(screen));
        } else {
            logger->logWarning("Unable to save preview file \"" + previewFilePath.string() + "\"");
        }
    }
}

template<std::derived_from<Wallpaper> T>
void PreviewManager::addGenerator(uptr<PreviewGenerator> generator) {
    generators.emplace(typeid(T), std::move(generator));
}
