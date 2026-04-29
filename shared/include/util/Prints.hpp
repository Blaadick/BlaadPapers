// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <iostream>

namespace std {
    inline void print(const std::string& str) {
        std::cout << str;
    }

    inline void println(const char* str) {
        std::cout << str << '\n';
    }

    inline void println(const std::string& str) {
        std::cout << str << '\n';
    }

    inline void println(const std::string_view& str) {
        std::cout << str << '\n';
    }

    inline void println(const int number) {
        std::cout << number << '\n';
    }
}
