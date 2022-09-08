#pragma once

#include <orchid/net/socket.hpp>

#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid::http
{
    class Client
    {

    public:
        Socket socket;
        
        Client()
        {
            socket.method = TLS_client_method();
        }

        Client(const std::string& hostname)
        {
            socket.method = TLS_client_method();
            socket.hostname = hostname;
        }

        Response request(Request&& request)
        {
            if (!socket.connected)
            {
                socket.connect(socket.hostname);
            }
            request.addHeader("host", socket.hostname);
            socket.write(request.serialize());
            return Response(socket);
        }
    };
}