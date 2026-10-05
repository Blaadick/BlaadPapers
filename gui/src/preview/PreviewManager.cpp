// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "preview/PreviewManager.hpp"

#include "util/PathUtils.hpp"
#include "util/Screenutils.hpp"
#include "util/ToString.hpp"

PreviewManager::PreviewManager(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void PreviewManager::createAndSavePreviews(const Wallpaper& wallpaper) const {
    auto previewsDirPath = wallpaper.getDirPath() / "preview";

    if(!util::createDirIfNotExists(previewsDirPath)) {
        logger->logWarning(std::format("Failed to create directory \"{}\"", previewsDirPath));
        return;
    }

    for(const QScreen* screen : QGuiApplication::screens()) {
        auto previewFilePath = previewsDirPath / (util::toString(screen) + ".webp");
        auto previewSize = util::getScreenAspectRatio(screen) * 20 * static_cast<int>(screen->devicePixelRatio());

        if(std::filesystem::exists(previewFilePath)) {
            continue;
        }

        auto it = generators.find(typeid(wallpaper));
        if(it == generators.end()) {
            logger->logError(std::format("No preview generator found for {} ({})", wallpaper.getId(), wallpaper.getFilePath().extension()));
            return;
        }

        auto isSaved = it->second->createAndSavePreview(wallpaper, previewSize, previewFilePath);
        if(isSaved) {
            logger->logInfo(std::format("Preview of \"{}\" saved for {}", wallpaper.getId(), util::toString(screen)));
        } else {
            logger->logWarning(std::format("Unable to save preview file \"{}\"", previewFilePath));
        }
    }
}
