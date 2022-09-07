#pragma once

#include <map>
#include <filesystem>

#include <orchid/net/socket.hpp>
#include <orchid/http/util.hpp>

namespace orchid::http
{
    class Response
    {

    public:
        std::string protocol = "HTTP/1.1";
        CODE code = CODE::OK;
        std::string status = STATUS[CODE::OK];
        Headers headers;
        std::string body;

        Response() = default;
        Response(Socket& socket);
        Response(const std::string& filename);
        Response(CODE code);

        Buffer serialize();
    };
}