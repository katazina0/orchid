#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <iostream>

#include <orchid/net/socket.hpp>

#include <orchid/http/exception.hpp>
#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid::http
{
    class Server
    {

    public:
        Socket socket = Socket(false);
        uint16_t port = 0;
        std::map<std::string, std::function<Response(Socket&, Request&&)>> endpointRegistry =
        {
            { "/", [](Socket&, Request&&)
                {
                    return Response("/index.html");
                }
            }
        };

        Server(bool ssl = true)
        {
            if (ssl)
            {
                socket.method = TLS_server_method();
                socket.ctx = SSL_CTX_new(socket.method);
                socket.ssl = SSL_new(socket.ctx);
            }
        }

        void setSSLCertificateChain(const std::string& certificateChain)
        {
            SSL_CTX_use_certificate_chain_file(socket.ctx, certificateChain.c_str());
        }

        void setSSLPrivateKey(const std::string& privateKey)
        {
            SSL_CTX_use_PrivateKey_file(socket.ctx, privateKey.c_str(), SSL_FILETYPE_PEM);
        }

        void run(uint16_t port = 443)
        {
            this->port = port;
            bool err = socket.bind(port);
            socket.listen();

            if (err)
            {
                if (port <= 1024)
                {
                    std::cerr << "port " << port << " might already be in use." << std::endl;
                    std::cerr << "did you forget to run the server as root?" << std::endl;
                }
                else
                {
                    std::cerr << "port " << port << " is already in use." << std::endl;
                }
                std::exit(1);
            }

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
                            if (endpointRegistry.contains(request.getEndpoint()))
                            {
                                respond(client, endpointRegistry[request.getEndpoint()](client, std::forward<Request>(request)));
                            }
                            else
                            {
                                respond(client, Response(request.getEndpoint()));
                            }
                        }
                    }
                    catch (SocketClosedException& exception)
                    {
                        delete &client;
                    }
                    catch (InvalidRequestException& exception)
                    {
                        Response response;
                        response.setStatus(Status::INTERNAL_SERVER_ERROR);
                        respond(client, std::forward<Response>(response));
                        delete &client;
                    }
                    catch (InvalidArgumentException& exception)
                    {
                        Response response;
                        response.setStatus(Status::INTERNAL_SERVER_ERROR);
                        respond(client, std::forward<Response>(response));
                        delete &client;
                    }
                }).detach();
            }
        }

        void respond(Socket& client, Response&& response)
        {
            client.write(response.serialize());
        }

        void bindEndpoint(const std::string& endpoint, std::function<Response(Socket&, Request&&)> function)
        {
            endpointRegistry[endpoint] = function;
        }
    };
}