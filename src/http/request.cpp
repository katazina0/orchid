#include <orchid/http/request.hpp>

orchid::http::Request::Request(Socket& client)
{
    method = client.read_until(' ');
    client.read(1);

    endpoint = client.read_until(' ');
    client.read(1);

    protocol = client.read_until('\r');
    client.read(2);

    while (true)
    {
        auto header = client.read_until('\r');
        client.read(2);

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
        body = client.read(std::stoul(headers["content-length"]));
    }
    else
    {
        throw std::runtime_error("ill-formed request");
    }
}