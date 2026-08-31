#pragma once

#include <boost/url.hpp>

template<>
struct std::formatter<boost::url_view> : std::formatter<std::string_view> {
    auto format(const boost::url_view& url, std::format_context& ctx) const {
        return std::formatter<std::string_view>::format(url.buffer(), ctx);
    }
};
