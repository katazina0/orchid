#pragma once

#ifdef __linux__
    #include <orchid/impl/net_unix.hpp>
#elif WIN32
    #include <orchid/impl/net_windows.hpp>
#endif

namespace orchid
{
    class HTTPServer
    {

    private:
        uint16_t _port = 8080;

    public:
        void run();

        uint16_t getPort();
        void setPort(uint16_t port);
    };
}