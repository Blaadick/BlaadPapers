#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

class Wallpaper {
public:
    Wallpaper(
        const QString& name,
        const QString& filePath,
        const QVector<QString>& tags
    );

    const QString& getName() const;

    const QString& getFilePath() const;

    const QVector<QString>& getTags() const;

    bool isBad() const;

    QJsonObject toJson() const;

private:
    QString name;
    QString filePath;
    QVector<QString> tags;
};
