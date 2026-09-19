// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/DownloadManager.hpp"

#include <yyjson.h>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "network/Downloader.hpp"
#include "util/PathUtils.hpp"

const std::unordered_map<Uri, DownloadData>& DownloadManager::getOngoingDownloads() {
    return ongoingDownloads;
}

void DownloadManager::addOngoingDownload(Uri uri, DownloadData downloadData) {
    ongoingDownloads.emplace(std::move(uri), std::move(downloadData));
    saveOngoingDownloads();
}

void DownloadManager::removeOngoingDownload(const Uri& uri) {
    ongoingDownloads.erase(uri);
    saveOngoingDownloads();
}

std::optional<const DownloadData&> DownloadManager::getOngoingDownload(const Uri& uri) const {
    const auto it = ongoingDownloads.find(uri);
    if(it == ongoingDownloads.end()) {
        return std::nullopt;
    }

    return it->second;
}

void DownloadManager::addDownloader(std::string scheme, uptr<Downloader> downloader) {
    downloaders.emplace(std::move(scheme), std::move(downloader));
}

auto DownloadManager::downloadFile(Uri uri, const std::filesystem::path& downloadDir) -> std::expected<std::filesystem::path, std::string> {
    auto it = downloaders.find(std::string(uri.scheme()));
    if(it == downloaders.end()) {
        return std::unexpected("Unsupported URI scheme");
    }

    return it->second->downloadFile(std::move(uri), downloadDir);
}

const std::filesystem::path& DownloadManager::ongoingDownloadsFilePath() {
    static const auto ongoingDownloadsFilePath = util::localDataDir() / "downloads.json";
    return ongoingDownloadsFilePath;
}

void DownloadManager::loadOngoingDownloads() {
    if(!std::filesystem::exists(ongoingDownloadsFilePath())) {
        saveOngoingDownloads();
        return;
    }

    try {
        auto downloadsJson = JsonArr::parse(ongoingDownloadsFilePath());
        downloadsJson.forEachObj(
            [this](const JsonObj& objVal) {
                auto urlVal = objVal.getString("url");
                auto finalPathVal = objVal.getString("part_file_path");
                auto eTagVal = objVal.getString("etag");

                ongoingDownloads.emplace(
                    Uri(urlVal),
                    DownloadData(
                        finalPathVal,
                        std::string(eTagVal)
                    )
                );
            }
        );
    } catch(const std::exception&) {
        std::filesystem::remove(ongoingDownloadsFilePath());
        ongoingDownloads.clear();
    }
}

void DownloadManager::saveOngoingDownloads() {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_arr(doc);
    yyjson_mut_doc_set_root(doc, root);

    for(const auto& [url, download] : ongoingDownloads) {
        auto downloadData = yyjson_mut_arr_add_obj(doc, root);
        yyjson_mut_obj_add_str(doc, downloadData, "url", url.c_str());
        yyjson_mut_obj_add_str(doc, downloadData, "etag", download.eTag.c_str());
        yyjson_mut_obj_add_str(doc, downloadData, "part_file_path", download.partFilePath.c_str());
    }

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(ongoingDownloadsFilePath().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        std::filesystem::remove(ongoingDownloadsFilePath());
        ongoingDownloads.clear();
    }

    yyjson_mut_doc_free(doc);
}
