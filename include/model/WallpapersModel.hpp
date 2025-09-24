#pragma once

#include <QAbstractListModel>

class WallpapersModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum WallpaperRoles {
        NameRole = Qt::UserRole + 1,
        TagsRole,
        IsBadRole
    };

    static WallpapersModel& inst();

    Q_INVOKABLE void load();

    Q_INVOKABLE void applyWallpaper(const QString& wallpaperName) const;

    Q_INVOKABLE void deleteWallpaper(const QString& wallpaperName) const;

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
};
