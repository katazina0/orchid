#pragma once

#include <orchid/net/net_ssl.hpp>
#include <map>

namespace orchid
{
    class HTTPRequest
    {

    public:
        HTTPRequest() = default;
        HTTPRequest(orchid::Socket& socket);

        std::string method = "GET";
        std::string endpoint = "/";
        std::string protocol = "HTTP/1.1";
        std::map<std::string, std::string> headers;
        std::string body;

        orchid::Buffer serialize();

    };
}