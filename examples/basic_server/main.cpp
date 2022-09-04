#include <iostream>

#include <orchid/http/server.hpp>

orchid::HTTPServer server;

void onRequestReceived(orchid::Socket socket, orchid::HTTPRequest request)
{
    std::cout << request.endpoint << "\n";
    orchid::HTTPResponse response;

    
    server.respond(socket, response);
}   

int main()
{
    server.onRequestReceived = onRequestReceived;
    server.run();
}