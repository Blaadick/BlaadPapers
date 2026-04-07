#pragma once

#include "data/Wallpaper.hpp"

class VideoWallpaper : public Wallpaper {
public:
    VideoWallpaper(
        QString id,
        QString filePath,
        QString name,
        QSize resolution,
        int frameRate,
        QString source,
        QVector<QString> tags
    );

    [[nodiscard]]
    int getFrameRate() const;

    [[nodiscard]]
    QJsonObject toJson() const override;

    [[nodiscard]]
    QString toString() const override;

protected:
    int frameRate;
};
