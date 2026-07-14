// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QAbstractListModel>
#include <QVariant>
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "logger/Logger.hpp"
#include "preview/PreviewManager.hpp"

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

    WallpapersModel(
        sptr<WallpaperLoader> wallpaperLoader,
        sptr<Wallpapers> wallpapers,
        sptr<Config> config,
        sptr<PreviewManager> previewManager,
        sptr<util::Logger> logger
    );

    void loadWallpapers();

    void addWallpapers(const QStringList& paths, const QString& destinationDirPath);

    /**
     * Launches file dialog
     */
    Q_INVOKABLE void addWallpapers();

    Q_INVOKABLE void addWallpapers(const QStringList& paths);

    Q_INVOKABLE void applyWallpaper(const QString& wallpaperId) const;

    Q_INVOKABLE void deleteWallpaper(const QString& wallpaperId) const;

    Q_INVOKABLE void refreshWallpapers();

    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    sptr<WallpaperLoader> wallpaperLoader;
    sptr<Wallpapers> wallpapers;
    sptr<Config> config;
    sptr<PreviewManager> previewManager;
    sptr<util::Logger> logger;
};
