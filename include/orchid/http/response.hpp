#pragma once

#include <orchid/net/net_ssl.hpp>
#include <map>

namespace orchid
{
    class HTTPResponse
    {

    public:
        HTTPResponse() = default;
        HTTPResponse(orchid::Socket& socket);

        std::string protocol = "HTTP/1.1";
        std::string code = "200";
        std::string status = "OK";
        std::map<std::string, std::string> headers;
        std::string body;

        orchid::Buffer serialize();

    };
}