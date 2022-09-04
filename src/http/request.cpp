#include <orchid/http/request.hpp>

orchid::HTTPRequest::HTTPRequest(orchid::Socket& socket)
{
    method = socket.read_until<std::string>(' ');
    socket.read(1);

    endpoint = socket.read_until<std::string>(' ');
    socket.read(1);

    protocol = socket.read_until<std::string>('\r');
    socket.read(2);

    while (true)
    {
        auto header = socket.read_until<std::string>('\r');
        socket.read(2);

        auto div = header.find(':');
        if (div == std::string::npos)
            break;

        headers[header.substr(0, div)] = header.substr(div + 2);
    }

    if (method == "GET")
        return;

    
}