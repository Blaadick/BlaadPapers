// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/HttpClient.hpp"

#include <yyjson.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "util/PathUtils.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace urls = boost::urls;
namespace ip = asio::ip;
namespace ssl = asio::ssl;
namespace http = beast::http;

HttpClient::HttpClient() : sslCtx(ssl::context::tlsv12_client), resolver(ioCtx) {
    sslCtx.set_default_verify_paths();
    sslCtx.set_verify_mode(ssl::verify_peer);

    loadOngoingDownloads();
}

std::expected<std::string, std::string> HttpClient::requestString(const boost::url_view& url) {
    auto host = url.host();
    auto target = url.encoded_target();
    if(target.empty()) {
        target = "/";
    }

    try {
        if(url.scheme_id() == urls::scheme::http) {
            auto port = url.has_port() ? url.port() : "80";
            auto endpoints = resolver.resolve(host, port);

            beast::tcp_stream stream(ioCtx);
            stream.connect(endpoints);

            http::request<http::string_body> request(http::verb::get, target, 11);
            request.set(http::field::host, host);
            request.set(http::field::user_agent, PROJECT_USER_AGENT);
            request.set(http::field::connection, "close");
            http::write(stream, request);

            beast::flat_buffer buffer;
            http::response<http::string_body> response;
            http::read(stream, buffer, response);

            beast::error_code ec;
            stream.socket().shutdown(ip::tcp::socket::shutdown_both, ec);

            if(response.result_int() >= 400) {
                return std::unexpected(response.reason());
            }

            return std::move(response.body());
        }

        if(url.scheme_id() == urls::scheme::https) {
            auto port = url.has_port() ? url.port() : "443";
            auto endpoints = resolver.resolve(host, port);

            beast::ssl_stream<beast::tcp_stream> stream(ioCtx, sslCtx);
            if(!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
                return std::unexpected("Failed to set SNI hostname");
            }
            beast::get_lowest_layer(stream).connect(endpoints);
            stream.handshake(ssl::stream_base::client);

            http::request<http::string_body> request(http::verb::get, target, 11);
            request.set(http::field::host, host);
            request.set(http::field::user_agent, PROJECT_USER_AGENT);
            request.set(http::field::connection, "close");
            http::write(stream, request);

            beast::flat_buffer buffer;
            http::response<http::string_body> response;
            http::read(stream, buffer, response);

            beast::error_code ec;
            stream.shutdown(ec);

            if(response.result_int() >= 400) {
                return std::unexpected(response.reason());
            }

            return std::move(response.body());
        }
    } catch(const boost::system::system_error& error) {
        return std::unexpected(std::format("{} [{}]", error.code().message(), error.code().category().name()));
    }

    return std::unexpected("Unsupported url");
}

std::expected<std::filesystem::path, std::string> HttpClient::downloadFile(
    const boost::url_view& url,
    const std::filesystem::path& downloadDir
) {
    auto host = url.host();

    if(!util::createDirIfNotExists(downloadDir)) {
        return std::unexpected(std::format("Failed to create dir {}", downloadDir));
    }

    try {
        beast::error_code ec;

        if(url.scheme_id() == urls::scheme::http) {
            auto port = url.has_port() ? url.port() : "80";
            auto endpoints = resolver.resolve(host, port);

            beast::tcp_stream stream(ioCtx);
            stream.connect(endpoints);

            auto result = performDownload(stream, url, downloadDir);
            stream.socket().shutdown(ip::tcp::socket::shutdown_both, ec);
            return result;
        }

        if(url.scheme_id() == urls::scheme::https) {
            auto port = url.has_port() ? url.port() : "443";
            auto endpoints = resolver.resolve(host, port);

            beast::ssl_stream<beast::tcp_stream> stream(ioCtx, sslCtx);
            if(!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
                return std::unexpected("Failed to set SNI hostname");
            }
            beast::get_lowest_layer(stream).connect(endpoints);
            stream.handshake(ssl::stream_base::client);

            auto result = performDownload(stream, url, downloadDir);
            stream.shutdown(ec);
            return result;
        }
    } catch(const boost::system::system_error& error) {
        return std::unexpected(std::format("{} [{}]", error.code().message(), error.code().category().name()));
    }

    return std::unexpected("Unsupported url");
}

const std::filesystem::path& HttpClient::localDownloadsDirPath() {
    static const auto downloadsDirPath = util::localDataDir() / "downloads";
    return downloadsDirPath;
}

const std::filesystem::path& HttpClient::downloadsInProgressFilePath() {
    static const auto downloadsInProgressFilePath = util::localDataDir() / "downloads.json";
    return downloadsInProgressFilePath;
}

void HttpClient::loadOngoingDownloads() {
    if(!std::filesystem::exists(downloadsInProgressFilePath())) {
        return;
    }

    try {
        auto downloadsJson = JsonArr::parse(downloadsInProgressFilePath());
        downloadsJson.forEachObj(
            [this](const JsonObj& objVal) {
                auto urlVal = objVal.getString("url");
                auto finalPathVal = objVal.getString("part_file_path");
                auto eTagVal = objVal.getString("etag");

                auto url = urls::parse_uri(urlVal).value();

                ongoingDownloads.emplace(
                    std::move(url),
                    DownloadData(
                        finalPathVal,
                        std::string(eTagVal)
                    )
                );
            }
        );
    } catch(const std::exception&) {
        std::filesystem::remove_all(localDownloadsDirPath());
        std::filesystem::remove(downloadsInProgressFilePath());
        ongoingDownloads.clear();
    }
}

void HttpClient::saveOngoingDownloads() {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_arr(doc);
    yyjson_mut_doc_set_root(doc, root);

    for(const auto& [url, download] : ongoingDownloads) {
        auto downloadData = yyjson_mut_arr_add_obj(doc, root);
        yyjson_mut_obj_add_str(doc, downloadData, "url", url.data());
        yyjson_mut_obj_add_str(doc, downloadData, "etag", download.eTag.c_str());
        yyjson_mut_obj_add_str(doc, downloadData, "part_file_path", download.partFilePath.c_str());
    }

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(downloadsInProgressFilePath().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        std::filesystem::remove_all(localDownloadsDirPath());
        std::filesystem::remove(downloadsInProgressFilePath());
        ongoingDownloads.clear();
    }

    yyjson_mut_doc_free(doc);
}

void HttpClient::addOngoingDownload(boost::url_view url, DownloadData downloadData) {
    ongoingDownloads.emplace(url, downloadData);
    saveOngoingDownloads();
}

void HttpClient::removeOngoingDownload(const boost::url_view& url) {
    ongoingDownloads.erase(url);
    saveOngoingDownloads();
}

std::optional<DownloadData> HttpClient::getOngoingDownload(const boost::url_view& url) const {
    const auto it = ongoingDownloads.find(url);
    if(it == ongoingDownloads.end()) {
        return std::nullopt;
    }

    return it->second;
}

std::optional<std::string> HttpClient::getFilename(const beast::string_view contentDisposition) const {
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
            if(auto decoded = urls::make_pct_string_view(value.substr(quotePos + 1))) {
                return decoded->decode();
            }
        }

        return std::string(value);
    }

    if(auto value = extractParam("filename="); !value.empty()) {
        return std::string(value);
    }

    return std::nullopt;
}

std::optional<std::string> HttpClient::getFilename(const urls::url_view& url) const {
    auto segments = url.segments();
    if(segments.empty() || segments.back().empty()) {
        return std::nullopt;
    }

    return segments.back();
}

template<typename Stream>
std::expected<std::filesystem::path, std::string> HttpClient::performDownload(
    Stream& stream,
    const boost::url_view& url,
    const std::filesystem::path& downloadDir
) {
    auto host = url.host();
    auto target = url.encoded_target();
    if(target.empty()) {
        target = "/";
    }

    auto downloadData = getOngoingDownload(url);
    auto isOngoingDownload = downloadData.has_value() && std::filesystem::exists(downloadData->partFilePath);

    http::request<http::empty_body> request(http::verb::get, target, 11);
    request.set(http::field::host, host);
    request.set(http::field::user_agent, PROJECT_USER_AGENT);
    request.set(http::field::connection, "close");
    if(isOngoingDownload) {
        auto existingFileSize = std::filesystem::file_size(downloadData->partFilePath);
        request.set(http::field::range, "bytes=" + std::to_string(existingFileSize) + "-");
        request.set(http::field::if_range, downloadData->eTag);
    }
    http::write(stream, request);

    beast::flat_buffer buffer;
    http::response_parser<http::file_body> parser;
    parser.body_limit(boost::none);
    http::read_header(stream, buffer, parser);

    auto& response = parser.get();

    auto filename = getFilename(response[http::field::content_disposition]);
    if(!filename.has_value()) {
        filename = getFilename(url);
        if(!filename.has_value()) {
            filename = "file";
        }
    }
    auto finalPath = downloadDir / *filename;

    beast::error_code ec;
    std::string partPath;

    if(response.result() == http::status::ok) {
        partPath = std::filesystem::path(finalPath) += ".part";

        addOngoingDownload(url, DownloadData(partPath, response[http::field::etag]));

        response.body().open(partPath.c_str(), beast::file_mode::write, ec);
    } else if(response.result() == http::status::partial_content) {
        partPath = downloadData->partFilePath;

        response.body().open(partPath.c_str(), beast::file_mode::append_existing, ec);
    } else {
        return std::unexpected(response.reason());
    }

    http::read(stream, buffer, parser);
    response.body().close();

    removeOngoingDownload(url);

    std::filesystem::rename(partPath, finalPath);
    return finalPath;
}
