// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <vector>
#include "Size.hpp"

namespace util {
    inline auto join(const std::vector<std::string>& strings, const std::string_view separator = ", ") -> std::string {
        if(strings.empty()) {
            return "";
        }

        if(strings.size() == 1) {
            return strings[0];
        }

        size_t outputSize = 0;
        outputSize += separator.size() * (strings.size() - 1);
        for(size_t i = 1; i < strings.size(); ++i) {
            outputSize += strings[i].size();
        }

        std::string output;
        output.reserve(outputSize);

        output += strings[0];
        for(size_t i = 1; i < strings.size(); ++i) {
            output += separator;
            output += strings[i];
        }

        return output;
    }

    inline auto toString(const Size& size) -> std::string {
        return std::format("{}", size);
    }
}
