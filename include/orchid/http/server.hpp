#pragma once

#ifdef __linux__
    #include "orchid/impl/unix_socket.hpp"
#elif WIN32
    #include "orchid/impl/win_socket.hpp"
#endif

class HTTPServer
{
    void run();
};