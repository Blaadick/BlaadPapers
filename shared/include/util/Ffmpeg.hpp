// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

extern "C" {
    #include <libavformat/avformat.h>
}

#include <data/Size.hpp>

struct VideoData {
    Size resolution = {};
    int frameRate = 0;
};

inline VideoData getVideoData(const std::filesystem::path& filePath) {
    VideoData data;
    AVFormatContext* fmt = nullptr;
    AVDictionary* options = nullptr;
    av_dict_set(&options, "probesize", "32", 0);
    av_dict_set(&options, "analyzeduration", "0", 0);

    if(avformat_open_input(&fmt, filePath.c_str(), nullptr, &options) < 0) {
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

    data.resolution = Size(stream->codecpar->width, stream->codecpar->height);
    data.frameRate = static_cast<int>(av_q2d(stream->avg_frame_rate));

    avformat_close_input(&fmt);
    return data;
}
