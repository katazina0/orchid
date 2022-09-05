#include <iostream>

#include <orchid/http/server.hpp>
#include <orchid/http/client.hpp>

using namespace orchid;

http::Server server;

http::Response onExampleRequest(Socket& client, http::Request&& request)
{
    std::cout << client.getAddress() << " " << request.endpoint << "\n";
    return http::Response();
}

int main()
{
    server.bindEndpoint("/example", onExampleRequest);
    server.run();
}