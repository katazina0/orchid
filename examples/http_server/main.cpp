#include <iostream>
#include <orchid/http/server.hpp>

using namespace orchid;

http::Server server;

http::Response onExampleRequest(Socket& client, http::Request&& request)
{
    return http::Response(http::CODE::OK);
}

int main()
{
    server.registerEndpoint("/get/example", onExampleRequest);
    server.run();
}