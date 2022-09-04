#pragma once

#include <functional>
#include <map>
#include <string>

#ifdef __linux__
    #include <orchid/net/net_unix.hpp>
#elif WIN32
    #include <orchid/net/net_windows.hpp>
#endif

#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid
{
    class HTTPServer
    {

    public:
        uint16_t port = 8080;

        std::function<void(orchid::Socket, orchid::HTTPRequest)> onRequestReceived = nullptr;

        void run();
        void respond(const orchid::Socket& socket, const orchid::HTTPResponse& response);

    };
}