#include "model/WallpaperList.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QScreen>
#include <QtConcurrentRun>
#include "Wallpapers.hpp"
#include "util/Seters.hpp"

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
                const QString previewPath = screenPreviewsPath + wallpaper.getName() + ".png";

                if(QFile previewFile(previewPath); !previewFile.exists()) {
                    auto preview = QImage(wallpaper.getPicturePath()).scaled(
                        screenAspectRatio * 20 * screen->devicePixelRatio(),
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation
                    );

                    preview.save(previewPath, "PNG");
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
        case PicturePathRole: return wallpaper.getPicturePath();
        default: return {};
    }
}

QHash<int, QByteArray> WallpaperList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "wallpaperName";
    roles[DescriptionRole] = "wallpaperDescription";
    roles[PicturePathRole] = "wallpaperPicturePath";
    return roles;
}

void WallpaperList::setWallpaper(const QString& picturePath) {
    QThreadPool::globalInstance()->start([=] {
        applyWallpaper(picturePath);
    });
}
