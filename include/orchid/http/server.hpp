#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <thread>

#include <orchid/net/socket.hpp>
#include <orchid/http/util.hpp>
#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid::http
{
    class Server
    {

    public:
        uint16_t port = 8080;
        std::map<std::string, std::function<Response(Socket&, Request&&)>> responseRegistry =
        {
            { "/", [](Socket&, Request&&)
                {
                    return Response("/index.html");
                }
            }
        };

        void run();
        void respond(Socket& client, Response&& response);
        void bindEndpoint(const std::string& endpoint, std::function<Response(Socket&, Request&&)> function);
    };
}