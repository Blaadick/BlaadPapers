#include "model/WallpaperList.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QScreen>
#include <QtConcurrentRun>
#include "Wallpapers.hpp"
#include "util/Seters.hpp"

WallpaperList::WallpaperList(QObject* parent) : QAbstractListModel(parent) {}

void WallpaperList::loadPreviews() {
    QThreadPool::globalInstance()->start([] {
        const QString previewsPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview/";

        for(const auto& screen : QGuiApplication::screens()) {
            const QString screenPreviewsPath = previewsPath + QString::number(screen->geometry().width() * screen->devicePixelRatio()) + "x" + QString::number(screen->geometry().height() * screen->devicePixelRatio()) + '/';
            const QSize screenAspectRatio = screen->geometry().size() / std::gcd(screen->geometry().width(), screen->geometry().height());

            if(QDir screenPreviewsDir(screenPreviewsPath); !screenPreviewsDir.exists()) {
                screenPreviewsDir.mkpath(screenPreviewsPath);
            }

            for(const auto& wallpaper : Wallpapers::getWallpapers()) {
                const QString previewPath = screenPreviewsPath + wallpaper.getName() + ".webp";

                if(QFile previewFile(previewPath); !previewFile.exists()) {
                    auto preview = QImage(wallpaper.getFilePath()).scaled(
                        screenAspectRatio * 20 * screen->devicePixelRatio(),
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation
                    );

                    preview.save(previewPath, "WEBP");
                }
            }
        }
    });
}

int WallpaperList::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(Wallpapers::getWallpapers().count());
}

QVariant WallpaperList::data(const QModelIndex& index, const int role) const {
    const Wallpaper& wallpaper = Wallpapers::getWallpapers().at(index.row());

    switch(role) {
        case NameRole: return wallpaper.getName();
        case DescriptionRole: return wallpaper.getDescription();
        case TagsRole: return wallpaper.getTags();
        case IsBadRole: return wallpaper.isBad();
        default: return {};
    }
}

QHash<int, QByteArray> WallpaperList::roleNames() const {
    return {
        {NameRole, "wallpaperName"},
        {DescriptionRole, "wallpaperDescription"},
        {TagsRole, "wallpaperTags"},
        {IsBadRole, "isWallpaperBad"},
    };
}

void WallpaperList::setWallpaper(const QString& wallpaperName) {
    QThreadPool::globalInstance()->start([=] {
        applyWallpaper(wallpaperName);
    });
}
