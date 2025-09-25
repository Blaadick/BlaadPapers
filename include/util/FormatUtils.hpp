#pragma once

namespace util {
    inline const QVector<QString> supportedStaticFormats = {
        "png",
        "webp",
        "jpeg"
    };

    inline const QVector<QString> supportedAnimatedFormats = {
        "mp4"
        "webm"
    };

    inline QVector<QString> getFileMask(QVector<QString> vector) {
        for(auto& item : vector) {
            item.prepend("*.");
        }

        return vector;
    }
}
