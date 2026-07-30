// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

extern "C" {
    #include <libavformat/avformat.h>
}

#include "data/VideoData.hpp"

inline std::optional<VideoData> getVideoData(const std::filesystem::path& filePath) {
    AVFormatContext* fmt = nullptr;
    AVDictionary* options = nullptr;
    av_dict_set(&options, "probesize", "32", 0);
    av_dict_set(&options, "analyzeduration", "0", 0);

    if(avformat_open_input(&fmt, filePath.string().c_str(), nullptr, &options) < 0) {
        av_dict_free(&options);
        return std::nullopt;
    }

    av_dict_free(&options);

    const AVStream* stream;
    if(const int streamIndex = av_find_best_stream(fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0); streamIndex < 0) {
        avformat_close_input(&fmt);
        return std::nullopt;
    } else {
        stream = fmt->streams[streamIndex];
    }

    auto data = VideoData(
        Size(stream->codecpar->width, stream->codecpar->height),
        static_cast<int>(av_q2d(stream->avg_frame_rate))
    );

    avformat_close_input(&fmt);
    return data;
}
