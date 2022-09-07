#pragma once

#include <map>

#include <orchid/net/socket.hpp>
#include <orchid/http/util.hpp>

namespace orchid::http
{
    class Request
    {

    public:
        std::string method = "GET";
        std::string endpoint = "/";
        std::string protocol = "HTTP/1.1";
        Headers headers;
        FormData formData;
        std::string body;

        Request() = default;
        Request(Socket& socket, bool read_form = true);

        Buffer serialize();
    };
}