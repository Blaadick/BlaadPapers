// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/WallhavenHandler.hpp"

#include <iostream>

#include "option/AddOption.hpp"

WallhavenHandler::WallhavenHandler(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<HttpWorker> httpWorker
) : wallpaperLoader(std::move(wallpaperLoader)), httpWorker(std::move(httpWorker)) {}

int WallhavenHandler::handle(const Url& url) const {
    if(url.path.size() != 2 || url.path[0] != "w") {
        return 1;
    }

    auto response = httpWorker->getString("https://wallhaven.cc/api/v1/w/" + url.path[1]);
    if(!response.has_value()) {
        std::cout << "Failed to get data";
        return 1;
    }

    auto doc = yyjson_read(response->c_str(), response->size(), YYJSON_READ_NOFLAG);
    if(!doc) {
        return 1;
    }

    auto root = yyjson_doc_get_root(doc);
    if(!yyjson_is_obj(root)) {
        yyjson_doc_free(doc);
        return 1;
    }

    auto errorStr = yyjson_obj_get(root, "error");
    if(errorStr) {
        yyjson_doc_free(doc);
        std::cout << "Error in response!";
        return 1;
    }

    std::string wallpaperFileLink;
    std::string wallpaperSource;

    auto wallpaperData = yyjson_obj_get(root, "data");
    if(!yyjson_is_obj(wallpaperData)) {
        yyjson_doc_free(doc);
        return 1;
    }

    auto fileLinkData = yyjson_obj_get(wallpaperData, "path");
    if(yyjson_is_str(fileLinkData)) {
        wallpaperFileLink = unsafe_yyjson_get_str(fileLinkData);
    } else {
        yyjson_doc_free(doc);
        return 1;
    }

    auto sourceData = yyjson_obj_get(wallpaperData, "source");
    if(yyjson_is_str(sourceData)) {
        wallpaperSource = unsafe_yyjson_get_str(sourceData);
    }

    std::cout << wallpaperFileLink << std::endl;
    std::cout << wallpaperSource << std::endl;
    return 0;
}
