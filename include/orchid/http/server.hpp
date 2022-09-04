#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <thread>

#ifdef __linux__
    #include <orchid/net/net_unix.hpp>
#elif WIN32
    #include <orchid/net/net_windows.hpp>
#endif

#include <orchid/http/util.hpp>
#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid
{
    namespace http
    {
        class Server
        {

        public:
            uint16_t port = 8080;

            std::function<void(orchid::Socket, Request)> onRequestReceived = nullptr;

            void run();
            void respond(orchid::Socket& socket, Response&& response);
            void respondFile(orchid::Socket& socket, const std::string& filename);

        };
    }
}