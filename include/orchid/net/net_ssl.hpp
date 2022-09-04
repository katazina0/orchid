#pragma once

#ifdef __linux__
    #include <orchid/net/net_unix.hpp>
#elif WIN32
    #include <orchid/net/net_windows.hpp>
#endif