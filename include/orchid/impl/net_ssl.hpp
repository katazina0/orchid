#pragma once

#ifdef __linux__
    #include <orchid/impl/net_unix.hpp>
#elif WIN32
    #include <orchid/impl/net_windows.hpp>
#endif