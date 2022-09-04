#pragma once

#include <orchid/http/util.hpp>
#include <orchid/net/net_ssl.hpp>
#include <map>

namespace orchid
{
    namespace http
    {
        class Request
        {

        public:
            Request() = default;
            Request(orchid::Socket& socket);

            std::string method = "GET";
            std::string endpoint = "/";
            std::string protocol = "HTTP/1.1";
            std::map<std::string, std::string> headers;
            std::string body;

            orchid::Buffer serialize();

        };
    }
}