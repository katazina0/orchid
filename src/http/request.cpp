#include <orchid/http/request.hpp>

orchid::http::Request::Request(orchid::Socket& socket)
{
    method = socket.read_until(' ');
    socket.read(1);

    endpoint = socket.read_until(' ');
    socket.read(1);

    protocol = socket.read_until('\r');
    socket.read(2);

    while (true)
    {
        auto header = socket.read_until('\r');
        socket.read(2);

        auto div = header.find(':');
        if (div == std::string::npos)
        {
            break;
        }

        auto key = header.substr(0, div);
        headers[key] = header.substr(div + 2);
    }

    if (method == "GET" || method == "DELETE")
    {
        return;
    }

    if (headers.contains("content-length"))
    {
        body = socket.read(std::stoul(headers["content-length"]));
    }
    else
    {
        throw std::runtime_error("ill-formed request");
    }
}