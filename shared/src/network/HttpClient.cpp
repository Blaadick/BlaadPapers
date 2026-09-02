// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/HttpClient.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace urls = boost::urls;
namespace ip = asio::ip;
namespace ssl = asio::ssl;
namespace http = beast::http;

std::expected<std::string, std::string> HttpClient::requestString(const boost::url_view& url) const {
    try {
        if(url.scheme_id() == urls::scheme::http) {
            auto host = url.host();
            auto port = url.has_port() ? url.port() : "80";

            asio::io_context ioc;
            ip::tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc);

            const auto endpoints = resolver.resolve(host, port);
            stream.connect(endpoints);

            http::request<http::string_body> request(http::verb::get, url, 11);
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
            auto host = url.host();
            auto port = url.has_port() ? url.port() : "443";

            asio::io_context ioc;
            ssl::context ctx(ssl::context::tlsv12_client);
            ctx.set_default_verify_paths();
            ctx.set_verify_mode(ssl::verify_peer);

            ip::tcp::resolver resolver(ioc);
            beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

            if(!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
                return std::unexpected("Failed to set SNI hostname");
            }

            const auto endpoints = resolver.resolve(host, port);
            beast::get_lowest_layer(stream).connect(endpoints);
            stream.handshake(ssl::stream_base::client);

            http::request<http::string_body> request(http::verb::get, url, 11);
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
    const std::filesystem::path& downloadDir,
    std::function<bool(const HeaderResponse&)> validateFunction
) const {
    return std::unexpected("Downloader not implemented");
}
