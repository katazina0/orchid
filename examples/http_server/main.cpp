#include <orchid/http.hpp>

using namespace orchid;

http::Server server;

http::Response onExampleRequest(Socket& client, http::Request&& request)
{
    return http::Response();
}

int main()
{
    server.bindEndpoint("/ok", onExampleRequest);
    server.run();
}