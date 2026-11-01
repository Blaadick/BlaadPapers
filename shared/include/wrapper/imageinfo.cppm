// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <imageinfo.hpp>

export module imageinfo;

export namespace imageinfo {
    using imageinfo::Format;
    using enum Format;
    using imageinfo::Error;
    using enum Error;

    using imageinfo::FileReader;
    using imageinfo::FilePathReader;
    using imageinfo::FileStreamReader;
    using imageinfo::RawData;
    using imageinfo::RawDataReader;

    using imageinfo::Buffer;
    using imageinfo::ReadFunc;
    using imageinfo::ReadInterface;
    using imageinfo::ImageSize;
    using imageinfo::EntrySizes;
    using imageinfo::ImageInfo;

    using imageinfo::is_numeric;
    using imageinfo::try_avif_heic;
    using imageinfo::try_bmp;
    using imageinfo::try_cur_ico;
    using imageinfo::try_dds;
    using imageinfo::try_gif;
    using imageinfo::try_hdr;
    using imageinfo::try_icns;
    using imageinfo::try_jpeg2000_code_stream;
    using imageinfo::try_jpeg2000;
    using imageinfo::try_jpg;
    using imageinfo::try_ktx;
    using imageinfo::try_png;
    using imageinfo::try_psd;
    using imageinfo::try_qoi;
    using imageinfo::try_tiff;
    using imageinfo::try_webp;
    using imageinfo::try_tga;

    using imageinfo::parse;
}
