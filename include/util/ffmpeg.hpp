// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

extern "C" {
    #include <libavformat/avformat.h>
}

#include <QSize>

inline QSize getVideoResolution(const QString& filePath) {
    AVFormatContext* fmt = nullptr;
    AVDictionary* options = nullptr;
    av_dict_set(&options, "probesize", "32", 0);
    av_dict_set(&options, "analyzeduration", "0", 0);

    if(avformat_open_input(&fmt, filePath.toStdString().c_str(), nullptr, &options) < 0) {
        av_dict_free(&options);
        return {};
    }

    av_dict_free(&options);

    for(int i = 0; i < fmt->nb_streams; ++i) {
        const AVStream* stream = fmt->streams[i];

        if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            const QSize resolution(stream->codecpar->width, stream->codecpar->height);

            avformat_close_input(&fmt);
            return resolution;
        }
    }

    avformat_close_input(&fmt);
    return {};
}
