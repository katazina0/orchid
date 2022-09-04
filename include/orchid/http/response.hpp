#pragma once

#include <orchid/http/util.hpp>
#include <orchid/net/net_ssl.hpp>
#include <map>

namespace orchid
{
    namespace http
    {
        class Response
        {

        public:
            Response() = default;
            Response(orchid::Socket& socket);
            Response(CODE code);

            std::string protocol = "HTTP/1.1";
            std::string code = "200";
            std::string status = "OK";
            std::map<std::string, std::string> headers;
            std::string body;

            orchid::Buffer serialize();

        };
    }
}