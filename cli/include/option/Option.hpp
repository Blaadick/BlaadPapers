// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <vector>

class Option {
public:
    explicit Option(std::string description);

    virtual ~Option() = default;

    [[nodiscard]]
    const std::string& getDescription() const;

    virtual int execute(const std::vector<std::string_view>& arguments) = 0;

private:
    std::string description;
};
