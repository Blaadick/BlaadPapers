// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/VersionOption.hpp"

#include <format>
#include "flag/Flags.hpp"

VersionOption::VersionOption(sptr<util::Logger> logger) : Option("Shows program version"), logger(std::move(logger)) {}

std::vector<std::string_view> VersionOption::getUsageStrings() const {
    return {"[flags...]"};
}

int VersionOption::execute(const std::vector<std::string_view>&, const std::unordered_set<sptr<Flag>>& flags) {
    if(flags.contains(Flags::json)) {
        const auto doc = yyjson_mut_doc_new(nullptr);
        const auto root = yyjson_mut_obj(doc);

        yyjson_mut_obj_add_str(doc, root, "name", PROJECT_NAME);
        yyjson_mut_obj_add_str(doc, root, "description", PROJECT_DESCRIPTION);
        yyjson_mut_obj_add_str(doc, root, "version", PROJECT_VERSION);

        yyjson_mut_doc_set_root(doc, root);

        logger->logInfo(yyjson_mut_write(doc, YYJSON_WRITE_NOFLAG, nullptr));
        yyjson_mut_doc_free(doc);
    } else {
        logger->logInfo(std::format("{} {}\n{}", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION));
    }

    return 0;
}
