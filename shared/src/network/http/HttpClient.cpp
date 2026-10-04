// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/http/HttpClient.hpp"

#include <fstream>
#include <iostream>
#include <ranges>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "util/PathUtils.hpp"

namespace {
    using CurlHeaders = std::unique_ptr<curl_slist, decltype(&curl_slist_free_all)>;
    using CurlMulti = std::unique_ptr<CURLM, decltype(&curl_multi_cleanup)>;
    using CurlHandle = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)>;

    auto isSuccess(long status) -> bool {
        return status >= 200 && status < 300;
    }

    auto readHeader(CURL* curl, const char* name) -> std::optional<std::string> {
        curl_header* header = nullptr;
        if(curl_easy_header(curl, name, 0, CURLH_HEADER, -1, &header) != CURLHE_OK) {
            return std::nullopt;
        }
        return std::string(header->value);
    }

    auto writeToString(char* data, std::size_t size, std::size_t count, void* userdata) -> std::size_t {
        static_cast<std::string*>(userdata)->append(data, size * count);
        return size * count;
    }

    auto writeToFile(char* data, std::size_t size, std::size_t count, void* userdata) -> std::size_t {
        auto& file = *static_cast<std::ofstream*>(userdata);
        if(!file.is_open()) {
            return CURL_WRITEFUNC_PAUSE;
        }

        auto length = size * count;
        file.write(data, length);

        return file ? length : 0;
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
    CurlHandle curl(curl_easy_init(), curl_easy_cleanup);
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
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &body);
    curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, PROJECT_USER_AGENT);

    if(auto code = curl_easy_perform(curl.get()); code != CURLE_OK) {
        return std::unexpected(curl_easy_strerror(code));
    }

    return body;
}

namespace {}

auto HttpClient::downloadFile(
    Uri uri,
    const std::filesystem::path& downloadDir
) -> std::expected<std::filesystem::path, std::string> {
    auto ec = std::error_code{};
    std::filesystem::create_directories(downloadDir, ec);
    if(ec) {
        return std::unexpected(std::format("Cannot create dir {} ({})", downloadDir, ec.message()));
    }

    std::optional<DownloadData> previous;
    if(const auto ongoing = getOngoingDownload(uri)) {
        previous = *ongoing;
    }

    std::uintmax_t offset;
    if(previous) {
        offset = std::filesystem::file_size(previous->partFilePath, ec);
        if(ec) {
            offset = 0;
        }
    }

    auto headers = CurlHeaders(nullptr, curl_slist_free_all);
    auto multi = CurlMulti(curl_multi_init(), curl_multi_cleanup);
    auto curl = CurlHandle(curl_easy_init(), curl_easy_cleanup);
    if(!multi || !curl) {
        return std::unexpected("Cannot initialize libcurl");
    }

    std::ofstream file;
    char errorBuffer[CURL_ERROR_SIZE];

    curl_easy_setopt(curl.get(), CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errorBuffer);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeToFile);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, PROJECT_USER_AGENT);

    if(offset > 0) {
        curl_easy_setopt(curl.get(), CURLOPT_RANGE, std::format("{}-", offset).c_str());
        if(!previous->eTag.empty()) {
            headers.reset(curl_slist_append(nullptr, std::format("If-Range: {}", previous->eTag).c_str()));
            curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headers.get());
        }
    }

    const auto addResult = curl_multi_add_handle(multi.get(), curl.get());
    if(addResult != CURLM_OK) {
        return std::unexpected(curl_multi_strerror(addResult));
    }

    auto running = 0;
    auto status = 0L;
    while(true) {
        auto performResult = curl_multi_perform(multi.get(), &running);
        if(performResult != CURLM_OK) {
            return std::unexpected(curl_multi_strerror(performResult));
        }

        curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &status);

        if(running == 0 || isSuccess(status)) {
            break;
        }

        curl_multi_poll(multi.get(), nullptr, 0, 1000, nullptr);
    }

    if(!isSuccess(status)) {
        return std::unexpected(errorBuffer);
    }

    auto resumed = previous && status == 206;

    std::filesystem::path partFilePath;
    if(previous) {
        partFilePath = previous->partFilePath;
    } else {
        std::optional<std::string> filename;
        if(const auto disposition = readHeader(curl.get(), "Content-Disposition")) {
            filename = extractFilename(*disposition);
        }
        if(!filename) {
            filename = extractFilename(uri);
        }

        partFilePath = downloadDir / filename.value_or("file");
        partFilePath += ".part";
    }

    file.open(partFilePath, std::ios::binary | (resumed ? std::ios::app : std::ios::trunc));
    if(!file) {
        return std::unexpected(std::format("Cannot open {}", partFilePath.string()));
    }

    if(!resumed) {
        if(previous) {
            removeOngoingDownload(uri);
        }

        addOngoingDownload(uri, DownloadData{partFilePath, readHeader(curl.get(), "ETag").value_or("")});
    }

    if(running > 0) {
        const auto pauseResult = curl_easy_pause(curl.get(), CURLPAUSE_CONT);
        if(pauseResult != CURLE_OK) {
            return std::unexpected(curl_easy_strerror(pauseResult));
        }
    }

    while(running > 0) {
        const auto performResult = curl_multi_perform(multi.get(), &running);

        if(performResult != CURLM_OK) {
            return std::unexpected(curl_multi_strerror(performResult));
        }

        if(running > 0) {
            curl_multi_poll(multi.get(), nullptr, 0, 1000, nullptr);
        }
    }

    auto messages = 0;
    const auto message = curl_multi_info_read(multi.get(), &messages);
    if(!message || message->data.result != CURLE_OK) {
        return std::unexpected(errorBuffer);
    }

    file.close();
    if(!file) {
        return std::unexpected(std::format("Cannot write {}", partFilePath.string()));
    }

    auto filePath = partFilePath;
    filePath.replace_extension();

    std::filesystem::rename(partFilePath, filePath, ec);
    if(ec) {
        return std::unexpected(std::format("Cannot rename file {} ({})", partFilePath.string(), ec.message()));
    }

    removeOngoingDownload(uri);
    return filePath;
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
                    Uri::parse(std::string(urlVal)).value(),
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
    if(!path.has_value()) {
        return std::nullopt;
    }

    if(path == "/") {
        return std::nullopt;
    }

    size_t lastPathSegmentEndPos;
    size_t lastPathSegmentStartPos;
    if(path->ends_with('/')) {
        lastPathSegmentEndPos = path->size() - 1;
        lastPathSegmentStartPos = path->rfind('/', lastPathSegmentEndPos - 1) + 1;
    } else {
        lastPathSegmentEndPos = path->size();
        lastPathSegmentStartPos = path->rfind('/', lastPathSegmentEndPos) + 1;
    }

    auto fileNameLength = lastPathSegmentEndPos - lastPathSegmentStartPos;
    if(fileNameLength == 0) {
        return std::nullopt;
    }

    return std::string(path->subview(lastPathSegmentStartPos, fileNameLength));
}
