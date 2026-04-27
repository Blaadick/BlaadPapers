// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

extern "C" {
    #include <libavformat/avformat.h>
}

#include <QSize>

struct VideoData {
    QSize resolution = QSize();
    int frameRate = 0;
};

struct PictureData {
    QSize resolution = QSize();
};

inline VideoData getVideoData(const QString& filePath) {
    VideoData data;
    AVFormatContext* fmt = nullptr;
    AVDictionary* options = nullptr;
    av_dict_set(&options, "probesize", "32", 0);
    av_dict_set(&options, "analyzeduration", "0", 0);

    if(avformat_open_input(&fmt, filePath.toStdString().c_str(), nullptr, &options) < 0) {
        av_dict_free(&options);
        return data;
    }

    av_dict_free(&options);

    const AVStream* stream;
    if(const int streamIndex = av_find_best_stream(fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0); streamIndex < 0) {
        av_dict_free(&options);
        return data;
    } else {
        stream = fmt->streams[streamIndex];
    }

    data.resolution = QSize(stream->codecpar->width, stream->codecpar->height);
    data.frameRate = static_cast<int>(av_q2d(stream->avg_frame_rate));

    avformat_close_input(&fmt);
    return data;
}

inline PictureData getPictureData(const QString& filePath) {
    PictureData data;
    AVFormatContext* fmt = nullptr;

    if(avformat_open_input(&fmt, filePath.toStdString().c_str(), nullptr, nullptr) < 0) {
        return data;
    }

    if(avformat_find_stream_info(fmt, nullptr) < 0) {
        avformat_close_input(&fmt);
        return data;
    }

    for(unsigned i = 0; i < fmt->nb_streams; ++i) {
        const AVStream* stream = fmt->streams[i];
        const AVCodecParameters* codec = stream->codecpar;

        if(codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            int w = codec->width;
            int h = codec->height;
            data.resolution = {w, h};

            avformat_close_input(&fmt);
            break;
        }
    }

    avformat_close_input(&fmt);
    return data;
}
