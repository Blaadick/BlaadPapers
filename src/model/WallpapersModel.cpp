#include "model/WallpapersModel.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QScreen>
#include <QtConcurrentRun>
#include "Wallpapers.hpp"
#include "util/Loggers.hpp"
#include "util/PathUtils.hpp"
#include "util/WallpaperUtils.hpp"

WallpapersModel& WallpapersModel::inst() {
    static WallpapersModel instance;
    return instance;
}

void WallpapersModel::load() {
    beginResetModel();
    Wallpapers::load();
    endResetModel();

    QThreadPool::globalInstance()->start([] {
        const QString previewsPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview/";

        for(const auto screen : QGuiApplication::screens()) {
            const QSize screenAspectRatio = screen->geometry().size() / std::gcd(screen->geometry().width(), screen->geometry().height());

            for(const auto& wallpaper : Wallpapers::getWallpapers()) {
                const QString previewPath = util::getPreviewPath(screen, wallpaper.getName());

                if(QFile previewFile(previewPath); !previewFile.exists()) {
                    auto preview = QImage(wallpaper.getFilePath()).scaled(
                        screenAspectRatio * 20 * screen->devicePixelRatio(),
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation
                    );

                    if(!preview.save(previewPath, "WEBP", 100)) {
                        logError("Unable to save preview to {}", previewPath.toStdString());
                        sendStatus("Unable to save preview to {}", previewPath.toStdString());
                    }
                }
            }
        }
    });
}

void WallpapersModel::applyWallpaper(const QString& wallpaperName) const {
    QThreadPool::globalInstance()->start([=] {
        util::applyWallpaper(wallpaperName);
    });
}

void WallpapersModel::deleteWallpaper(const QString& wallpaperName) const {
    QThreadPool::globalInstance()->start([=] {
        util::deleteWallpaper(wallpaperName);
    });
}

int WallpapersModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(Wallpapers::getWallpapers().count());
}

QVariant WallpapersModel::data(const QModelIndex& index, const int role) const {
    const Wallpaper& wallpaper = Wallpapers::getWallpapers().at(index.row());

    switch(role) {
        case NameRole: return wallpaper.getName();
        case TagsRole: return wallpaper.getTags();
        case IsBadRole: return wallpaper.isBad();
        default: return {};
    }
}

QHash<int, QByteArray> WallpapersModel::roleNames() const {
    return {
        {NameRole, "wallpaperName"},
        {TagsRole, "wallpaperTags"},
        {IsBadRole, "isWallpaperBad"},
    };
}
