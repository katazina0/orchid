#include <orchid/http/server.hpp>

void orchid::HTTPServer::run()
{
    orchid::Socket server;
    server.bind(port);
    server.listen();

    while (true)
    {
        orchid::Socket client = server.accept();

        if (onRequestReceived != nullptr)
            onRequestReceived(client, orchid::HTTPRequest(client));
    }
}

void orchid::HTTPServer::respondTo(const orchid::Socket& socket, const orchid::HTTPResponse& response)
{
    
}