#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>
#include "util/FormatUtils.hpp"

class Wallpaper {
public:
    Wallpaper(
        const QString& id,
        const QString& format,
        const QString& name,
        const QVector<QString>& tags
    );

    const QString& getId() const;

    const QString& getFormat() const;

    const QString& getName() const;

    const QVector<QString>& getTags() const;

    QString getFilePath() const;

    bool isAnimated() const;

    bool isBad() const;

    QJsonObject toJson() const;

    bool operator==(const Wallpaper& other) const;

private:
    QString id;
    QString format;
    QString name;
    QVector<QString> tags;
};
