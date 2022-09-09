#pragma once

#include <exception>

namespace orchid
{
    class SocketClosedException : std::exception
    {
        
    };

    class PortInUseException : std::exception
    {

    };
}