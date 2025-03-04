#pragma once

enum class ImageFormat {
    UNSUPPORTED,
    PNG,
    JPG,
    SVG,
};

inline std::map<std::string, ImageFormat> to_imageFormat = {
    {"unsupported", ImageFormat::UNSUPPORTED},
    {"png", ImageFormat::PNG},
    {"jpg", ImageFormat::JPG},
    {"svg", ImageFormat::SVG}
};

inline std::map<ImageFormat, std::string> toString = {
    {ImageFormat::UNSUPPORTED, "unsupported"},
    {ImageFormat::PNG, "png"},
    {ImageFormat::JPG, "jpg"},
    {ImageFormat::SVG, "svg"},
};
