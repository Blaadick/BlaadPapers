#include "model/WallpapersModel.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QScreen>
#include <QtConcurrentRun>
#include "Wallpapers.hpp"
#include "util/Loggers.hpp"
#include "util/Seters.hpp"

WallpapersModel& WallpapersModel::inst() {
    static WallpapersModel instance;
    return instance;
}

void WallpapersModel::loadPreviews() {
    QThreadPool::globalInstance()->start([] {
        const QString previewsPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview/";

        for(const auto& screen : QGuiApplication::screens()) {
            const QString screenPreviewsPath = previewsPath + QString::number(screen->geometry().width() * screen->devicePixelRatio()) + "x" + QString::number(screen->geometry().height() * screen->devicePixelRatio()) + '/';
            const QSize screenAspectRatio = screen->geometry().size() / std::gcd(screen->geometry().width(), screen->geometry().height());

            if(QDir screenPreviewsDir(screenPreviewsPath); !screenPreviewsDir.exists()) {
                if(!screenPreviewsDir.mkpath(screenPreviewsPath)) {
                    logError("Cant create previews cache dir {}", screenPreviewsPath.toStdString());
                }
            }

            for(const auto& wallpaper : Wallpapers::getWallpapers()) {
                const QString previewPath = screenPreviewsPath + wallpaper.getName() + ".webp";

                if(QFile previewFile(previewPath); !previewFile.exists()) {
                    auto preview = QImage(wallpaper.getFilePath()).scaled(
                        screenAspectRatio * 20 * screen->devicePixelRatio(),
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation
                    );

                    if(!preview.save(previewPath, "WEBP", 100)) {
                        logError("Unable to save preview to {}", previewPath.toStdString());
                    }
                }
            }
        }
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

void WallpapersModel::refresh() {
    beginResetModel();
    endResetModel();
}

void WallpapersModel::applyWallpaper(const QString& wallpaperName) {
    QThreadPool::globalInstance()->start([=] {
        util::applyWallpaper(wallpaperName);
    });
}
