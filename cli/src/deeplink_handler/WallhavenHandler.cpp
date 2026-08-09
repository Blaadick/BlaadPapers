// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/WallhavenHandler.hpp"

#include <fstream>
#include <iostream>
#include "option/AddOption.hpp"
#include "util/StringUtils.hpp"

WallhavenHandler::WallhavenHandler(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<Config> config,
    sptr<HttpWorker> httpWorker
) : wallpaperLoader(std::move(wallpaperLoader)), config(std::move(config)), httpWorker(std::move(httpWorker)) {}

int WallhavenHandler::handle(const Url& url) const {
    if(url.path.size() != 2 || url.path[0] != "w") {
        return 1;
    }

    auto requestUrl = "https://wallhaven.cc/api/v1/w/" + url.path[1];
    if(config->getWallhavenApiKey().has_value()) {
        requestUrl += "?apikey=" + config->getWallhavenApiKey().value();
    }

    auto wallhavenData = httpWorker->requestString(requestUrl);
    if(!wallhavenData.has_value()) {
        std::cout << "No wallpaper data";
        return 1;
    }

    auto doc = yyjson_read(wallhavenData->c_str(), wallhavenData->size(), YYJSON_READ_NOFLAG);
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

    std::string wallpaperId = "wallhaven-" + url.path[1];
    std::string wallpaperFileLink;
    std::string wallpaperSource;
    Size wallpaperResolution;

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

    auto widthData = yyjson_obj_get(wallpaperData, "dimension_x");
    if(yyjson_is_int(widthData)) {
        wallpaperResolution.width = unsafe_yyjson_get_int(widthData);
    }

    auto heightData = yyjson_obj_get(wallpaperData, "dimension_y");
    if(yyjson_is_int(heightData)) {
        wallpaperResolution.height = unsafe_yyjson_get_int(heightData);
    }

    yyjson_doc_free(doc);

    auto wallhavenImage = httpWorker->requestBinary(wallpaperFileLink);
    auto wallpaperFolder = config->getWallpapersDirPath() / wallpaperId;

    std::filesystem::create_directory(wallpaperFolder);

    std::ofstream wallpaperFile(wallpaperFolder / "wallpaper.jpeg", std::ios::binary);
    wallpaperFile.write(
        reinterpret_cast<const char*>(wallhavenImage->data()),
        wallhavenImage->size()
    );

    return 0;
}
