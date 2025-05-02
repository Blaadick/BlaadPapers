#include "CacheLoader.hpp"

#include <QApplication>
#include "Util.hpp"
#include "Wallpapers.hpp"

using namespace std;
using namespace filesystem;

void CacheLoader::loadCache() {
    if(!exists(cacheDir)) {
        create_directory(cacheDir);
    }

    for(const auto* screen : QApplication::screens()) {
        path screenCacheFolder = cacheDir / to_string(screen->devicePixelRatio());

        if(!exists(screenCacheFolder)) {
            create_directory(screenCacheFolder);
        }

        for(const auto& wallpaper : Wallpapers::getWallpapers()) {
            path cachedPreviewPath = getPreviewPath(wallpaper, screen->devicePixelRatio());

            if(!exists(cachedPreviewPath)) {
                QImage maxSizedPreview = QImage(wallpaper.getFilePath().c_str()).scaled(
                    getAspectRatio(screen) * 18.4 * screen->devicePixelRatio(),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation
                );

                if(maxSizedPreview.save(cachedPreviewPath.c_str(), "PNG")) {
                    qDebug() << "wallpaper" << wallpaper.getName() << "with screen size" << screen->devicePixelRatio() << "preview caching success";
                } else {
                    qDebug() << "wallpaper" << wallpaper.getName() << "with screen size" << screen->devicePixelRatio() << "preview caching fail";
                }
            }
        }
    }
}

path CacheLoader::getPreviewPath(const Wallpaper& wallpaper, const double devicePixelRatio) {
    return cacheDir / to_string(devicePixelRatio) / (wallpaper.getName() + ".png");
}
