#include <orchid/http/server.hpp>

void orchid::http::Server::run()
{
    Socket server;
    server.bind(port);
    server.listen();

    while (true)
    {
        Socket& client = *new Socket(server.accept());
        std::jthread([&]
        {
            try
            {
                while (true)
                {
                    auto request = Request(client);

                    if (responseRegistry.contains(request.endpoint))
                    {
                        respond(client, responseRegistry[request.endpoint](client, std::forward<Request>(request)));
                    }
                    else
                    {
                        respond(client, Response(request.endpoint));
                    }
                }
            }
            catch (...) 
            {
                delete &client;
            }
        }).detach();
    }
}

void orchid::http::Server::respond(Socket& client, Response&& response)
{
    auto serialized = response.serialize();
    client.write(serialized);
}

void orchid::http::Server::registerEndpoint(const std::string& endpoint, std::function<Response(Socket&, Request&&)> function)
{
    responseRegistry[endpoint] = function;
}