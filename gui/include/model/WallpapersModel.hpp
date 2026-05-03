// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QAbstractListModel>
#include <QVariant>

class WallpapersModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum WallpaperRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        RootDirRole,
        ResolutionRole,
        SourceRole,
        TagsRole,
        TypeRole,
        IsBadRole
    };

    static WallpapersModel& inst();

    void loadWallpapers();

    void addWallpapers(const QStringList& paths, const QString& destinationDirPath);

    Q_INVOKABLE void addWallpapers();

    Q_INVOKABLE void addWallpapers(const QStringList& paths);

    Q_INVOKABLE void applyWallpaper(const QString& wallpaperId) const;

    Q_INVOKABLE void deleteWallpaper(const QString& wallpaperId) const;

    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
};
