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
    sptr<HttpClient> httpClient
) : wallpaperLoader(std::move(wallpaperLoader)), config(std::move(config)), httpClient(std::move(httpClient)) {}

std::optional<WallhavenFileData> WallhavenHandler::getWallpaperData(const Url& url) const {
    WallhavenFileData wallhavenFileData;

    auto requestUrl = "https://wallhaven.cc/api/v1/w/" + url.path[1];
    if(config->getWallhavenApiKey().has_value()) {
        requestUrl += "?apikey=" + config->getWallhavenApiKey().value();
    }

    auto wallhavenResponse = httpClient->requestString(requestUrl);
    if(!wallhavenResponse.has_value()) {
        return std::nullopt;
    }

    auto doc = yyjson_read(wallhavenResponse->c_str(), wallhavenResponse->size(), YYJSON_READ_NOFLAG);
    if(!doc) {
        return std::nullopt;
    }

    auto root = yyjson_doc_get_root(doc);
    if(!yyjson_is_obj(root)) {
        yyjson_doc_free(doc);
        return std::nullopt;
    }

    auto errorStr = yyjson_obj_get(root, "error");
    if(errorStr) {
        yyjson_doc_free(doc);
        return std::nullopt;
    }

    auto wallpaperData = yyjson_obj_get(root, "data");
    if(!yyjson_is_obj(wallpaperData)) {
        yyjson_doc_free(doc);
        return std::nullopt;
    }

    auto fileLinkData = yyjson_obj_get(wallpaperData, "path");
    if(yyjson_is_str(fileLinkData)) {
        wallhavenFileData.url = unsafe_yyjson_get_str(fileLinkData);
    }

    auto idData = yyjson_obj_get(wallpaperData, "id");
    if(yyjson_is_str(idData)) {
        wallhavenFileData.id = unsafe_yyjson_get_str(idData);
    }

    auto sourceData = yyjson_obj_get(wallpaperData, "source");
    if(yyjson_is_str(sourceData)) {
        wallhavenFileData.source = unsafe_yyjson_get_str(sourceData);
    }

    auto widthData = yyjson_obj_get(wallpaperData, "dimension_x");
    if(yyjson_is_int(widthData)) {
        wallhavenFileData.resolution.width = unsafe_yyjson_get_int(widthData);
    }

    auto heightData = yyjson_obj_get(wallpaperData, "dimension_y");
    if(yyjson_is_int(heightData)) {
        wallhavenFileData.resolution.height = unsafe_yyjson_get_int(heightData);
    }

    yyjson_doc_free(doc);

    return wallhavenFileData;
}

int WallhavenHandler::handle(const Url& url) const {
    if(url.path.size() != 2 || url.path[0] != "w") {
        return 1;
    }

    auto wallpaperData = getWallpaperData(url);
    if(!wallpaperData.has_value()) {
        std::cout << "no wallpaper data";
        return 1;
    }

    auto contentType = httpClient->requestContentType(wallpaperData->url);
    if(!contentType.has_value()) {
        std::cout << "no content type";
        return 1;
    }

    auto fileType = file::getTypeFromMime(*contentType);
    if(!fileType.has_value()) {
        std::cout << "no file type:" << contentType.value();
        return 1;
    }

    if(!wallpaperLoader->getSupportedFileTypes().contains(&*fileType)) {
        std::cout << "file type not supported";
        return 1;
    }

    auto wallpaperFileName = "wallhaven-" + wallpaperData->id + fileType->canonicalExtension();
    auto wallpaperFile = httpClient->downloadFile(
        wallpaperData->url,
        util::localDataDirPath().append("downloads"),
        wallpaperFileName
    );

    if(!wallpaperFile.has_value()) {
        return 1;
    }

    wallpaperLoader->addWallpaper(*wallpaperFile, config->getWallpapersDirPath());
    std::filesystem::remove(*wallpaperFile);

    return 0;
}
