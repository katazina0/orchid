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
        Response() = default;
        Response(Socket& socket);
        Response(const std::string& filename);
        Response(CODE code);

        std::string protocol = "HTTP/1.1";
        CODE code = CODE::OK;
        std::string status = STATUS[CODE::OK];
        Headers headers;
        std::string body;

        Buffer serialize();

    };
}