#pragma once

#include <orchid/net/socket.hpp>
#include <orchid/http/util.hpp>
#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid::http
{
    class Client
    {

    public:
        Client(const std::string& hostname);

        Socket socket;

        Response request(Request& request);

    };
}