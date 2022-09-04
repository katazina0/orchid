#pragma once

#include <map>

#include <orchid/net/socket.hpp>
#include <orchid/http/util.hpp>

namespace orchid::http
{
    class Request
    {

    public:
        Request() = default;
        Request(Socket& socket);

        std::string method = "GET";
        std::string endpoint = "/";
        std::string protocol = "HTTP/1.1";
        Headers headers;
        std::string body;

        Buffer serialize();

    };
}