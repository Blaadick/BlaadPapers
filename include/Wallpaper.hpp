#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

class Wallpaper {
public:
    Wallpaper(
        const QString& name,
        const QString& description,
        const QString& picturePath,
        const QVector<QString>& tags
    );

    const QString& getName() const;

    const QString& getDescription() const;

    const QString& getPicturePath() const;

    const QVector<QString>& getTags() const;

    bool isBad() const;

    QJsonObject toJson() const;

private:
    QString name;
    QString description;
    QString picturePath;
    QVector<QString> tags;
};
