#include <orchid/http/client.hpp>

orchid::http::Client::Client()
{
    socket.method = TLS_client_method();
}

orchid::http::Client::Client(const std::string& hostname)
{
    socket.hostname = hostname;
    socket.connect(socket.hostname);
}

orchid::http::Response orchid::http::Client::request(Request&& request)
{
    socket.write(request.serialize());
    return Response(socket);
}