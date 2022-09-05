#include <orchid/http/client.hpp>

orchid::http::Client::Client(const std::string& hostname)
{
    socket.hostname = hostname;
    socket.connect(socket.hostname);
}

orchid::http::Response orchid::http::Client::request(Request& request)
{
    auto serialized = request.serialize();
    socket.write(serialized);
    return Response(socket);
}