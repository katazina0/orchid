#include <orchid/http/server.hpp>

orchid::http::Server::Server(bool ssl)
{
    if (ssl)
    {
        socket.method = TLS_server_method();
        socket.ctx = SSL_CTX_new(socket.method);
        socket.ssl = SSL_new(socket.ctx);
        SSL_CTX_use_certificate_chain_file(socket.ctx, "server.crt");
        SSL_CTX_use_PrivateKey_file(socket.ctx, "server.key", SSL_FILETYPE_PEM);
        SSL_set_verify(socket.ssl, SSL_VERIFY_NONE, nullptr);
    }
}

void orchid::http::Server::run()
{
    socket.bind(port);
    socket.listen();

    while (true)
    {
        Socket& client = *new Socket(socket.accept());
        std::jthread([&]
        {
            try
            {
                while (true)
                {
                    auto request = Request(client);
                    if (endpointRegistry.contains(request.endpoint))
                    {
                        respond(client, endpointRegistry[request.endpoint](client, std::forward<Request>(request)));
                    }
                    else
                    {
                        respond(client, Response(request.endpoint));
                    }
                }
            }
            catch (...) 
            {
                SSL_free(client.ssl);
                delete &client;
            }
        }).detach();
    }
}

void orchid::http::Server::respond(Socket& client, Response&& response)
{
    client.write(response.serialize());
}

void orchid::http::Server::bindEndpoint(const std::string& endpoint, std::function<Response(Socket&, Request&&)> function)
{
    endpointRegistry[endpoint] = function;
}