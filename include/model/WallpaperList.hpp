#pragma once

#include <QAbstractListModel>
#include "Wallpaper.hpp"

class WallpaperList : public QAbstractListModel {
    Q_OBJECT

public:
    enum WallpaperRoles {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        TagsRole,
        IsBadRole
    };

    explicit WallpaperList(QObject *parent = nullptr);

    static void loadPreviews();

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setWallpaper(const QString& wallpaperName);
};
