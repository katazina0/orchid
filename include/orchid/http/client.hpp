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
        Socket socket;

        Client();
        Client(const std::string& hostname);
        
        Response request(Request&& request);
    };
}