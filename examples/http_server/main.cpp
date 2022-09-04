#include <iostream>

#include <orchid/http/server.hpp>

orchid::http::Server server;

void onRequestReceived(orchid::Socket socket, orchid::http::Request request)
{
    std::cout << request.endpoint << "\n";

    if (request.endpoint == "/")
        server.respondFile(socket, "/index.html");
    else
        server.respondFile(socket, request.endpoint);
}   

int main()
{
    server.onRequestReceived = onRequestReceived;
    server.run();
}