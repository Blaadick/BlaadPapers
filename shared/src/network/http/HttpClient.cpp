// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/http/HttpClient.hpp"

#include <fstream>
#include <iostream>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "util/PathUtils.hpp"

namespace {
    auto writeToString(char* data, std::size_t size, std::size_t count, void* out) -> std::size_t {
        static_cast<std::string*>(out)->append(data, size * count);
        return size * count;
    }
}

HttpClient::HttpClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    loadOngoingDownloads();
}

HttpClient::~HttpClient() {
    curl_global_cleanup();
}

auto HttpClient::requestString(Uri uri) -> std::expected<std::string, std::string> {
    const std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), curl_easy_cleanup);
    if(!curl) {
        return std::unexpected("curl_easy_init failed");
    }

    std::string body;

    curl_easy_setopt(curl.get(), CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl.get(), CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &body);

    if(auto code = curl_easy_perform(curl.get()); code != CURLE_OK) {
        return std::unexpected(curl_easy_strerror(code));
    }

    return body;
}

auto HttpClient::downloadFile(
    Uri uri,
    const std::filesystem::path& downloadDir
) -> std::expected<std::filesystem::path, std::string> {
    std::error_code ec;
    std::filesystem::create_directories(downloadDir);

    if(ec) {
        return std::unexpected("Cannot create " + downloadDir.string() + ": " + ec.message());
    }

    std::optional<DownloadData> previous;
    if(const auto ongoing = getOngoingDownload(uri)) {
        previous = *ongoing;
    }

    std::uintmax_t offset = 0;
    if(previous) {
        offset = std::filesystem::file_size(previous->partFilePath, ec);

        if(ec) {
            offset = 0;
        }
    }

    std::unique_ptr<curl_slist, decltype(&curl_slist_free_all)> headers(nullptr, curl_slist_free_all);
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curlHandle(curl_easy_init(), curl_easy_cleanup);
    CURL* curl = curlHandle.get();
    if(!curl) {
        return std::unexpected("curl_easy_init failed");
    }

    char errorBuffer[CURL_ERROR_SIZE] = {};
    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

    if(offset > 0) {
        curl_easy_setopt(curl, CURLOPT_RANGE, (std::to_string(offset) + "-").c_str());
        if(!previous->eTag.empty()) {
            headers.reset(curl_slist_append(nullptr, ("If-Range: " + previous->eTag).c_str()));
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.get());
        }
    }

    std::filesystem::path finalPath;
    std::filesystem::path partFilePath;
    std::ofstream file;
    std::string error;

    auto openFile = [&]() -> bool {
        long status = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

        curl_header* header = nullptr;
        std::optional<std::string> filename;
        if(curl_easy_header(curl, "Content-Disposition", 0, CURLH_HEADER, -1, &header) == CURLHE_OK) {
            filename = extractFilename(header->value);
        }
        if(!filename.has_value()) {
            filename = extractFilename(uri);
        }
        if(!filename.has_value()) {
            filename = "file";
        }

        finalPath = downloadDir / *filename;

        if(previous && status == 206) {
            partFilePath = previous->partFilePath;
            file.open(partFilePath, std::ios::binary | std::ios::app);
            if(!file) {
                error = "Cannot open " + partFilePath.string();
                return false;
            }
            return true;
        }

        if(previous) {
            std::error_code ignored;
            removeOngoingDownload(uri);
            std::filesystem::remove(previous->partFilePath, ignored);
        }

        std::string eTag;
        if(curl_easy_header(curl, "ETag", 0, CURLH_HEADER, -1, &header) == CURLHE_OK) {
            eTag = header->value;
        }

        partFilePath = finalPath;
        partFilePath += ".part";
        file.open(partFilePath, std::ios::binary | std::ios::trunc);
        if(!file) {
            error = "Cannot open " + partFilePath.string();
            return false;
        }

        addOngoingDownload(uri, DownloadData{partFilePath, std::move(eTag)});
        return true;
    };

    auto writeBody = [&](const char* data, std::size_t size) -> bool {
        if(!file.is_open() && !openFile()) {
            return false;
        }

        return static_cast<bool>(file.write(data, size));
    };

    using WriteBody = decltype(writeBody);

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        +[](char* data, std::size_t size, std::size_t count, void* userdata) -> std::size_t {
        auto& body = *static_cast<WriteBody*>(userdata);
        return body(data, size * count) ? size * count : 0;
        }
    );
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeBody);

    const CURLcode result = curl_easy_perform(curl);

    long status = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    if(offset > 0 && status == 416) {
        removeOngoingDownload(uri);
        std::filesystem::remove(previous->partFilePath, ec);
        return downloadFile(std::move(uri), downloadDir);
    }

    if(result != CURLE_OK) {
        if(error.empty()) {
            error = errorBuffer[0] ? errorBuffer : curl_easy_strerror(result);
        }
        return std::unexpected(error);
    }

    if(!file.is_open() && !openFile()) {
        return std::unexpected(error);
    }

    file.close();
    if(!file) {
        return std::unexpected("Cannot write " + partFilePath.string());
    }

    std::filesystem::rename(partFilePath, finalPath, ec);
    if(ec) {
        return std::unexpected("Cannot rename " + partFilePath.string() + ": " + ec.message());
    }

    removeOngoingDownload(uri);
    return finalPath;
}

const std::filesystem::path& HttpClient::ongoingDownloadsFilePath() {
    static const auto ongoingDownloadsFilePath = util::localDataDir() / "downloads.json";
    return ongoingDownloadsFilePath;
}

void HttpClient::loadOngoingDownloads() {
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

void HttpClient::saveOngoingDownloads() {
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

void HttpClient::addOngoingDownload(Uri uri, DownloadData downloadData) {
    ongoingDownloads.emplace(std::move(uri), std::move(downloadData));
    saveOngoingDownloads();
}

void HttpClient::removeOngoingDownload(const Uri& uri) {
    ongoingDownloads.erase(uri);
    saveOngoingDownloads();
}

std::optional<const DownloadData&> HttpClient::getOngoingDownload(const Uri& uri) const {
    const auto it = ongoingDownloads.find(uri);
    if(it == ongoingDownloads.end()) {
        return std::nullopt;
    }

    return it->second;
}

auto HttpClient::extractFilename(const std::string_view contentDisposition) const -> std::optional<std::string> {
    auto extractParam = [&](std::string_view paramName) -> std::string_view {
        auto pos = contentDisposition.find(paramName);
        if(pos == std::string_view::npos) {
            return {};
        }

        pos += paramName.size();
        if(pos < contentDisposition.size() && contentDisposition[pos] == '\"') {
            auto quoteEnd = contentDisposition.find('\"', pos + 1);
            return contentDisposition.substr(pos + 1, quoteEnd - pos - 1);
        }

        auto separator = contentDisposition.find(';', pos);
        return contentDisposition.substr(pos, (separator == std::string_view::npos ? contentDisposition.size() : separator) - pos);
    };

    if(auto value = extractParam("filename*="); !value.empty()) {
        auto quotePos = value.rfind('\'');

        if(quotePos != std::string_view::npos) {
            return precentDecode(value.subview(quotePos + 1));
        }

        return std::string(value);
    }

    if(auto value = extractParam("filename="); !value.empty()) {
        return std::string(value);
    }

    return std::nullopt;
}

auto HttpClient::extractFilename(const Uri& uri) const -> std::optional<std::string> {
    auto path = uri.path();
    if(path.empty() || path.back().empty()) {
        return std::nullopt;
    }

    return path.back().data();
}
