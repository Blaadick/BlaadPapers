// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#undef signals

#include <filesystem>
#include <vips/vips.h>
#include "data/Size.hpp"

inline Size getPictureResolutionData(const std::filesystem::path& filePath) {
    Size resolution;
    VipsImage* image = vips_image_new_from_file(filePath.c_str(), nullptr);

    if(!image) {
        return resolution;
    }

    resolution.width = vips_image_get_width(image);
    resolution.height = vips_image_get_height(image);

    g_object_unref(image);
    return resolution;
}
